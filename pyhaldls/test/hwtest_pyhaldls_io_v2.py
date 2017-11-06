import unittest
import pyhalco_common as Co
import pyhalco_hicann_dls_v2 as C
import pyhaldls_container_v2 as Ct
import pyhaldls_io_v2 as IO


class TestPyhaldlsIOV2Hardware(unittest.TestCase):
    def test_playback(self):
        test_board, = IO.available_board_usb_serial_numbers()

        capmem_config = Ct.CapMem()
        for cell in C.iter_all(C.CapMemCellOnDLS):
            capmem_config.set(cell, Ct.CapMemCell.Value(cell.toEnum().value()))

        cell = C.CapMemCellOnDLS(Co.Enum(2))
        capmemvalue = Ct.CapMemCell.Value(334)
        capmem_config.set(
            C.CapMemCellOnDLS(Co.Enum(0)), Ct.CapMemCell.Value(123))
        capmem_config.set(
            C.CapMemCellOnDLS(Co.Enum(1)), Ct.CapMemCell.Value(321))
        capmem_config.set(cell, capmemvalue)

        builder = IO.PlaybackProgramBuilder()
        builder.set_container(Co.unique, capmem_config)
        builder.wait_until(100)
        capmem_ticket = builder.get_container(Co.unique, capmem_config)
        capmemcell_ticket = builder.get_container(cell, Ct.CapMemCell())
        builder.halt()

        program = builder.done()

        # Builder is reset (starts a new program) every time .done() is called
        empty_new_program = builder.done()
        self.assertNotEqual(program.serial_number(), empty_new_program.serial_number())

        # No data available yet
        with self.assertRaises(RuntimeError):
            capmem_copy = program.get(capmem_ticket)
        with self.assertRaises(RuntimeError):
            capmemcell_copy = program.get(capmemcell_ticket)

        capmem_ticket_ = builder.get_container(Co.unique, capmem_config)
        program_ = builder.done()

        # Using Ticket issued for a different program
        with self.assertRaises(ValueError):
            capmem_copy = program.get(capmem_ticket_)
        with self.assertRaises(ValueError):
            capmem_copy = program_.get(capmem_ticket)

        ctrl = IO.connect(test_board)
        ctrl.soft_reset()
        ctrl.run(program)

        with self.assertRaises(ValueError):
            capmem_copy = program.get(capmem_ticket_)

        capmem_copy = program.get(capmem_ticket)
        capmemcell_copy = program.get(capmemcell_ticket)

        self.assertEqual(capmem_config, capmem_copy)
        self.assertEqual(capmemvalue, capmemcell_copy.get_value())


if __name__ == "__main__":
    unittest.main()
