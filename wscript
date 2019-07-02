#!/usr/bin/env python
import os
from os.path import join
from waflib.extras.symwaf2ic import get_toplevel_path


def depends(ctx):
    ctx('code-format')
    ctx('bitter')
    ctx('logger')
    ctx('halco')
    ctx('hate')
    ctx('uni')
    ctx('lib-rcf')
    ctx('flyspi-rw_api')
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
    hopts.add_withoption('munge', default=True,
        help='Toggle build of quiggeldy with munge-based '
             'authentification support')
    hopts.add_withoption('haldls-python-bindings', default=True,
            help='Toggle the generation and build of haldls python bindings')


def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.load('gtest')

    cfg.env.build_with_munge = cfg.options.with_munge
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

    cfg.check_boost(lib='program_options system',
            uselib_store='BOOST4TOOLS')

    cfg.check_cxx(lib="dl", mandatory=True, uselib_store="DL4TOOLS")
    cfg.check_cxx(lib="pthread", uselib_store="PTHREAD")

    if cfg.env.build_with_munge:
        cfg.check_cxx(lib="munge",
                      header_name="munge.h",
                      msg="Checking for munge",
                      uselib_store="MUNGE")
        cfg.env.DEFINES_MUNGE = ["USE_MUNGE_AUTH"]

    if cfg.env.build_with_haldls_python_bindings:
        cfg.recurse("pyhaldls")
        cfg.recurse("pystadls")
        cfg.recurse("pylola")
        cfg.recurse("dlens")


