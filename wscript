#!/usr/bin/env python
from subprocess import check_output, CalledProcessError

def depends(ctx):
    ctx('bitter')
    ctx('logger')
    ctx('halco')
    ctx('hate')
    ctx('uni')
    ctx('lib-rcf', branch='v2')
    ctx('flyspi-rw_api')

def options(opt):
    opt.load('compiler_cxx')
    opt.load('gtest')


def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.load('gtest')
    cfg.check_cxx(mandatory=True, header_name='cereal/cereal.hpp')

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


    if cfg.env.build_python_bindings:
        cfg.recurse("pyhaldls")
        cfg.recurse("pystadls")


def build(bld):
    bld(
        target = 'haldls_inc',
        export_includes = 'include',
    )

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
        target = 'stadls_v2',
        source = bld.path.ant_glob('src/stadls/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['dls_common', 'haldls_v2', 'flyspi-rw_api', 'rcf-sf-only', 'rcf_extensions'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld(
        target = 'quiggeldy',
        features = 'cxx cxxprogram',
        source = bld.path.ant_glob('src/tools/quiggeldy.cpp'),
        use = ['stadls_v2', 'BOOST4TOOLS', 'DL4TOOLS', 'PTHREAD', 'logger_obj'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'dls_test_common',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/common/test-*.cpp'),
        use = ['dls_common', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'haldls_test_v2',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/v2/test-*.cpp'),
        use = ['haldls_v2', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    stadl_tests_kwargs = dict(
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/stadls/v2/hwtest-*.cpp'),
        test_main = 'tests/test_with_logger.cpp',
        use = ['haldls_v2', 'stadls_v2', 'logger_obj', 'GTEST', 'DL4TOOLS'],
        install_path = '${PREFIX}/bin',
        skip_run = True,
    )

    bld(target = 'stadls_hwtest_v2',
        **stadl_tests_kwargs)

    bld(target = 'quiggeldy_hwtest_v2',
        defines = 'NO_LOCAL_BOARD',
        **stadl_tests_kwargs)

    if bld.env.build_python_bindings:
        bld.recurse("pyhaldls")
        bld.recurse("pystadls")
