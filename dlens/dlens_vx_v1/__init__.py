import pylogging as logger
import dlens_vx_v1.hal
import dlens_vx_v1.halco
import dlens_vx_v1.lola
import dlens_vx_v1.sta
import dlens_vx_v1.hxcomm

if logger.get_root().get_number_of_appenders() == 0:
    logger.reset()
    logger.default_config(level=logger.LogLevel.WARN)