def build(bld):
    bld.env.DLSv2_HARDWARE_AVAILABLE = "dls" == os.environ.get("SLURM_JOB_PARTITION")
    bld.env.DLSvx_HARDWARE_AVAILABLE = "cube" == os.environ.get("SLURM_JOB_PARTITION")

    bld(
        target = 'lola_inc',
        export_includes = 'include',
    )

    bld(
        target = 'haldls_inc',
        export_includes = 'include',
    )

    use_quiggeldy = ['rcf-sf-only', 'rcf_extensions']
    if bld.env.build_with_munge:
        use_quiggeldy.append("MUNGE")

    bld.shlib(
        target = 'dls_common',
        source = bld.path.ant_glob('src/common/*.cpp') + bld.path.ant_glob('src/exception/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_inc', 'hate_inc'],
        cxxflgas = ['-Werror'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld.shlib(
        target = 'haldls_v2',
        source = bld.path.ant_glob('src/haldls/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['dls_common', 'bitter', 'uni', 'halco_hicann_dls_v2_inc', 'halco_hicann_dls_v2'],
        uselib = 'HALDLS_LIBRARIES',
    )
    bld.shlib(
        target = 'haldls_vx',
        source = bld.path.ant_glob('src/haldls/vx/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['dls_common', 'bitter', 'uni', 'halco_hicann_dls_vx_inc', 'halco_hicann_dls_vx', 'fisch_vx'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld.shlib(
        target = 'stadls_v2',
        source = bld.path.ant_glob('src/stadls/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['dls_common', 'haldls_v2', 'flyspi-rw_api']
              + use_quiggeldy,
        uselib = 'HALDLS_LIBRARIES',
    )

    bld(
        features = 'cxx cxxshlib',
        target = 'lola_v2',
        source = bld.path.ant_glob('src/lola/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_v2'],
        uselib = 'LOLA_LIBRARIES',
    )

    bld(
        features = 'cxx cxxshlib',
        target = 'lola_vx',
        source = bld.path.ant_glob('src/lola/vx/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_vx'],
        uselib = 'LOLA_LIBRARIES',
    )

    bld(
        target = 'quiggeldy',
        features = 'cxx cxxprogram',
        source = bld.path.ant_glob('src/tools/quiggeldy.cpp'),
        use = ['stadls_v2', 'BOOST4TOOLS', 'DL4TOOLS', 'PTHREAD', 'logger_obj']
              + use_quiggeldy,
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'dls_swtest_common',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/sw/common/test-*.cpp'),
        use = ['dls_common', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'stadls_swtest_v2',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/sw/stadls/v2/test-*.cpp'),
        use = ['haldls_v2', 'stadls_v2', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'haldls_test_common_inc',
        export_includes = 'tests/common',
    )

    bld(
        target = 'haldls_swtest_v2',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/sw/haldls/v2/test-*.cpp'),
        use = ['haldls_v2', 'haldls_test_common_inc', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'haldls_swtest_vx',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/sw/haldls/vx/test-*.cpp'),
        use = ['haldls_vx', 'haldls_test_common_inc', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'lola_swtest_v2',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/sw/lola/v2/test-*.cpp'),
        use = ['lola_v2', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'lola_swtest_vx',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/sw/lola/vx/test-*.cpp'),
        use = ['lola_vx', 'GTEST'],
        install_path = '${PREFIX}/bin',
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
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/hw/stadls/vx/test-*.cpp'),
        test_main = 'tests/hw/stadls/vx/main.cpp',
        use = ['haldls_vx', 'GTEST', 'haldls_test_common_inc', 'stadls_hwtest_vx_inc'],
        install_path = '${PREFIX}/bin',
        skip_run = not bld.env.DLSvx_HARDWARE_AVAILABLE
    )

    bld(
        target = 'stadls_simtest_vx',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/hw/stadls/vx/test-*.cpp'),
        test_main = 'tests/hw/stadls/vx/main.cpp',
        use = ['haldls_vx', 'GTEST', 'haldls_test_common_inc', 'stadls_simtest_vx_inc'],
        install_path = '${PREFIX}/bin',
        skip_run = True
    )

    bld(target = 'stadls_hwtest_v2',
        features='gtest cxx cxxprogram',
        source=bld.path.ant_glob('tests/hw/stadls/v2/test-*.cpp'),
        use=['haldls_v2', 'stadls_v2', 'logger_obj', 'GTEST', 'DL4TOOLS'],
        install_path='${PREFIX}/bin',
        test_timeout=120,
        skip_run=not bld.env.DLSv2_HARDWARE_AVAILABLE
        )

    bld(target = 'quiggeldy_hwtest_v2',
        defines = ['NO_LOCAL_BOARD'],
        features='gtest cxx cxxprogram',
        source=bld.path.ant_glob('tests/hw/stadls/v2/test-*.cpp'),
        use=['haldls_v2', 'stadls_v2', 'logger_obj', 'GTEST', 'DL4TOOLS'],
        install_path='${PREFIX}/bin',
        test_timeout=120,
        skip_run=True	# Quiggeldy cannot be tested in CI
        )

    bld(
        target = 'run_ppu_program_v2',
        features = 'cxx cxxprogram',
        source = 'tools/stadls/v2/run_ppu_program.cpp',
        use = ['haldls_v2', 'stadls_v2', 'logger_obj'],
        install_path = '${PREFIX}/bin',
        linkflags = ['-lboost_program_options-mt'],
    )

    bld(
        target = 'run_ppu_program_vx',
        features = 'cxx cxxprogram',
        source = 'tools/stadls/vx/run_ppu_program.cpp',
        use = ['haldls_vx', 'logger_obj', 'fisch_vx'],
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
        doxyfile = bld.root.make_node('%s/code-format/doxyfile' % bld.env.PREFIX),
        install_path = 'doc/haldls',
        pars = {
            "PROJECT_NAME": "\"HALDLS\"",
            "INPUT": "%s/haldls/include/haldls" % bld.env.PREFIX,
            "OUTPUT_DIRECTORY": "%s/build/haldls/haldls/doc" % bld.env.PREFIX,
            "PREDEFINED": "GENPYBIND()= GENPYBIND_MANUAL()= GENPYBIND_TAG_HALDLS_V2= GENPYBIND_TAG_HALDLS_VX=",
            "WARN_LOGFILE": join(get_toplevel_path(), "build/haldls/haldls_doxygen_warnings.log"),
            "INCLUDE_PATH": join(get_toplevel_path(), "haldls", "include")
        },
    )

    bld(
        features = 'doxygen',
        doxyfile = bld.root.make_node('%s/code-format/doxyfile' % bld.env.PREFIX),
        install_path = 'doc/stadls',
        pars = {
            "PROJECT_NAME": "\"STADLS\"",
            "INPUT": "%s/haldls/include/stadls" % bld.env.PREFIX,
            "OUTPUT_DIRECTORY": "%s/build/haldls/stadls/doc" % bld.env.PREFIX,
            "PREDEFINED": "GENPYBIND()= GENPYBIND_TAG_STADLS_V2=",
            "WARN_LOGFILE": join(get_toplevel_path(), "build/haldls/stadls_doxygen_warnings.log"),
            "INCLUDE_PATH": join(get_toplevel_path(), "haldls", "include")
        },
    )
