#!/usr/bin/env python
import unittest
from dlens_vx import sta, hxcomm


class TestDlensVX(unittest.TestCase):
    @classmethod
    def test_connection(cls):
        with hxcomm.ManagedConnection() as connection:
            sta.run(connection, sta.PlaybackProgramBuilder().done())


if __name__ == "__main__":
    unittest.main()
