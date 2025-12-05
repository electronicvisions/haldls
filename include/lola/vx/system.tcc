#pragma once

#include "lola/vx/system.h"

#include "halco/common/iter_all.h"
#include "haldls/vx/traits.h"
#include <boost/hana/adapt_struct.hpp>


namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::ChipAndMultichipJboaLeafFPGA>
    : public BackendContainerBase<
          lola::vx::ChipAndMultichipJboaLeafFPGA,
          fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::ChipAndMultichipJboaLeafFPGA>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename lola::vx::ChipAndMultichipJboaLeafFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		visit_preorder(
		    config.fpga, lola::vx::ChipAndMultichipJboaLeafFPGA::FPGA::coordinate_type(), visitor);

		visit_preorder(
		    config.chip, lola::vx::ChipAndMultichipJboaLeafFPGA::Chip::coordinate_type(), visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename lola::vx::ChipAndMultichipJboaLeafFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		hate::Empty<lola::vx::MultichipJboaLeafFPGA> empty_fpga;
		visit_preorder(
		    empty_fpga, lola::vx::ChipAndMultichipJboaLeafFPGA::FPGA::coordinate_type(), visitor);

		hate::Empty<lola::vx::v3::Chip> empty_chip;
		visit_preorder(
		    empty_chip, lola::vx::ChipAndMultichipJboaLeafFPGA::Chip::coordinate_type(), visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename lola::vx::ChipAndMultichipJboaLeafFPGA::coordinate_type> const&
	        coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		visit_preorder(
		    config.fpga, lola::vx::ChipAndMultichipJboaLeafFPGA::FPGA::coordinate_type(), visitor);

		visit_preorder(
		    config.chip, lola::vx::ChipAndMultichipJboaLeafFPGA::Chip::coordinate_type(), visitor);
	}
};


template <>
struct BackendContainerTrait<lola::vx::ChipAndSinglechipFPGA>
    : public BackendContainerBase<
          lola::vx::ChipAndSinglechipFPGA,
          fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::ChipAndSinglechipFPGA>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename lola::vx::ChipAndSinglechipFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		visit_preorder(
		    config.fpga, lola::vx::ChipAndSinglechipFPGA::FPGA::coordinate_type(), visitor);

		visit_preorder(
		    config.chip, lola::vx::ChipAndSinglechipFPGA::Chip::coordinate_type(), visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename lola::vx::ChipAndSinglechipFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		hate::Empty<lola::vx::SinglechipFPGA> empty_fpga;
		visit_preorder(
		    empty_fpga, lola::vx::ChipAndSinglechipFPGA::FPGA::coordinate_type(), visitor);

		hate::Empty<lola::vx::v3::Chip> empty_chip;
		visit_preorder(
		    empty_chip, lola::vx::ChipAndSinglechipFPGA::Chip::coordinate_type(), visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename lola::vx::ChipAndSinglechipFPGA::coordinate_type> const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		visit_preorder(
		    config.fpga, lola::vx::ChipAndSinglechipFPGA::FPGA::coordinate_type(), visitor);

		visit_preorder(
		    config.chip, lola::vx::ChipAndSinglechipFPGA::Chip::coordinate_type(), visitor);
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::ChipAndMultichipJboaLeafFPGA, fpga, chip);
BOOST_HANA_ADAPT_STRUCT(lola::vx::ChipAndSinglechipFPGA, fpga, chip);