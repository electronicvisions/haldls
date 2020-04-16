#!/usr/bin/env python
import argparse
import os
from os.path import join
from waflib.extras.symwaf2ic import get_toplevel_path


def depends(ctx):
    ctx('code-format')
    ctx('logger')
    ctx('halco')
    ctx('hate')
    ctx('fisch')

    if getattr(ctx.options, 'with_haldls_python_bindings', True):
        ctx('haldls', 'pyhaldls')
        ctx('haldls', 'pystadls')
        ctx('haldls', 'pylola')
        ctx('haldls', 'dlens')


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


def build(bld):
    bld.env.DLSvx_HARDWARE_AVAILABLE = "cube" == os.environ.get("SLURM_JOB_PARTITION")
    bld.env.DLSvx_SIM_AVAILABLE = "FLANGE_SIMULATION_RCF_PORT" in os.environ

    if bld.options.disable_coverage_reduction:
        bld.env.SIMTEST_TIMEOUT_SECONDS = 50 * 3600
        bld.env.REDUCED_SIMTESTS_DEFINES = ["REDUCED_TESTS=0",
                                            "MAX_WORDS_PER_REDUCED_TEST=10"]
    else:
        bld.env.SIMTEST_TIMEOUT_SECONDS = 1 * 3600
        bld.env.REDUCED_SIMTESTS_DEFINES = ["REDUCED_TESTS=1",
                                            "MAX_WORDS_PER_REDUCED_TEST=10"]

    bld(
        target = 'haldls_inc',
        export_includes = 'include',
    )

    bld(
        target = 'haldls_vx',
        source = bld.path.ant_glob('src/haldls/vx/*.cpp'),
        install_path = '${PREFIX}/lib',
        features = 'cxx cxxshlib pyembed',
        use = ['haldls_inc', 'halco_hicann_dls_vx', 'fisch_vx'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld(
        target = 'stadls_vx',
        source = bld.path.ant_glob('src/stadls/vx/*.cpp'),
        install_path = '${PREFIX}/lib',
        features = 'cxx cxxshlib pyembed',
        use = ['haldls_vx', 'lola_vx'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld(
        features = 'cxx cxxshlib pyembed',
        target = 'lola_vx',
        source = bld.path.ant_glob('src/lola/vx/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_vx', 'ELF'],
        uselib = 'LOLA_LIBRARIES',
    )

    bld(
        target = 'stadls_swtest_vx',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/stadls/vx/test-*.cpp'),
        use = ['haldls_vx', 'stadls_vx', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'haldls_test_common_inc',
        export_includes = 'tests/common',
    )

    bld(
        target = 'haldls_swtest_vx',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/haldls/vx/test-*.cpp'),
        use = ['haldls_vx', 'haldls_test_common_inc', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'lola_swtest_vx',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/sw/lola/vx/test-*.cpp'),
        use = ['lola_vx', 'GTEST', 'haldls_test_common_inc'],
        install_path = '${PREFIX}/bin',
        defines = ['TEST_PPU_PROGRAM="' + join(get_toplevel_path(), 'haldls', 'tests', 'sw', 'lola', 'lola_ppu_test_elf_file.bin') + '"'],
    )

    bld(
        target = 'stadls_hwtest_vx_inc',
        export_includes = 'tests/hw/stadls/vx/executor_hw/',
    )

    bld(
        target = 'stadls_simtest_vx_inc',
        export_includes = 'tests/hw/stadls/vx/executor_sim/',
    )

    bld(
        target = 'stadls_hwtest_vx',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/hw/stadls/vx/hw/test-*.cpp')
                 + bld.path.ant_glob('tests/hw/stadls/vx/common/test-*.cpp'),
        test_main = 'tests/hw/stadls/vx/main.cpp',
        use = ['haldls_vx', 'stadls_vx', 'GTEST', 'stadls_hwtest_vx_inc', 'haldls_test_common_inc'],
        defines = ['REDUCED_TESTS=0', 'MAX_WORDS_PER_REDUCED_TEST=10'],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
        skip_run = not bld.env.DLSvx_HARDWARE_AVAILABLE
    )

    bld(
        target = 'stadls_simtest_vx',
        features = 'gtest cxx cxxprogram pyembed',
        source = bld.path.ant_glob('tests/hw/stadls/vx/sim/test-*.cpp')
                 + bld.path.ant_glob('tests/hw/stadls/vx/common/test-*.cpp'),
        test_main = 'tests/hw/stadls/vx/main.cpp',
        use = ['haldls_vx', 'GTEST', 'stadls_simtest_vx_inc', 'stadls_vx', 'haldls_test_common_inc'],
        defines = bld.env.REDUCED_SIMTESTS_DEFINES + ["SIMULATION_TEST=1"],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
        skip_run = not bld.env.DLSvx_SIM_AVAILABLE,
        test_timeout = bld.env.SIMTEST_TIMEOUT_SECONDS
    )

    bld(
        target = 'run_ppu_program_vx',
        features = 'cxx cxxprogram pyembed',
        source = 'tools/stadls/vx/run_ppu_program.cpp',
        use = ['haldls_vx', 'logger_obj', 'stadls_vx'],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
    )

    if bld.env.build_with_haldls_python_bindings:
        bld.recurse("pyhaldls")
        bld.recurse("pystadls")
        bld.recurse("pylola")
        bld.recurse("dlens")

    bld(
        features = 'doxygen',
        doxyfile = bld.root.make_node('%s/code-format/doxyfile' % get_toplevel_path()),
        install_path = 'doc/haldls',
        pars = {
            "PROJECT_NAME": "\"HALDLS\"",
            "INPUT": "%s/haldls/include/haldls" % get_toplevel_path(),
            "OUTPUT_DIRECTORY": "%s/build/haldls/haldls/doc" % get_toplevel_path(),
            "PREDEFINED": "GENPYBIND()= GENPYBIND_MANUAL()= GENPYBIND_TAG_HALDLS_VX=",
            "WARN_LOGFILE": join(get_toplevel_path(), "build/haldls/haldls_doxygen_warnings.log"),
            "INCLUDE_PATH": join(get_toplevel_path(), "haldls", "include")
        },
    )

    bld(
        features = 'doxygen',
        doxyfile = bld.root.make_node('%s/code-format/doxyfile' % get_toplevel_path()),
        install_path = 'doc/stadls',
        pars = {
            "PROJECT_NAME": "\"STADLS\"",
            "INPUT": "%s/haldls/include/stadls" % get_toplevel_path(),
            "OUTPUT_DIRECTORY": "%s/build/haldls/stadls/doc" % get_toplevel_path(),
            "PREDEFINED": "GENPYBIND()= GENPYBIND_MANUAL()= "
                          + "GENPYBIND_TAG_STADLS_VX= "
                          + "GENPYBIND_TAG_HALDLS_VX=",
            "WARN_LOGFILE": join(get_toplevel_path(), "build/haldls/stadls_doxygen_warnings.log"),
            "INCLUDE_PATH": join(get_toplevel_path(), "haldls", "include")
        },
    )

    bld(
        features = 'doxygen',
        doxyfile = bld.root.make_node('%s/code-format/doxyfile' % get_toplevel_path()),
        install_path = 'doc/lola',
        pars = {
            "PROJECT_NAME": "\"LOLA\"",
            "INPUT": "%s/haldls/include/lola" % get_toplevel_path(),
            "OUTPUT_DIRECTORY": "%s/build/haldls/lola/doc" % get_toplevel_path(),
            "PREDEFINED": "GENPYBIND()= GENPYBIND_MANUAL()= "
                          + "GENPYBIND_TAG_LOLA_VX= "
                          + "GENPYBIND_TAG_HALDLS_VX=",
            "WARN_LOGFILE": join(get_toplevel_path(), "build/haldls/lola_doxygen_warnings.log"),
            "INCLUDE_PATH": join(get_toplevel_path(), "haldls", "include")
        },
    )
