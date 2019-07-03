#!/usr/bin/env python

import unittest


class TestPylolaVX(unittest.TestCase):
    def test_import(self):
        import pyhalco_common
        import pyhalco_hicann_dls_vx
        import pylola_vx

        self.assertIsNotNone(pyhalco_common)
        self.assertIsNotNone(pyhalco_hicann_dls_vx)
        self.assertIsNotNone(pylola_vx)


if __name__ == "__main__":
    unittest.main()
