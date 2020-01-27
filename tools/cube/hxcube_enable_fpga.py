#!/usr/bin/env python

"""Turn on selected FPGA on Cube Setup via MSP430 firmware provided by TUD.

Communicates via USB HID, using pyhid_cube (TUD) -> pyhid (TUD) -> libusb.
"""


import os
import argparse
import time
import pyhid
import pyhid_cube


# c.f. https://brainscales-r.kip.uni-heidelberg.de/projects/symap2ic/wiki/hx-cube-setup-inventory#CUBE-IO-Boards-v-2018  # pylint: disable=line-too-long
LICENSE_SERIAL_LUT = {
    "cubex0": "AFE25B4712002000",
    "cubex2": "AFE25B4713002000",
    "cubex3": "AFE25B471D002000",
    "cubex4": "AFE25B4717002000",
    "cubex5": "AFE25B4714002000",
    "cubex6": "AFE25B4724002000",
    "cubex7": "AFE25B4727002000",
    "cubex8": "AFE25B4710002000",
    "cubex9": "AFE25B472A002000",
    "cubex10": "AFE25B472D002000",
    "cubex11": "AFE25B4716002000",
    "cubex12": "AFE25B4720002000",
    "cubex13": "AFE25B471E002000",
    "cubex14": "AFE25B471A002000",
    "cubex15": "AFE25B4722002000",
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("fpga", type=int)
    parser.add_argument("--poweroff", dest='enable', action='store_false')
    parser.add_argument("--skip-bootloader", dest='skip', action='store_true')
    parser.add_argument("--desktop", dest='ignore_license',
                        action='store_true',
                        help=("Ignore SLURM license to select correct setup."
                              "Intended for desktop use without SLURM."))
    args = parser.parse_args()
    if not args.ignore_license:
        if "MY_SLURM_GRES" not in os.environ:
            raise KeyError(("No gres specified. For desktop use,"
                            "please run with --desktop"))
        gres = os.environ["MY_SLURM_GRES"]

    hid = pyhid.pyhidaccess()  # pylint: disable=no-member
    if args.ignore_license:
        # will connect to the first available match
        hid.openHID(vid=0x0451, pid=0x4253)
    else:
        if gres not in LICENSE_SERIAL_LUT:
            raise KeyError("No serial number found for specified gres {}."
                           .format(gres))
        serial = LICENSE_SERIAL_LUT[gres]
        hid.openHID(vid=0x0451, pid=0x4253, serial=serial)
    cube = pyhid_cube.pyhid_cube(hid)
    cube.powerControlPMIC(args.fpga, args.enable)

    if args.enable and args.skip:
        # wait until init is done
        time.sleep(2)
        cube.rebootFPGA(args.fpga, 1)

    cube.closeDevice()


if __name__ == "__main__":
    main()
