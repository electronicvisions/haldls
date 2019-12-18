from abc import ABCMeta, abstractmethod
from pystadls_vx import PlaybackProgramExecutor


class ExecutorConnection(metaclass=ABCMeta):
    """
    Abstract base of context-managers for a PlaybackProgramExecutor with
    connection to hardware, simulator etc.
    """

    def __init__(self):
        self.executor = PlaybackProgramExecutor()

    @abstractmethod
    def _connect(self):
        """
        Establish a connection between :py:attr:`ExecutorConnection.executor`
        and the respective client (hardware, simulator, ...)
        """
        raise NotImplementedError

    def __enter__(self) -> PlaybackProgramExecutor:
        self._connect()
        return self.executor

    def __exit__(self, *args):
        self.executor.disconnect()


class AutoConnection(ExecutorConnection):
    """
    Context-manager for a PlaybackProgramExecutor that connects to hardware
    or simulation based on environment variables.
    """

    def _connect(self):
        self.executor.connect()


class HardwareConnection(ExecutorConnection):
    """
    Context-manager for a PlaybackProgramExecutor with connection to hardware.
    """

    def __init__(self, ip_address: str = None):
        """
        Construct a :py:class:`HardwareConnection` based on the FPGA's IP
        address. If none is given, the connection will try to obtain
        the respective value from the environment.

        :param ip_address (optional) IP address of FPGA
        """
        super().__init__()
        self.ip_address = ip_address  # type: str

    def _connect(self):
        if self.ip_address is None:
            self.executor.connect_hardware()
        else:
            self.executor.connect_hardware(self.ip_address)


class SimulatorConnection(ExecutorConnection):
    """
    Context-manager for a PlaybackProgramExecutor with connection to simulator.
    """

    def __init__(self, ip_address: str = None, port: int = None):
        """
        Construct a :py:class:`SimulatorConnection` based on the simulator's IP
        address and port. If neither is given, the connection will try to
        obtain the respective values from the environment.

        :param ip_address (optional) IP address of host running simulation
        :param port (optional) Port for RCF connection to simulation
        """
        super().__init__()
        self.ip_address = ip_address  # type: str
        self.port = port  # type: int

    def _connect(self):
        if self.ip_address is None and self.port is None:
            self.executor.connect_simulator()
        else:
            self.executor.connect_simulator(self.ip_address, self.port)
