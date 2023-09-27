.. _stadls:

stadls
======

This layer provides an representation for hardware runtime control.
It makes use of {ref}`fisch <fisch>` and {ref}`hxcomm <hxcomm>`.

Rationale of (low-level) experiment execution
---------------------------------------------

The basic design goal of Playback programs is that we can asynchronously create, run and evaluate these programs.
A Program typically represents a "atomic" part of an experiment to be executed on hardware.
Motivation: pipelining of programs, experiment streams, etc.
Due to the asynchronous design errors can be only detected after the program has run and the result data is available.

Contents of a playback program are "instructions" that can come from 3 categories:
* `read`s
* `write`s
* *barriers*

For read and write it's clear what the FPGA performs.
Spike events are implemented as writes.
From the software perspective the combination of asynchronous execution and result data in `read`s is interesting.
We implement a mechanism which is similar to [promises](https://en.cppreference.com/w/cpp/thread/promise).
The program itself manages memory for the future result of an instruction.
The program execution on the FPGA progresses FPGA time.

On the other hand, barriers "block" any further program execution until a condition is met:
* either a target time has been reached (by waiting),
* or a timed wait interval has been completed,
* or a read result shows an expected value after polling,
* or a communication channel is idle (e.g. omnibus, jtag, …).

The latter can be used to verify that something has happend, i.e. a certain value `X` has been reached at some register location.

For everything there are FPGA-handled timeouts that are generated during execution of the program.
The software can evaluate the occurence of such timeout events after the program has been run.
In addition to such events, in such *trace* programs for each `read` in the playback program, a matching result value is stored.
