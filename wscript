#!/usr/bin/env python
import argparse
from enum import Enum, auto
import os
from os.path import join
import re
import yaml
from waflib.Errors import BuildError
from waflib.extras.symwaf2ic import get_toplevel_path
from waflib.Task import TaskSemaphore
from waflib.TaskGen import feature, after_method


def depends(ctx):
    ctx('code-format')
    ctx('logger')
    ctx('halco')
    ctx('hate')
    ctx('fisch')
    ctx('ztl')

    if getattr(ctx.options, 'with_haldls_python_bindings', True):
        ctx('halco', 'pyhalco')
        ctx('hxcomm', 'pyhxcomm')
        ctx('logger', 'pylogging')


def options(opt):
    opt.load('compiler_cxx')
    opt.load('gtest')
    opt.load('doxygen')

    opt.recurse("pyhaldls")
    opt.recurse("pystadls")
    opt.recurse("pylola")
    opt.recurse("dlens")

    hopts = opt.add_option_group('HALDLS options')
    hopts.add_withoption('haldls-python-bindings', default=True,
                         help='Toggle the generation and build of haldls python bindings')
    hopts.add_option("--disable-coverage-reduction", default=False,
                     action="store_true",
                     help="Disable test coverage (and runtime!) reduction. "
                          "Currently only applies to hardware/software "
                          "cosimulation runs, other tests are never reduced.")


def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.load('gtest')

    cfg.env.build_with_haldls_python_bindings = cfg.options.with_haldls_python_bindings

    cfg.check_cxx(mandatory=True, header_name='cereal/cereal.hpp')
    cfg.load('local_rpath')
    cfg.load('doxygen')

    cfg.env.CXXFLAGS_HALDLS_LIBRARIES = [
        '-fvisibility=hidden',
        '-fvisibility-inlines-hidden',
    ]
    cfg.env.LINKFLAGS_HALDLS_LIBRARIES = [
        '-fvisibility=hidden',
        '-fvisibility-inlines-hidden',
    ]

    cfg.check(lib='elf', header='libelf.h', uselib_store='ELF')

    if cfg.env.build_with_haldls_python_bindings:
        cfg.recurse("pyhaldls")
        cfg.recurse("pystadls")
        cfg.recurse("pylola")
        cfg.recurse("dlens")


@feature('apply_semaphore')
@after_method('apply_link', 'process_source')
def apply_semaphore(self):
    semaphore = getattr(self, 'semaphore')
    assert isinstance(semaphore, TaskSemaphore)
    for tsk in self.tasks:
        if not hasattr(tsk, 'semaphore'):
            tsk.semaphore = semaphore


