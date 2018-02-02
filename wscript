#!/usr/bin/env python
from subprocess import check_output, CalledProcessError

def depends(ctx):
    ctx('bitter')
    ctx('logger')
    ctx('halco')
    ctx('uni')
    ctx('flyspi-rw_api')

def options(opt):
    opt.load('compiler_cxx')
    opt.load('gtest')


def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.load('gtest')

    cfg.env.CXXFLAGS_HALDLS_LIBRARIES = [
        '-fvisibility=hidden',
        '-fvisibility-inlines-hidden',
    ]
    cfg.env.LINKFLAGS_HALDLS_LIBRARIES = [
        '-fvisibility=hidden',
        '-fvisibility-inlines-hidden',
    ]

    if cfg.env.build_python_bindings:
        cfg.recurse("pyhaldls")
        cfg.recurse("pystadls")


def build(bld):
    bld(
        target = 'haldls_inc',
        export_includes = 'include',
    )

    bld.shlib(
        target = 'haldls_common',
        source = bld.path.ant_glob('src/common/*.cpp') + bld.path.ant_glob('src/exception/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_inc'],
        cxxflgas = ['-Werror'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld.shlib(
        target = 'haldls_container_v2',
        source = bld.path.ant_glob('src/container/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_common', 'bitter', 'uni', 'halco_hicann_dls_v2_inc', 'halco_hicann_dls_v2'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld.shlib(
        target = 'stadls_v2',
        source = bld.path.ant_glob('src/stadls/v2/*.cpp'),
        install_path = '${PREFIX}/lib',
        use = ['haldls_common', 'haldls_container_v2', 'flyspi-rw_api'],
        uselib = 'HALDLS_LIBRARIES',
    )

    bld(
        target = 'haldls_test_common',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/common/test-*.cpp'),
        use = ['haldls_common', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'haldls_test_container_v2',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/container/v2/test-*.cpp'),
        use = ['haldls_container_v2', 'GTEST'],
        install_path = '${PREFIX}/bin',
    )

    bld(
        target = 'stadls_hwtest_v2',
        features = 'gtest cxx cxxprogram',
        source = bld.path.ant_glob('tests/stadls/v2/hwtest-*.cpp'),
        test_main = 'tests/test_with_logger.cpp',
        use = ['haldls_container_v2', 'stadls_v2', 'logger_obj', 'GTEST'],
        install_path = '${PREFIX}/bin',
        skip_run = True,
    )

    if bld.env.build_python_bindings:
        bld.recurse("pyhaldls")
        bld.recurse("pystadls")
