#!/usr/bin/env python
import unittest
from dlens_vx_v4 import sta, hxcomm


class TestDlensVXV4(unittest.TestCase):
    @classmethod
    def test_connection(cls):
        with hxcomm.ManagedConnection() as connection:
            sta.run(connection, sta.PlaybackProgramBuilder().done())


if __name__ == "__main__":
    unittest.main()
