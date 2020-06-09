#!/usr/bin/env python
import unittest
from dlens_vx import hxcomm, sta


class TestDlensHealthCheckVX(unittest.TestCase):
    @classmethod
    def test_digital_init(cls):
        with hxcomm.ManagedConnection() as connection:
            builder, _ = sta.generate(sta.DigitalInit())
            program = builder.done()
            sta.run(connection, program)


if __name__ == "__main__":
    unittest.main()