def build(bld):
    class TestTarget(Enum):
        SOFTWARE_ONLY = auto()
        HARDWARE = auto()
        SIMULATION = auto()

    if "FLANGE_SIMULATION_RCF_PORT" in os.environ:
        bld.env.TEST_TARGET = TestTarget.SIMULATION

        try:
            chip_revision = int(os.environ.get("SIMULATED_CHIP_REVISION"))
        except TypeError:
            raise BuildError("Environment variable 'SIMULATED_CHIP_REVISION' "
                             "not set or cannot be casted to integer.")
        bld.env.CURRENT_SETUP = dict(chip_revision=chip_revision)
    elif "SLURM_HWDB_ENTRIES" in os.environ:
        bld.env.TEST_TARGET = TestTarget.HARDWARE
        slurm_licenses = os.environ.get("SLURM_HARDWARE_LICENSES")
        hwdb_entries = os.environ.get("SLURM_HWDB_ENTRIES")
        fpga_id = int(re.match(r"W(?P<wafer>\d+)F(?P<fpga>\d+)",
                               slurm_licenses)["fpga"])
        bld.env.CURRENT_SETUP = yaml.full_load(hwdb_entries)["fpgas"][fpga_id]
    else:
        bld.env.TEST_TARGET = TestTarget.SOFTWARE_ONLY
        bld.env.CURRENT_SETUP = dict(chip_revision=None)

    if bld.options.disable_coverage_reduction:
        bld.env.SIMTEST_TIMEOUT_SECONDS = 75 * 3600
        bld.env.REDUCED_SIMTESTS_DEFINES = ["REDUCED_TESTS=0",
                                            "MAX_WORDS_PER_REDUCED_TEST=10"]
    else:
        bld.env.SIMTEST_TIMEOUT_SECONDS = 1 * 14400
        bld.env.REDUCED_SIMTESTS_DEFINES = ["REDUCED_TESTS=1",
                                            "MAX_WORDS_PER_REDUCED_TEST=10"]

    bld(
        target = 'haldls_inc',
        export_includes = 'include',
    )

    bld(
        target = 'haldls_vx_v1',
        source = bld.path.ant_glob('src/haldls/vx/*.cpp')
               + bld.path.ant_glob('src/haldls/vx/v1/*.cpp'),
        install_path = '${PREFIX}/lib',
        features = 'cxx cxxshlib pyembed',
        use = ['haldls_inc', 'halco_hicann_dls_vx_v1', 'fisch_vx'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld(
        target = 'haldls_vx_v2',
        source = bld.path.ant_glob('src/haldls/vx/*.cpp')
               + bld.path.ant_glob('src/haldls/vx/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        features = 'cxx cxxshlib pyembed',
        use = ['haldls_inc', 'halco_hicann_dls_vx_v2', 'fisch_vx'],
        uselib = 'HALDLS_LIBRARIES',
    )

    reduced_jobs = max(bld.jobs // 8, 1)
    bld.env['stadls_semaphore'] = TaskSemaphore(reduced_jobs)

    bld(
        target = 'stadls_vx_v1',
        source = bld.path.ant_glob('src/stadls/vx/*.cpp')
               + bld.path.ant_glob('src/stadls/vx/v1/*.cpp'),
        install_path = '${PREFIX}/lib',
        features = 'cxx cxxshlib pyembed apply_semaphore',
        use = ['haldls_vx_v1', 'lola_vx_v1', 'logger_obj'],
        uselib = 'HALDLS_LIBRARIES',
        semaphore = bld.env['stadls_semaphore'],
    )

    bld(
        target = 'stadls_vx_v2',
        source = bld.path.ant_glob('src/stadls/vx/*.cpp')
               + bld.path.ant_glob('src/stadls/vx/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        features = 'cxx cxxshlib pyembed apply_semaphore',
        use = ['haldls_vx_v2', 'lola_vx_v2', 'logger_obj'],
        uselib = 'HALDLS_LIBRARIES',
        semaphore = bld.env['stadls_semaphore'],
    )

    bld(
        features = 'cxx cxxshlib pyembed',
        target = 'lola_vx_v1',
        source = bld.path.ant_glob('src/lola/vx/*.cpp')
               + bld.path.ant_glob('src/lola/vx/v1/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_vx_v1', 'ELF'],
        uselib = 'LOLA_LIBRARIES',
    )

    bld(
        features = 'cxx cxxshlib pyembed',
        target = 'lola_vx_v2',
        source = bld.path.ant_glob('src/lola/vx/*.cpp')
               + bld.path.ant_glob('src/lola/vx/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_vx_v2', 'ELF'],
        uselib = 'LOLA_LIBRARIES',
    )

    bld(
        target = 'stadls_swtest_vx_v1',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/stadls/vx/test-*.cpp')
               + bld.path.ant_glob('tests/sw/stadls/vx/v1/test-*.cpp'),
        use = ['haldls_vx_v1', 'stadls_vx_v1', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'stadls_swtest_vx_v2',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/stadls/vx/test-*.cpp')
               + bld.path.ant_glob('tests/sw/stadls/vx/v2/test-*.cpp'),
        use = ['haldls_vx_v2', 'stadls_vx_v2', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'haldls_test_common_inc',
        export_includes = 'tests/common',
    )

    bld(
        target = 'haldls_swtest_vx_v1',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/haldls/vx/test-*.cpp')
               + bld.path.ant_glob('tests/sw/haldls/vx/v1/test-*.cpp'),
        use = ['haldls_vx_v1', 'haldls_test_common_inc', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'haldls_swtest_vx_v2',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/haldls/vx/test-*.cpp')
               + bld.path.ant_glob('tests/sw/haldls/vx/v2/test-*.cpp'),
        use = ['haldls_vx_v2', 'haldls_test_common_inc', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'lola_swtest_vx_v1',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/lola/vx/test-*.cpp')
               + bld.path.ant_glob('tests/sw/lola/vx/v1/test-*.cpp'),
        use = ['lola_vx_v1', 'GTEST', 'haldls_test_common_inc'],
        install_path = '${PREFIX}/bin',
        defines = ['TEST_PPU_PROGRAM="' + join(get_toplevel_path(), 'haldls', 'tests', 'sw', 'lola', 'lola_ppu_test_elf_file.bin') + '"'],
        test_timeout=60
    )

    bld(
        target = 'lola_swtest_vx_v2',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/lola/vx/test-*.cpp')
               + bld.path.ant_glob('tests/sw/lola/vx/v2/test-*.cpp'),
        use = ['lola_vx_v2', 'GTEST', 'haldls_test_common_inc'],
        install_path = '${PREFIX}/bin',
        defines = ['TEST_PPU_PROGRAM="' + join(get_toplevel_path(), 'haldls', 'tests', 'sw', 'lola', 'lola_ppu_test_elf_file.bin') + '"'],
        test_timeout=60
    )

    bld(
        target = 'stadls_hwtest_vx_v1',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/hw/stadls/vx/hw/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/common/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/v1/hw/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/v1/common/test-*.cpp'),
        test_main = 'tests/hw/stadls/vx/main.cpp',
        use = ['haldls_vx_v1', 'stadls_vx_v1', 'GTEST', 'haldls_test_common_inc', 'ZTL'],
        defines = ['REDUCED_TESTS=0', 'MAX_WORDS_PER_REDUCED_TEST=10'],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
        skip_run = not (bld.env.TEST_TARGET == TestTarget.HARDWARE and
                        int(bld.env.CURRENT_SETUP["chip_revision"]) == 1)
    )

    bld(
        target = 'stadls_hwtest_vx_v2',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/hw/stadls/vx/hw/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/common/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/v2/hw/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/v2/common/test-*.cpp'),
        test_main = 'tests/hw/stadls/vx/main.cpp',
        use = ['haldls_vx_v2', 'stadls_vx_v2', 'GTEST', 'haldls_test_common_inc', 'ZTL'],
        defines = ['REDUCED_TESTS=0', 'MAX_WORDS_PER_REDUCED_TEST=10'],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
        skip_run = not (bld.env.TEST_TARGET == TestTarget.HARDWARE and
                        int(bld.env.CURRENT_SETUP["chip_revision"]) == 2)
    )

    bld(
        target = 'stadls_simtest_vx_v1',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/hw/stadls/vx/sim/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/common/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/v1/sim/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/v1/common/test-*.cpp'),
        test_main = 'tests/hw/stadls/vx/main.cpp',
        use = ['haldls_vx_v1', 'GTEST', 'stadls_vx_v1', 'haldls_test_common_inc'],
        defines = bld.env.REDUCED_SIMTESTS_DEFINES + ["SIMULATION_TEST=1"],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
        skip_run = not (bld.env.TEST_TARGET == TestTarget.SIMULATION and
                        int(bld.env.CURRENT_SETUP["chip_revision"]) == 1),
        test_timeout = bld.env.SIMTEST_TIMEOUT_SECONDS
    )

    bld(
        target = 'stadls_simtest_vx_v2',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/hw/stadls/vx/sim/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/common/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/v2/sim/test-*.cpp')
               + bld.path.ant_glob('tests/hw/stadls/vx/v2/common/test-*.cpp'),
        test_main = 'tests/hw/stadls/vx/main.cpp',
        use = ['haldls_vx_v2', 'GTEST', 'stadls_vx_v2', 'haldls_test_common_inc'],
        defines = bld.env.REDUCED_SIMTESTS_DEFINES + ["SIMULATION_TEST=1"],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
        skip_run=not (bld.env.TEST_TARGET == TestTarget.SIMULATION and
                      int(bld.env.CURRENT_SETUP["chip_revision"]) == 2),
        test_timeout = bld.env.SIMTEST_TIMEOUT_SECONDS
    )

    bld(
        target = 'run_ppu_program_vx_v1',
        features = 'cxx cxxprogram pyembed',
        # TODO: switch to vx/v1
        source = 'tools/stadls/vx/v1/run_ppu_program.cpp',
        use = ['haldls_vx_v1', 'logger_obj', 'stadls_vx_v1'],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
    )

    bld(
        target = 'run_ppu_program_vx_v2',
        features = 'cxx cxxprogram pyembed',
        # TODO: switch to vx/v2
        source = 'tools/stadls/vx/v2/run_ppu_program.cpp',
        use = ['haldls_vx_v2', 'logger_obj', 'stadls_vx_v2'],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
    )

    if bld.env.build_with_haldls_python_bindings:
        bld.recurse("pyhaldls")
        bld.recurse("pystadls")
        bld.recurse("pylola")
        bld.recurse("dlens")

    bld(
        target = 'doxygen_haldls',
        doxy_inputs = 'include/haldls',
        features = 'doxygen',
        doxyfile = bld.root.make_node('%s/code-format/doxyfile' % get_toplevel_path()),
        install_path = 'doc/haldls',
        pars = {
            "PROJECT_NAME": "\"HALDLS\"",
            "OUTPUT_DIRECTORY": "%s/build/haldls/haldls/doc" % get_toplevel_path(),
            "PREDEFINED": "GENPYBIND()= GENPYBIND_MANUAL()= GENPYBIND_TAG_HALDLS_VX=",
            "WARN_LOGFILE": join(get_toplevel_path(), "build/haldls/haldls_doxygen_warnings.log"),
            "INCLUDE_PATH": join(get_toplevel_path(), "haldls", "include")
        },
    )

    bld(
        target = 'doxygen_stadls',
        doxy_inputs = 'include/stadls',
        features = 'doxygen',
        doxyfile = bld.root.make_node('%s/code-format/doxyfile' % get_toplevel_path()),
        install_path = 'doc/stadls',
        pars = {
            "PROJECT_NAME": "\"STADLS\"",
            "OUTPUT_DIRECTORY": "%s/build/haldls/stadls/doc" % get_toplevel_path(),
            "PREDEFINED": "GENPYBIND()= GENPYBIND_MANUAL()= "
                          + "GENPYBIND_TAG_STADLS_VX= "
                          + "GENPYBIND_TAG_HALDLS_VX=",
            "WARN_LOGFILE": join(get_toplevel_path(), "build/haldls/stadls_doxygen_warnings.log"),
            "INCLUDE_PATH": join(get_toplevel_path(), "haldls", "include")
        },
    )

    bld(
        target = 'doxygen_lola',
        doxy_inputs = 'include/lola',
        features = 'doxygen',
        doxyfile = bld.root.make_node('%s/code-format/doxyfile' % get_toplevel_path()),
        install_path = 'doc/lola',
        pars = {
            "PROJECT_NAME": "\"LOLA\"",
            "OUTPUT_DIRECTORY": "%s/build/haldls/lola/doc" % get_toplevel_path(),
            "PREDEFINED": "GENPYBIND()= GENPYBIND_MANUAL()= "
                          + "GENPYBIND_TAG_LOLA_VX= "
                          + "GENPYBIND_TAG_HALDLS_VX=",
            "WARN_LOGFILE": join(get_toplevel_path(), "build/haldls/lola_doxygen_warnings.log"),
            "INCLUDE_PATH": join(get_toplevel_path(), "haldls", "include")
        },
    )
