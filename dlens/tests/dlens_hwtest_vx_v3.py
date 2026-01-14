#!/usr/bin/env python
import unittest
from dlens_vx_v3 import sta, hxcomm


class TestDlensVXV3(unittest.TestCase):
    @classmethod
    def test_connection(cls):
        with hxcomm.ManagedConnection() as connection:
            program = sta.PlaybackProgramBuilder().done()
            sta.run(connection, [program])


if __name__ == "__main__":
    unittest.main()
