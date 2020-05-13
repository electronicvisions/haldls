#!/usr/bin/env python

import pickle
import unittest
import pyhaldls_vx_v1 as hal


class TestPyhaldlsVXV1(unittest.TestCase):
    def test_capmem_cell(self):
        cell = hal.CapMemCell()
        self.assertEqual(cell.value, cell.Value())

        cell.value = cell.Value(25)
        self.assertEqual(cell.value, cell.Value(25))

        cell.value = 123
        self.assertEqual(cell.value, cell.Value(123))

        with self.assertRaises(TypeError):
            cell.value = 1023

        cell.value = cell.DisableRefresh()
        self.assertEqual(cell.value, cell.DisableRefresh())
        self.assertEqual(cell.value, cell.DisableRefresh(1023))

    def test_crossbar_output_config(self):
        config = hal.CrossbarOutputConfig()
        config.enable_event_counter[3] = True  # pylint: disable=unsupported-assignment-operation
        config.enable_slow[2] = True  # pylint: disable=unsupported-assignment-operation
        self.assertEqual(config.enable_event_counter[3], True)  # pylint: disable=unsubscriptable-object
        self.assertEqual(config.enable_slow[2], True)  # pylint: disable=unsubscriptable-object

    def test_pickle(self):
        for container in hal.containers:
            obj = container()
            dump = pickle.dumps(obj)
            self.assertEqual(obj, pickle.loads(dump))

    def test_to_json(self):
        for cont in hal.containers:
            container = cont()
            container2 = cont()
            ser = hal.to_json(container)
            hal.from_json(container, ser)
            self.assertEqual(container, container2)


if __name__ == "__main__":
    unittest.main()
