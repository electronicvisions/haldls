# Hardware Abstraction Layer (and STAteful encapsulation) for the HICANN-DLS

The software in this repository has been developed by staff and students
of Heidelberg University as part of the research carried out by the
Electronic Vision(s) group at the Kirchhoff-Institute for Physics.
The research is funded by Heidelberg University, the State of
Baden-Württemberg, the European Union Sixth Framework Programme no.
15879 (FACETS), the Seventh Framework Programme under grant agreements
no 604102 (HBP), 269921 (BrainScaleS), 243914 (Brain-i-Nets), the
Horizon 2020 Framework Programme under grant agreement 720270 (HBP) as
well as from the Manfred Stärk Foundation.

This repository contains

* HALDLS -- the data structures for configuring the hardware
* STADLS -- code for experiment control and experiment encapsulation

## How to build
### Outside of visionary environment
* Prepare a fresh workspace: `mkdir workspace && cd workspace`
* Fetch a current copy of the waf build tool:
  `git clone https://github.com/electronicvisions/waf -b symwaf2ic symwaf2ic && cd symwaf2ic && make && cd .. && ln -s symwaf2ic/waf`
* Clone the necessary repos
  `./waf setup --repo-db-url=https://github.com/electronicvisions/projects --project=haldls` (`--clone-depth=1` to skip history)
