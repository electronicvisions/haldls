# Hardware Abstraction Layer (and STAteful encapsulation) for the HICANN-DLS

The software in this repository has been developed by staff and students
of Heidelberg University as part of the research carried out by the
Electronic Vision(s) group at the Kirchhoff-Institute for Physics.
The research is funded by Heidelberg University, the State of
Baden-Württemberg, the European Union Sixth Framework Programme no.
15879 (FACETS), the Seventh Framework Programme under grant agreements
no 604102 (HBP), 269921 (BrainScaleS), 243914 (Brain-i-Nets), the
Horizon 2020 Framework Programme under grant agreement 720270 (HBP) as
well as from the Manfred Stärk Foundation.

This repository contains

* HALDLS -- the data structures for configuring the hardware
* STADLS -- code for experiment control and experiment encapsulation

To build it:
* prepare a fresh workspace: `mkdir workspace && cd workspace`
* fetch a current copy of the waf build tool:
  `git clone https://github.com/electronicvisions/waf -b symwaf2ic symwaf2ic && cd symwaf2ic && make && cd .. && ln -s symwaf2ic/waf`
* clone the necessary repos
  `./waf setup --repo-db-url=https://github.com/electronicvisions/projects --project=haldls` (`--clone-depth=1` to skip history)
* to build:
  `singularity exec --app visionary-defaults /path/to/container ./waf configure build`
  (alternatively use `visionary-defaults-dls`)
