#!/usr/bin/env python
import unittest
from dlens_vx_v3 import hxcomm, sta


class TestDlensHealthCheckVXV3(unittest.TestCase):
    @classmethod
    def test_digital_init(cls):
        with hxcomm.ManagedConnection() as connection:
            builder, _ = sta.generate(sta.SystemInit(
                connection.get_hwdb_entry()[0]))
            program = builder.done()
            sta.run(connection, program)


if __name__ == "__main__":
    unittest.main()
