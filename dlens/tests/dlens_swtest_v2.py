import unittest
from dlens.v2 import halco, hal, sta


class TestDlensV2(unittest.TestCase):
    def test_import(self):
        builder = hal.PlaybackProgramBuilder()
        capmem_on_dls = halco.CapMemOnDLS()
        enum = halco.Enum(2)
        ctrl = sta.ExperimentControl


if __name__ == "__main__":
    unittest.main()
