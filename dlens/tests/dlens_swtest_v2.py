import unittest
from dlens.v2 import halco, hal, sta


class TestDlensV2(unittest.TestCase):
    def test_import(self):
        builder = hal.PlaybackProgramBuilder()
        self.assertIsNotNone(builder)

        capmem_on_dls = halco.CapMemOnDLS()
        self.assertIsNotNone(capmem_on_dls)

        enum = halco.Enum(2)
        self.assertIsNotNone(enum)

        ctrl = sta.ExperimentControl
        self.assertIsNotNone(ctrl)


if __name__ == "__main__":
    unittest.main()
