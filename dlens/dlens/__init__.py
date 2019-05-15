import pylogging as logger

logger.reset()
logger.default_config(level=logger.LogLevel.DEBUG)

try:
    import dlens.v2
except ImportError:
    logger.get("dlens").warn("Warning: v2 module not available")
