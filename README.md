# Hardware Abstraction Layer (and STAteful encapsulation) for the HICANN-DLS

The software in this repository has been developed by staff and students
of Heidelberg University as part of the research carried out by the
Electronic Vision(s) group at the Kirchhoff-Institute for Physics.
The research is funded by Heidelberg University, the State of
Baden-Württemberg, the European Union Sixth Framework Programme no.
15879 (FACETS), the Seventh Framework Programme under grant agreements
no 604102 (HBP), 269921 (BrainScaleS), 243914 (Brain-i-Nets), the
Horizon 2020 Framework Programme under grant agreement 720270, 785907, 945539
(HBP) as well as from the Manfred Stärk Foundation.

This repository contains

* HALDLS -- the data structures for configuring the hardware
* STADLS -- code for experiment control and experiment encapsulation

## How to build
### Build- and runtime dependencies
All build- and runtime dependencies are encapsulated in a [Singularity Container](https://sylabs.io/docs/).
If you want to build this project outside the Electronic Vision(s) cluster, please start by downloading the most recent version from [here](https://openproject.bioai.eu/containers/).

For all following steps, we assume that the most recent Singularity container is located at `/containers/stable/latest` – you are free to choose any other path.

### Github-based build
To build this project from public resources, adhere to the following guide:

```shell
# 1) Most of the following steps will be executed within a singularity container
#    To keep the steps clutter-free, we start by defining an alias
shopt -s expand_aliases
alias c="singularity exec --app dls-core /containers/stable/latest"

# 2) Add the cross-compiler and toolchain for the embedded processor to your environment
#    If you don't have access to the module, you may build it as noted here:
#    https://github.com/electronicvisions/oppulance
module load ppu-toolchain

# 3) Prepare a fresh workspace and change directory into it
mkdir workspace && cd workspace

# 4) Fetch a current copy of the symwaf2ic build tool
git clone https://github.com/electronicvisions/waf -b symwaf2ic symwaf2ic

# 5) Build symwaf2ic
c make -C symwaf2ic
ln -s symwaf2ic/waf

# 6) Setup your workspace and clone all dependencies (--clone-depth=1 to skip history)
c ./waf setup --repo-db-url=https://github.com/electronicvisions/projects --project=haldls

# 7) Build the project
#    Adjust -j1 to your own needs, beware that high parallelism will lead to high memory consumption!
c ./waf configure
c ./waf build -j1

# 8) Install the project to ./bin and ./lib
c ./waf install

# 9) If you run programs outside waf, you'll need to add ./lib and ./bin to your path specifications
export SINGULARITYENV_PREPEND_PATH=`pwd`/bin:$SINGULARITYENV_PREPEND_PATH
export SINGULARITYENV_LD_LIBRARY_PATH=`pwd`/lib:$SINGULARITYENV_LD_LIBRARY_PATH
export PYTHONPATH=`pwd`/lib:$PYTHONPATH

# 10) You can now run any program, e.g. plain unit tests
c ./bin/stadls_swtest_vx_v3
```

### On the Electronic Vision(s) Cluster

* Work on the frontend machine, `helvetica`. You should have received instructions how to connect to it.
* Follow [aforementioned instructions](#github-based-build) with the following simplifications
  * Replace **steps 4) and 5)** by `module load waf`
  * Make sure to run **step 7)** within a respective slurm allocation: Prefix e.g. `srun -p einc -c128 --mem 0` and remove the `-j1` option; depending on your shell, you might need to roll out the `c`-alias.
  * Replace **step 9)** by `module load localdir`.

### Build from internal sources

If you have access to the internal *Gerrit* server, you may drop the `--repo-db-url`-specification in **step 6)** of [aforementioned instructions](#github-based-build).


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

    static std::array<typename fisch::vx::MyNewRegister::coordinate_type, config_size_in_words> addresses();

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

    static std::array<typename fisch::vx::MyNewRegister::coordinate_type, read_config_size_in_words> read_addresses();
    static std::array<typename fisch::vx::MyNewRegister::coordinate_type, write_config_size_in_words> write_addresses();

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
