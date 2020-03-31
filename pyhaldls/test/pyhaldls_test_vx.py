#!/usr/bin/env python

import pickle
import unittest
import pyhaldls_vx as hal


class TestPyhaldlsVX(unittest.TestCase):
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

    def test_pickle(self):
        for container in hal.containers:
            obj = container()
            dump = pickle.dumps(obj)
            self.assertEqual(obj, pickle.loads(dump))


if __name__ == "__main__":
    unittest.main()