* To build:
  `singularity exec --app visionary-dls /path/to/container ./waf configure build --test-execnone`
  Singularity containers can be found [here](https://brainscales-r.kip.uni-heidelberg.de/containers/).
* To install into `workspace/{bin,lib}`:
  `singularity exec --app visionary-dls /containers/stable/latest ./waf install --test-execnone`
* To run (software) tests:
  `singularity exec --app visionary-dls /containers/stable/latest ./waf install --test-execall`
* To load the current local directory in path environment variables:
  `export SINGULARITYENV_PREPEND_PATH=$PWD/bin:$SINGULARITYENV_PREPEND_PATH` and
  `export SINGULARITYENV_LD_LIBRARY_PATH=$PWD/bin:$SINGULARITYENV_LD_LIBRARY_PATH` for library and binary access

### On hel
* On hel, see `cat /etc/motd` for latest instructions
* prepare a fresh workspace: `mkdir workspace && cd workspace`
* Load the most recent waf module: `module load waf`
* Clone the necessary repos: `waf setup --project haldls`
* To build:
  `srun -p compile -c8 singularity exec --app visionary-dls /containers/stable/latest waf configure build --test-execnone`
* To install into `workspace/{bin,lib}`:
  `srun -p compile -c8 singularity exec --app visionary-dls /containers/stable/latest waf install --test-execnone`
* To run (software) tests:
  `srun -p short -c8 singularity exec --app visionary-dls /containers/stable/latest waf install --test-execall`
* To set environment variables allowing for library and binary access (c.f. `export` commands above):
  `module load localdir`

## How to add a new container (HICANN-X)

### Creating a new container

A container in haldls abstracts the payload to a smallest-possible/feasible read/write operation using `fisch` register-access containers.
To each container, a unique `coordinate_type` coordinate from `halco` describes the range of accessible locations of this container unit:
```cpp
class MyNewContainer
{
public:
    typedef halco::hicann_dls::vx::MyNewContainerOnDLS coordinate_type;
};
```

Containers can either issue register read/write operations and are then called leaf nodes or use existing containers for higher-order structuring.
A leaf node container has to have a `is_leaf_node` typedef:
```
typedef std::true_type is_leaf_node;
```
A non-leaf node container on the other hand provides a `has_local_data` typedef:
```
typedef std::false_type has_local_data;
```

#### Creating a leaf-node container

A leaf node container provides a compile-time fixed number of register-access coordinates and for writing data payload values via the `addresses` and `encode` member functions.
A `decode` member function is used to decode a compile-time fixed number of register values to container data.

If the container only supports register access via one `fisch` register container type, the function signatures look like:
```cpp
class MyNewContainer
{
public:
    typedef halco::hicann_dls::vx::MyNewContainerOnDLS coordinate_type;

    constexpr static size_t config_size_in_words = N;

    std::array<typename fisch::vx::MyNewRegister::coordinate_type, config_size_in_words> addresses() const;

    std::array<fisch::vx::MyNewRegister, config_size_in_words> encode() const;

    void decode(std::array<fisch::vx::MyNewRegister, config_size_in_words> const& words);
};
```

If multiple register types are supportes, e.g. `fisch::vx::OmnibusOnChipOverJTAG` and `fisch::vx::Omnibus` for omnibus register access on the chip, templated functions provide specializationsfor the different register types:
```cpp
class MyNewContainer
{
public:
    typedef halco::hicann_dls::vx::MyNewContainerOnDLS coordinate_type;

    constexpr static size_t config_size_in_words = N;

    template <typename AddressT>
    std::array<AddressT, config_size_in_words> addresses() const;

    template <typename WordT>
    std::array<WordT, config_size_in_words> encode() const;

    template <typename WordT>
    void decode(std::array<WordT, config_size_in_words> const& words);
};

// *.cpp

template <>
std::array<typename fisch::vx::SupportedRegisterType::coordinate_type, config_size_in_words> MyNewContainer::addresses() const
{ /* ... */ }

template <>
std::array<typename fisch::vx::AnotherSupportedRegisterType::coordinate_type, config_size_in_words> MyNewContainer::addresses() const
{ /* ... */ }

// ... same for encode and decode ...
```

In addition to supporting a common number of words for reading and writing configuration, separated numbers can be supplied by providing `read_addresses` and `write_addresses` functions replacing to `addresses` with `{read,write}_config_size_in_words` number of entries.
The associated `encode` function is to provide `write_config_size_in_words` number of words and `decode` is to take `read_config_size_in_words` number of register words as argument:
```cpp
class MyNewContainer
{
public:
    typedef halco::hicann_dls::vx::MyNewContainerOnDLS coordinate_type;

    constexpr static size_t read_config_size_in_words = N;
    constexpr static size_t write_config_size_in_words = M;

    std::array<typename fisch::vx::MyNewRegister::coordinate_type, read_config_size_in_words> read_addresses() const;
    std::array<typename fisch::vx::MyNewRegister::coordinate_type, write_config_size_in_words> write_addresses() const;

    std::array<fisch::vx::MyNewRegister, write_config_size_in_words> encode() const;

    void decode(std::array<fisch::vx::MyNewRegister, read_config_size_in_words> const& words);
};
```

See `DACChannel` in `include/haldls/vx/spi.h` for a single-register-type container implementation, `PPUMemoryWord` for a container with support for two register types.

For the new leaf node container, a `BackendContainerTrait` specialization is to be provided:
```cpp
namespace haldls::vx::detail {

template <>
BackendContainerTrait<MyNewContainer> : public BackendContainerBase<MyNewContainer, fisch::vx::DefaultRegister, fisch::vx::AnotherSupportedRegister>

}
```
The first register container specified in the list is used as the default type in `PlaybackProgramBuilder::{read,write}` if no explicit register container to set the transport protocol is given.


#### Creating a non leaf-node container

A composed non leaf-node container can only be created for containers with (a) common `fisch` register container(s).

A composed container typically has the following form:
```cpp
class MyComposedContainer
{
public:
    typedef std::false_type has_local_data;

private:
    // some collection of other haldls containers
    halco::common::typed_array<MyNewContainer, typename MyNewContainer::coordinate_type> m_entries;
};
```

Opposed to leaf-node containers, a non leaf-node container does not feature `addresses`, `encode` and `decode` functions, as the already implemented features of the member containers are used by applying a visitor pattern.
A visitor pattern specifies the order of encoding/decoding operations performed on the member containers.
It is to be defined for each non leaf-node container as described in the following:
```cpp
namespace haldls::vx::detail {

template <>
struct VisitPreorderImpl<MyComposedContainer>
{
    template <typename ContainerT, typename VisitorT>
    static void call(ContainerT& config, typename MyComposedContainer::coordinate_type const& coord, VisitorT&& visitor)
    {
        visitor(coord, config);

        for (auto const entry_coord : halco::common::iter_all<typename MyNewContainer::coordinate_type>()) {
            visit_preorder(config.m_entries[entry_coord], entry_coord, visitor);
        }
    }
};

} // namespace haldls::vx::detail
```

For a non leaf-node container the `BackendContainerTrait` is to be specified the same way as for leaf-node containers:
```cpp
namespace haldls::vx::detail {

template <>
BackendContainerTrait<MyComposedContainer> : public BackendContainerBase<MyComposedContainer, fisch::vx::DefaultCommonRegister, fisch::vx::AnotherSupportedCommonRegister>

}
```
As for leaf-node containers, the first register container specified in the list is used as the default type in `PlaybackProgramBuilder::{read,write}` if no explicit register container to set the transport protocol is given.

#### Integrating a new container for builder and python support.

After having done the above steps the container is to be integrated in order to be supported by the `PlaybackProgramBuilder` and be available in python.
The container is to be added into the list in `include/haldls/vx/container.def` for support by the builder.
For python bindings, the header containing the container declaration is to be included in `include/haldls/vx/container.h`.

In addition to the above interface, each container is to support serialization by using `cereal` and an `std::ostream` operator for printing its content, see `JTAGClockScaler` for an exemplary implementation using debug printout of the encoded words.

#### Testing a new container

Software tests of containers are to be written in `C++`. A complete software test includes

* `General` tests
  - Testing all member functions for value access
  - Testing the equality and inequality operators
  - Testing value ranges of ranged types if applicable
* Testing encoding and decoding for unity if bijective and against fixed expected values
* Testing serialization coverage with `cereal`, which includes an identity test for altered members and adding the container in the list of tested types in `tests/sw/haldls/vx/test-serialization_cereal.cpp`

For e.g. random member changes, test helpers can be found in `tests/sw/haldls/common/test-helper.h`.
See `tests/sw/haldls/vx/test-dac_control.cpp` for an exemplary test.
