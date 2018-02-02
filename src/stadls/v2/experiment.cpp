#include "stadls/v2/experiment.h"

#include <chrono>
#include <sstream>
#include <thread>

#include "flyspi-rw_api/flyspi_com.h"
#include "halco/common/iter_all.h"
#include "log4cxx/logger.h"
#include "uni/bytewise_output_iterator.h"
#include "uni/decoder.h"
#include "uni/program_builder.h"

#include "haldls/container/v2/board.h"
#include "haldls/container/v2/chip.h"
#include "haldls/container/v2/common.h"
#include "haldls/container/v2/playback.h"
#include "haldls/container/v2/spike.h"
#include "haldls/exception/exceptions.h"
#include "stadls/v2/ocp.h"
#include "stadls/visitors.h"

namespace {

// vvv ------8<----------- (legacy code copied from frickel-dls)

struct Sdram_block_write_allocator
{
	static size_t const block_size = 4096;

	typedef rw_api::flyspi::SdramBlockWriteQuery Container;
	typedef uni::Bytewise_output_iterator<rw_api::FlyspiCom::Data, rw_api::FlyspiCom::BufferPtr>
		Iterator;

	rw_api::FlyspiCom& com;
	rw_api::FlyspiCom::Locator loc;
	uint32_t address;

	Sdram_block_write_allocator(rw_api::FlyspiCom& com, uint32_t address)
		: com(com), address(address)
	{
		loc = com.locate().chip(0);
	}

	Iterator begin(Container& c) { return uni::bytewise(std::begin(c)); }
	Iterator end(Container& c) { return uni::bytewise(std::end(c)); }
	Container allocate(size_t capacity)
	{
		rw_api::flyspi::SdramBlockWriteQuery rv(com, loc, capacity);
		rv.resize(capacity);
		rv.addr(address);
		address += capacity;
		return rv;
	}
};

// ^^^ ------8<-----------

struct UniDecoder
{
	std::vector<haldls::container::v2::hardware_word_type> words;
	haldls::container::v2::hardware_time_type current_time = 0;
	std::vector<haldls::container::v2::RecordedSpike> spikes;

	template <typename T>
	void operator()(T const& /*inst*/)
	{}

	void operator()(uni::Write_inst const& inst) { words.push_back(inst.data); }

	void operator()(uni::Set_time_inst const& inst) { current_time = inst.t; }

	void operator()(uni::Wait_until_inst const& inst) { current_time = inst.t; }

	void operator()(uni::Wait_for_7_inst const& inst) { current_time += inst.t; }

	void operator()(uni::Wait_for_16_inst const& inst) { current_time += inst.t; }

	void operator()(uni::Wait_for_32_inst const& inst) { current_time += inst.t; }

	void operator()(uni::Fire_inst const& inst)
	{
		using namespace haldls::container::v2;
		using namespace halco::hicann_dls::v2;
		using namespace halco::common;
		assert(inst.fire.size() == SynapseBlock::Synapse::Address::size);
		for (auto const address : iter_all<NeuronOnDLS>()) {
			if (!inst.fire.test(NeuronOnDLS::max - address))
				continue;
			spikes.emplace_back(current_time, address);
		}
	}

	void operator()(uni::Fire_one_inst const& inst)
	{
		using namespace haldls::container::v2;
		using namespace halco::hicann_dls::v2;
		assert(inst.index < NeuronOnDLS::size);
		spikes.emplace_back(current_time, NeuronOnDLS(NeuronOnDLS::max - inst.index));
	}
};

} // namespace

namespace stadls {
namespace v2 {

class ExperimentControl::Impl
{
public:
	typedef haldls::container::v2::hardware_word_type hardware_word_type;
	typedef haldls::container::v2::hardware_address_type hardware_address_type;

	Impl(std::string const& usb_serial_number) : com(usb_serial_number) {}

	rw_api::FlyspiCom com;

