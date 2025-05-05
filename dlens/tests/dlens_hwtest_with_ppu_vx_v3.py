import os
from os.path import join
import unittest
from typing import ClassVar

from dlens_vx_v3 import halco, hxcomm, hal, sta
from dlens_vx_v3.tools.run_ppu_program import load_and_start_program, \
    wait_until_ppu_finished, stop_program

_THIS_DIR = os.path.dirname(os.path.realpath(__file__))
TEST_BINARY_PATH = os.environ.get("TEST_BINARY_PATH",
                                  join(_THIS_DIR,
                                       os.pardir,
                                       os.pardir,
                                       os.pardir,
                                       "build",
                                       "haldls",
                                       "dlens")
                                  )


class CompareHostAndPPUWrite(unittest.TestCase):
    '''
    Check that a write performed with haldls on PPU sets the correct values.

    This test should ensure that the bit-order is handled correctly on PPU and
    in host code. The test procedure is as follows:
    - (host) write a known CommonNeuronBackendConfig
    - (ppu) read CommonNeuronBackendConfig, change single value, write
      CommonNeuronBackendConfig
    - (host) read CommonNeuronBackendConfig
    - (host) ensure single value has new value.
    '''
    MANAGED_CONNECTION = hxcomm.ManagedConnection()
    CONNECTION = None
    CHIP_REVISION: ClassVar[int]

    @classmethod
    def setUpClass(cls) -> None:
        cls.CONNECTION = cls.MANAGED_CONNECTION.__enter__()  # pylint: disable=unnecessary-dunder-call
        # Initialize the chip and find chip version
        init_builder, _ = sta.generate(sta.DigitalInit(
            cls.CONNECTION.get_hwdb_entry()))
        jtag_id_ticket = init_builder.read(halco.JTAGIdCodeOnDLS())
        init_builder.block_until(halco.BarrierOnFPGA(), hal.Barrier.jtag)
        sta.run(cls.CONNECTION, init_builder.done())
        jtag_id = jtag_id_ticket.get()
        cls.CHIP_REVISION = int(jtag_id.version)

    @classmethod
    def tearDownClass(cls) -> None:
        cls.MANAGED_CONNECTION.__exit__()

    def run_ppu_program(self, ppu: halco.PPUOnDLS, program_path: str,
                        timeout: int):
        load_and_start_program(self.CONNECTION, program_path, ppu)
        wait_until_ppu_finished(self.CONNECTION, timeout=timeout, ppu=ppu)
        ret_code = stop_program(self.CONNECTION, ppu=ppu, print_mailbox=False)

        self.assertEqual(0, ret_code,
                         f"PPU exit code was {ret_code}, expected 0.")

    def test_neuron_reset(self):
        enable_clocks = True
        builder = sta.PlaybackProgramBuilder()
        config = hal.CommonNeuronBackendConfig()
        config.enable_clocks = enable_clocks
        builder.write(halco.CommonNeuronBackendConfigOnDLS(), config)

        sta.run(self.CONNECTION, builder.done())

        # PPU program should flip value enable_fire
        program = join(TEST_BINARY_PATH,
                       f"test_ppu_write_vx_v{self.CHIP_REVISION}.bin")
        self.run_ppu_program(halco.PPUOnDLS(), program, int(5e5))

        builder = sta.PlaybackProgramBuilder()
        ticket = builder.read(halco.CommonNeuronBackendConfigOnDLS())
        builder.write(halco.TimerOnDLS(), hal.Timer(0))
        builder.block_until(halco.TimerOnDLS(), hal.Timer.Value(10000))
        sta.run(self.CONNECTION, builder.done())

        self.assertEqual(ticket.get().enable_clocks, not enable_clocks)


if __name__ == '__main__':
    unittest.main()
