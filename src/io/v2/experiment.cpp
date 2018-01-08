#include "haldls/io/v2/experiment.h"

#include <sstream>

#include "flyspi-rw_api/flyspi_com.h"
#include "frickel-dls/com.h"
#include "frickel-dls/execute.h"
#include "halco/common/iter_all.h"
#include "uni/bytewise_output_iterator.h"
#include "uni/decoder.h"
#include "uni/program_builder.h"

#include "haldls/container/v2/board.h"
#include "haldls/container/v2/chip.h"
#include "haldls/container/v2/common.h"
#include "haldls/container/v2/spike.h"
#include "haldls/io/v2/playback.h"
#include "haldls/io/visitors.h"

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

namespace haldls {
namespace io {
namespace v2 {

class ExperimentControl::Impl
{
public:
	typedef container::v2::hardware_word_type hardware_word_type;
	typedef container::v2::hardware_address_type hardware_address_type;

	Impl(std::string const& usb_serial_number) : connection(frickel_dls::connect(usb_serial_number))
	{}

	frickel_dls::Connection connection;

	PlaybackProgram::serial_number_type program_serial_number =
		PlaybackProgram::invalid_serial_number;
	static constexpr hardware_address_type program_address = 0;
	hardware_address_type program_size = 0;
	static constexpr hardware_address_type result_address = 0;
	hardware_address_type result_size = 0;
};

ExperimentControl::ExperimentControl(std::string const& usb_serial_number)
	: m_impl(new Impl(usb_serial_number))
{}

ExperimentControl::ExperimentControl(ExperimentControl&&) noexcept = default;

ExperimentControl& ExperimentControl::operator=(ExperimentControl&&) noexcept = default;

ExperimentControl::~ExperimentControl() = default;

void ExperimentControl::soft_reset()
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	frickel_dls::soft_reset(m_impl->connection);
}

void ExperimentControl::configure_static(
	container::v2::Board const& board, container::v2::Chip const& chip)
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	// An experiment always happens as follows:
	// * Soft reset of the FPGA
	// * Set the external DACs
	// * Set the spike router
	// * Set the digital config and wait for the cap-mem to settle

	frickel_dls::soft_reset(m_impl->connection);

	typedef std::vector<haldls::container::v2::ocp_address_type> ocp_addresses_type;
	typedef std::vector<haldls::container::v2::ocp_word_type> ocp_words_type;

	halco::common::Unique const coord;

	ocp_addresses_type addresses;
	visit_preorder(board, coord, haldls::io::WriteAddressVisitor<ocp_addresses_type>{addresses});

	ocp_words_type words;
	visit_preorder(board, coord, haldls::io::EncodeVisitor<ocp_words_type>{words});

	if (words.size() != addresses.size())
		throw std::logic_error("number of OCP addresses and words do not match");

	auto& com = *(m_impl->connection.com());
	auto const loc = com.locate().chip(0);
	auto addr_it = addresses.cbegin();
	for (auto const& word : words) {
		rw_api::flyspi::ocpWrite(com, loc, addr_it->value, word.value);
		++addr_it;
	}

	// Chip configuration program
	PlaybackProgramBuilder setup_builder;
	setup_builder.set_time(0);
	setup_builder.set_container(halco::common::Unique(), chip);
	// Wait for the cap-mem to settle (based on empirical measurement by DS)
	setup_builder.wait_for(2'000'000); // ~ 20.8 ms for 96 MHz
	setup_builder.halt();
	PlaybackProgram setup = setup_builder.done();

	run(setup);
}

void ExperimentControl::transfer(PlaybackProgram const& playback_program)
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	auto const program_serial_number = playback_program.serial_number();
	if (program_serial_number == PlaybackProgram::invalid_serial_number)
		throw std::logic_error("trying to transfer program with invalid state");

	m_impl->program_serial_number = program_serial_number;
	m_impl->result_size = 0;

	// vvv ------8<----------- (legacy code copied from frickel-dls)

	using namespace rw_api::flyspi;

	std::vector<SdramBlockWriteQuery> queries;
	std::vector<SdramRequest> reqs;
	Sdram_block_write_allocator alloc(*m_impl->connection.com(), m_impl->program_address);

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
}

void ExperimentControl::execute()
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	if (m_impl->program_serial_number == PlaybackProgram::invalid_serial_number ||
		m_impl->program_size == 0)
		throw std::runtime_error("no valid playback program has been transferred yet");

	frickel_dls::start_execution(
		m_impl->connection, m_impl->program_address, m_impl->program_size, m_impl->result_address);
	m_impl->result_size = frickel_dls::wait_for_execution(m_impl->connection);
}

void ExperimentControl::fetch(PlaybackProgram& playback_program)
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	if (m_impl->program_serial_number == PlaybackProgram::invalid_serial_number ||
		m_impl->program_size == 0)
		throw std::runtime_error("no valid playback program has been transferred yet");

	// vvv ------8<----------- (legacy code copied from frickel-dls)

	using namespace rw_api::flyspi;

	// transfer data back
	auto loc = m_impl->connection.com()->locate().chip(0);
	SdramBlockReadQuery q_read(*m_impl->connection.com(), loc, m_impl->result_size);
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

void ExperimentControl::run(PlaybackProgram& playback_program)
{
	transfer(playback_program);
	execute();
	fetch(playback_program);
}

ExperimentControl connect(std::string const& usb_serial_number)
{
	return {usb_serial_number};
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
} // namespace io
} // namespace haldls
