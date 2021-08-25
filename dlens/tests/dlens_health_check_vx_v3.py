#!/usr/bin/env python
import unittest
from dlens_vx_v3 import hxcomm, sta


class TestDlensHealthCheckVXV3(unittest.TestCase):
    @classmethod
    def test_digital_init(cls):
        with hxcomm.ManagedConnection() as connection:
            builder, _ = sta.generate(sta.DigitalInit())
            program = builder.done()
            sta.run(connection, program)


if __name__ == "__main__":
    unittest.main()
