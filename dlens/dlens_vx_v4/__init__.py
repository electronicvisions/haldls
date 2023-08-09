import pylogging as logger
import dlens_vx_v4.hal
import dlens_vx_v4.halco
import dlens_vx_v4.lola
import dlens_vx_v4.sta
import dlens_vx_v4.hxcomm

if logger.get_root().get_number_of_appenders() == 0:
    logger.reset()
    logger.default_config(level=logger.LogLevel.WARN)
