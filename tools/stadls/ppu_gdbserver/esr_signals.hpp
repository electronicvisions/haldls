#pragma once
#include <map>

// These values are taken from binutils-gdb 2.25 and might
// differ in more recent versions of this package.
#define GDB_SIGNAL_BUS 10
#define GDB_SIGNAL_TRAP 5
#define GDB_SIGNAL_ILL 4
#define GDB_SIGNAL_FPE 8
#define GDB_SIGNAL_USR1 30
#define GDB_SIGNAL_USR2 31
#define GDB_SIGNAL_UNKNOWN 143

// Table to translate ESR to signal # as defined in includes/gdb/signals.h
// TODO: confirm exceptions, map unknown signals to ones in signals.h
static std::map<int, int> esr_table = {
    {0x0000002, GDB_SIGNAL_BUS},     // Misaligned instruction
    {0x0000020, GDB_SIGNAL_UNKNOWN}, // VLE operation
    {0x0000040, GDB_SIGNAL_UNKNOWN}, // External process ID operation
    {0x0000080, GDB_SIGNAL_UNKNOWN}, // Signal processing operation
    {0x0000100, GDB_SIGNAL_UNKNOWN}, // Page table
    {0x0000200, GDB_SIGNAL_UNKNOWN}, // TLB ineligible
    {0x0000400, GDB_SIGNAL_UNKNOWN}, // Data access
    {0x0010000, GDB_SIGNAL_UNKNOWN}, // Imprecise exception
    {0x0020000, GDB_SIGNAL_UNKNOWN}, // Byte-ordering
    {0x0040000, GDB_SIGNAL_UNKNOWN}, // Program unimplemented operation
    {0x0080000, GDB_SIGNAL_UNKNOWN}, // Auxiliary processor operation
    {0x0100000, GDB_SIGNAL_USR2},    // Implementation dependent
    {0x0200000, GDB_SIGNAL_USR1},    // Implementation dependant
    {0x0800000, GDB_SIGNAL_UNKNOWN}, // Store operation
    {0x1000000, GDB_SIGNAL_FPE},     // Floating point
    {0x2000000, GDB_SIGNAL_TRAP},    // Trap
    {0x4000000, GDB_SIGNAL_ILL},     // Priviliged instruction
    {0x8000000, GDB_SIGNAL_ILL}      // Illegal instruction
};

// Convert ESR from PPU to gdb signal number
uint32_t esr_to_signal(uint32_t const esr)
{
	for (auto signal : esr_table)
		if (esr & signal.first)
			return signal.second;
	return 0;
}