	haldls::container::v2::PlaybackProgram::serial_number_type program_serial_number =
		haldls::container::v2::PlaybackProgram::invalid_serial_number;
	static constexpr hardware_address_type program_address = 0;
	hardware_address_type program_size = 0;
	static constexpr hardware_address_type result_address = 0;
};

ExperimentControl::ExperimentControl(std::string const& usb_serial_number)
	: m_impl(new Impl(usb_serial_number))
{
	soft_reset();
}

ExperimentControl::ExperimentControl(ExperimentControl&&) noexcept = default;

ExperimentControl& ExperimentControl::operator=(ExperimentControl&&) noexcept = default;

ExperimentControl::~ExperimentControl() = default;

void ExperimentControl::soft_reset()
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	halco::common::Unique unique;

	// Set dls and soft reset
	haldls::container::v2::FlyspiConfig reset_config;
	reset_config.set_dls_reset(true);
	reset_config.set_soft_reset(true);
	ocp_write(m_impl->com, unique, reset_config);

	// Set default config
	ocp_write(m_impl->com, unique, haldls::container::v2::FlyspiConfig());
}

void ExperimentControl::configure_static(
	haldls::container::v2::Board const& board, haldls::container::v2::Chip const& chip)
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	// An experiment always happens as follows:
	// * Set the board config, including DACs, spike router and FPGA config
	// * Set the spike router
	// * Set the digital config and wait for the cap-mem to settle

	halco::common::Unique const coord;

	// Set the board
	ocp_write(m_impl->com, coord, board);

	// If the dls is in reset during playback of a playback program, the FPGA
	// will never stop execution for v2 and freeze the FPGA. Therefore, the
	// playback of programs is prohibited if the DLS is in reset.
	if (board.get_flyspi_config().get_dls_reset()) {
		auto log = log4cxx::Logger::getLogger(__func__);
		LOG4CXX_WARN(log, "DLS in reset during configuration");
		LOG4CXX_WARN(log, "The chip configuration cannot be written");
	} else {
		// Chip configuration program
		haldls::container::v2::PlaybackProgramBuilder setup_builder;
		setup_builder.set_time(0);
		setup_builder.set_container(halco::common::Unique(), chip);
		// Wait for the cap-mem to settle (based on empirical measurement by DS)
		// clang-format off
		setup_builder.wait_for(2'000'000); // ~ 20.8 ms for 96 MHz
		// clang-format on
		setup_builder.halt();
		haldls::container::v2::PlaybackProgram setup = setup_builder.done();

		run(setup);
	}
}

void ExperimentControl::transfer(haldls::container::v2::PlaybackProgram const& playback_program)
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	auto const program_serial_number = playback_program.serial_number();
	if (program_serial_number == haldls::container::v2::PlaybackProgram::invalid_serial_number)
		throw std::logic_error("trying to transfer program with invalid state");

	m_impl->program_serial_number = program_serial_number;

	// vvv ------8<----------- (legacy code copied from frickel-dls)

	using namespace rw_api::flyspi;

	std::vector<SdramBlockWriteQuery> queries;
	std::vector<SdramRequest> reqs;
	Sdram_block_write_allocator alloc(m_impl->com, m_impl->program_address);

	// copy to USB buffer memory and transfer
	for (auto const& container : playback_program.instruction_byte_blocks()) {
		queries.push_back(alloc.allocate(container.size() / 4));

		auto it_in = std::begin(container);
		auto it_out = uni::bytewise(std::begin(queries.back()));
		while (it_in != std::end(container)) {
			*it_out = *it_in;
			++it_in;
			++it_out;
		}

		reqs.push_back(queries.back().commit());
	}

	// wait for completion of transfer
	for (auto& req : reqs) {
		req.wait();
	}

	// ^^^ ------8<-----------

	m_impl->program_size = alloc.address - m_impl->program_address;

	// write program address, size and result pointer
	haldls::container::v2::FlyspiProgramAddress program_address(m_impl->program_address);
	haldls::container::v2::FlyspiProgramSize program_size(m_impl->program_size);
	haldls::container::v2::FlyspiResultAddress result_address(m_impl->result_address);

	halco::common::Unique unique;

	ocp_write(m_impl->com, unique, program_address);
	ocp_write(m_impl->com, unique, program_size);
	ocp_write(m_impl->com, unique, result_address);
}

