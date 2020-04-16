#!/usr/bin/env python
import unittest
from dlens_vx import sta


class TestDlensVX(unittest.TestCase):
    def test_connection(self):
        with sta.AutoConnection() as connection:
            self.assertIsInstance(connection, sta.PlaybackProgramExecutor)
            connection.run(sta.PlaybackProgramBuilder().done())


if __name__ == "__main__":
    unittest.main()
