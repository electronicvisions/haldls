#!/usr/bin/env python
import unittest
from dlens_vx_v3 import halco, hal, sta, lola


class TestDlensVXV3(unittest.TestCase):
    def test_import(self):
        capmem_cell = hal.CapMemCell()
        self.assertIsNotNone(capmem_cell)

        builder = sta.PlaybackProgramBuilder()
        self.assertIsNotNone(builder)

        capmem_on_dls = halco.CapMemBlockOnDLS()
        self.assertIsNotNone(capmem_on_dls)

        enum = halco.common.Enum(2)
        self.assertIsNotNone(enum)

        row = lola.SynapseRow()
        self.assertIsNotNone(row)


if __name__ == "__main__":
    unittest.main()
