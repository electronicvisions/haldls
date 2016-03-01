#!/usr/bin/env python

"""
Read the cube-io MSP430 firmware version via USB
"""


import pyhid
import pyhid_cube


def main():
    hid = pyhid.pyhidaccess()
    hid.openHID(vid=0x0451, pid=0x4253)
    cube = pyhid_cube.pyhid_cube(hid)

    fw = cube.readFirmwareVersion()
    print "firmware", ".".join(map(str,fw))
    cube.closeDevice()

if __name__ == "__main__":
    main()
