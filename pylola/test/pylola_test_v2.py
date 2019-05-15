#!/usr/bin/env python

import unittest


class TestPylolaV2(unittest.TestCase):
    def test_import(self):
        import pyhalco_common
        import pyhalco_hicann_dls_v2
        import pylola_v2

        self.assertIsNotNone(pyhalco_common)
        self.assertIsNotNone(pyhalco_hicann_dls_v2)
        self.assertIsNotNone(pylola_v2)


if __name__ == "__main__":
    unittest.main()