void ExperimentControl::execute()
{
	auto log = log4cxx::Logger::getLogger(__func__);

	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	if (m_impl->program_serial_number == haldls::container::v2::PlaybackProgram::invalid_serial_number ||
		m_impl->program_size == 0)
		throw std::runtime_error("no valid playback program has been transferred yet");

	halco::common::Unique unique;

	// check that the DLS is not in reset
	auto config = ocp_read<haldls::container::v2::FlyspiConfig>(m_impl->com, unique);
	if (config.get_dls_reset()) {
		LOG4CXX_ERROR(log, "Asking to execute a program although the DLS is in reset.");
		LOG4CXX_ERROR(log, "This is prohibited for v2 as it will freeze the system.");
		throw haldls::exception::InvalidConfiguration(
			"Refuse to execute playback program with DLSv2 in reset");
	}

	// start execution by setting the execute bit
	haldls::container::v2::FlyspiControl control;
	control.set_execute(true);
	LOG4CXX_DEBUG(log, "start execution");
	ocp_write(m_impl->com, unique, control);

	// wait until execute bit is cleared again
	{
		std::chrono::microseconds sleep_till_poll(50);
		while (control.get_execute()) {
			LOG4CXX_DEBUG(
				log, "execute flag not yet cleared, sleep for " << sleep_till_poll.count() << "us");
			std::this_thread::sleep_for(sleep_till_poll);
			control = ocp_read<haldls::container::v2::FlyspiControl>(m_impl->com, unique);
			// Increase exponential sleep time
			sleep_till_poll *= 2;
		}
	}
	LOG4CXX_DEBUG(log, "execution finished");
}

void ExperimentControl::fetch(haldls::container::v2::PlaybackProgram& playback_program)
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	if (m_impl->program_serial_number == haldls::container::v2::PlaybackProgram::invalid_serial_number ||
		m_impl->program_size == 0)
		throw std::runtime_error("no valid playback program has been transferred yet");

	// get result size
	halco::common::Unique unique;
	auto result_size = ocp_read<haldls::container::v2::FlyspiResultSize>(m_impl->com, unique);
	if (!result_size.get_value()) {
		throw std::logic_error("no result size read from board");
	}

	// vvv ------8<----------- (legacy code copied from frickel-dls)

	using namespace rw_api::flyspi;

	// transfer data back
	auto loc = (m_impl->com).locate().chip(0);
	SdramBlockReadQuery q_read((m_impl->com), loc, result_size.get_value().value());
	q_read.addr(0x08000000 + m_impl->result_address);

	auto r_read = q_read.commit();
	r_read.wait();

	// ^^^ ------8<-----------

	// extract read/write results from data
	UniDecoder decoder;
	uni::decode(uni::bytewise(std::begin(r_read)), uni::bytewise(std::end(r_read)), decoder);

	playback_program.set_results(std::move(decoder.words));
	playback_program.set_spikes(std::move(decoder.spikes));
}

void ExperimentControl::run(haldls::container::v2::PlaybackProgram& playback_program)
{
	transfer(playback_program);
	execute();
	fetch(playback_program);
}

std::vector<std::string> available_board_usb_serial_numbers()
{
	std::vector<std::string> result;
	char const* const ptr = std::getenv("SLURM_FLYSPI_ID");
	if (ptr == nullptr)
		return result;
	std::istringstream board_ids(ptr);
	std::string token;
	while (std::getline(board_ids, token, ','))
		result.push_back(token);
	return result;
}

} // namespace v2
} // namespace stadls
