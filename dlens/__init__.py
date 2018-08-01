import pylogging as logger

logger.reset()
logger.default_config(level=logger.LogLevel.DEBUG)

try:
    import v2
except ImportError:
    logger.warn("Warning: v2 module not available")
