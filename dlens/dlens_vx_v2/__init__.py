import pylogging as logger
import dlens_vx_v2.hal
import dlens_vx_v2.halco
import dlens_vx_v2.lola
import dlens_vx_v2.sta
import dlens_vx_v2.hxcomm

if logger.get_root().get_number_of_appenders() == 0:
    logger.reset()
    logger.default_config(level=logger.LogLevel.WARN)
