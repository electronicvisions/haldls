# -*- coding: utf-8 -*-
##-----------------------------------------------------------------
##
## Copyright (c) 2013 TU-Dresden  All rights reserved.
##
## Unless otherwise stated, the software on this site is distributed
## in the hope that it will be useful, but WITHOUT ANY WARRANTY;
## without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR A PARTICULAR PURPOSE. THERE IS NO WARRANTY FOR THE SOFTWARE,
## TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE
## STATED IN WRITING THE COPYRIGHT HOLDERS PROVIDE THE SOFTWARE
## "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED,
## INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
## MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE
## RISK AS TO THE QUALITY AND PERFORMANCE OF THE SOFTWARE IS WITH YOU.
## SHOULD THE SOFTWARE PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL
## NECESSARY SERVICING, REPAIR OR CORRECTION. IN NO EVENT UNLESS
## REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL ANY
## COPYRIGHT HOLDER, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
## GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT
## OF THE USE OR INABILITY TO USE THE SOFTWARE (INCLUDING BUT NOT
## LIMITED TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES
## SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF THE SOFTWARE TO
## OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER HAS BEEN
## ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
##
##-----------------------------------------------------------------

## Company           :   TU-Dresden
## Author            :   Stephan Hartmann
## E-Mail            :   hartmann@iee.et.tu-dresden.de
##
## Filename          :   pyhid_cube.py
## Project Name      :   MSP430
## Description       :   Python Function for Firmware Communication
##-----------------------------------------------------------------
## Version   |  Author    |  Date         |  Comment
## ----------------------------------------------------------------
## 0.1       | hartmann   | 19 Jun 2013   |  initial version
## ----------------------------------------------------------------
import pyhid

class pyhid_cube(object):

    def __init__(self, pyhidobj):
        if not isinstance(pyhidobj, pyhid.pyhidaccess):
            raise IOError('__init__() Need instance of PyHID class.')
        self.__pyhidobj = pyhidobj
        self.__usb_timeout = 1000
        self.__usb_response_retries = 5

    def closeDevice(self):
        self.__pyhidobj.closeHID()

    def __check_value(self, name, value, minimum=None, maximum=None):
        if type(value) != long and type(value) != int:
            raise TypeError('%s() Invalid type for %s' % name)
        if minimum is not None:
            if value < minimum:
                raise ValueError('%s() %s violates minimum value.' \
                                 % name)
        if maximum is not None:
            if value > maximum:
                raise ValueError('%s() %s violates maximum value.' \
                                 % name)

    def __check_pmic_params(self, name, ucdno, cc, page):
        self.__check_value((name, 'PMIC number'), ucdno, 0x00, 0x03)
        if page is not None:
            self.__check_value((name, 'PMIC page'), page, 0x00, 0xFE)
        self.__check_value((name, 'PMIC command code'), cc, 0x00, 0xFF)

    def __read_result(self, name, cmd, byteno=0):
        buf = self.__pyhidobj.readHID(64, self.__usb_timeout)
        length = len(buf)
        start = 2
        if byteno is None:
            byteno = 0
            start = 3
            if length < 3:
                length = 0
            else:
                byteno = buf[2]
        if length < byteno + 2:
            raise IOError('%s() Failed to read result.' % name[0])
        if buf[0] != ( cmd | 0x80 ) or buf[1] != 0x00:
            raise IOError('%s() Failed to %s.' % name)
        if byteno > 0:
            return buf[start:byteno+start]
        return None

    def readFirmwareVersion(self):
        buf = [0] * 64
        self.__pyhidobj.writeHID(buf)
        data = self.__read_result(('readFirmwareVersion', 'read firmware version'),
                                  0x00, 2)
        return (data[0], data[1])

    def powerControlPMIC(self, ucdno, powerup):
        self.__check_value(('powerControlPMIC', 'PMIC number'),
                           ucdno, 0x00, 0x03)
        if type(powerup) != bool:
            raise TypeError('powerControlPMIC() Invalid type for '
                            'power-up/power-down parameter.')
        buf = [0] * 64
        buf[0] = 0x02
        buf[1] = ucdno
        buf[2] = 0x01 if powerup else 0x00
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('powerControlPMIC', 'enable/disable PMIC'),
                           0x02, 0)

    def enableUCDStatusMon(self):
        buf = [0] * 64
        buf[0] = 0x01
        buf[1] = 0x01
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('enableUCDStatusMon', 'enable UCD status monitor'),
                           0x01)

    def disableUCDStatusMon(self):
        buf = [0] * 64
        buf[0] = 0x01
        buf[1] = 0x00
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('disableUCDStatusMon', 'disable UCD status monitor'),
                           0x01)

    def resetPMIC(self):
        buf = [0] * 64
        buf[0] = 0x03
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('resetPMIC', 'reset PMICs'),
                           0x03)

    def readBytePMIC(self, ucdno, cc, page=None):
        self.__check_pmic_params('readBytePMIC', ucdno, cc, page)
        buf = [0] * 64
        buf[0] = 0x10
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        self.__pyhidobj.writeHID(buf)
        return self.__read_result(('readBytePMIC', 'read byte from PMIC'),
                                  0x10, 1)[0]

    def writeBytePMIC(self, ucdno, cc, page=None, data=0):
        self.__check_pmic_params('writeBytePMIC', ucdno, cc, page)
        self.__check_value(('writeBytePMIC', 'data byte'),
                           data, 0x00, 0xFF)
        buf = [0] * 64
        buf[0] = 0x11
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        buf[4] = data
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('writeBytePMIC', 'write byte to PMIC'),
                           0x11, 0)

    def readWordPMIC(self, ucdno, cc, page=None):
        self.__check_pmic_params('readWordPMIC', ucdno, cc, page)
        buf = [0] * 64
        buf[0] = 0x12
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        self.__pyhidobj.writeHID(buf)
        buf = self.__read_result(('readWordPMIC', 'read word from PMIC'),
                                 0x12, 2)
        return buf[0] | ( buf[1] << 8 )

    def writeWordPMIC(self, ucdno, cc, page=None, data=0):
        self.__check_pmic_params('writeWordPMIC', ucdno, cc, page)
        self.__check_value(('writeWordPMIC', 'data word'),
                           data, 0x00, 0xFFFF)
        buf = [0] * 64
        buf[0] = 0x13
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        buf[4] = data & 0xFF
        buf[5] = ( data >> 8 ) & 0xFF
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('writeWordPMIC', 'write word to PMIC'),
                           0x13, 0)

    def readBlockPMIC(self, ucdno, cc, page=None):
        self.__check_pmic_params('readBlockPMIC', ucdno, cc, page)
        buf = [0] * 64
        buf[0] = 0x14
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        self.__pyhidobj.writeHID(buf)
        return self.__read_result(('readBlockPMIC', 'read block from PMIC'),
                                  0x14, None)

    def writeBlockPMIC(self, ucdno, cc, page=None, data=[]):
        self.__check_pmic_params('writeBlockPMIC', ucdno, cc, page)
        if type(data) != list:
            raise TypeError('writeBlockPMIC() Invalid type for '
                            'for data parameter.')
        length = len(data)
        if length == 0:
            return
        if length > 59:
            raise IOError('writeBlockPMIC() Can''t sent block with '
                          'more than 59 bytes.')
        buf = [0] * 64
        buf[0] = 0x15
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        buf[4] = len(data)
        k = 5
        for byte in data:
            self.__check_value(('writeBlockPMIC', 'data byte'),
                               byte, 0x00, 0xFF)
            buf[k] = byte
            k += 1
        self.__pyhidobj.writeHID(buf)
        return self.__read_result(('writeBlockPMIC', 'write block to PMIC'),
                                  0x15, 0)

    def sendBytePMIC(self, ucdno, cc, page=None, pec=True):
        self.__check_pmic_params('sendBytePMIC', ucdno, cc, page)
        buf = [0] * 64
        buf[0] = 0x16
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        buf[4] = 0x00 if not pec else 0x01
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('sendBytePMIC', 'send byte to PMIC'),
                           0x16, 0)

    def readPMICStatus(self, ucdno):
        self.__check_value('PMIC number', ucdno, 0, 3)
        buf = [0] * 64
        buf[0] = 0x30
        buf[1] = ucdno
        self.__pyhidobj.writeHID(buf)
        block = self.__read_result(('readPMICStatus', 'read PMIC status'),
                                    0x30, 50)
        data = [0x0] * 13
        status = [0x1] * 13
        for i in range(0, 13):
            data[i] = ( block[3 * i + 1] << 8 ) | block[3 * i + 2]
            status[i] = block[3 * i + 3]
        return (status, data)

    def readFPGAStatus(self, fpgano):
        self.__check_value('FPGA number', fpgano, 0, 3)
        buf = [0] * 64
        buf[0] = 0x31
        buf[1] = fpgano
        self.__pyhidobj.writeHID(buf)
        block = self.__read_result(('readFPGAStatus', 'read FPGA status'),
                                    0x31, 43)
        data = [0x0] * 14
        status = [0x1] * 14
        for i in range(0, 14):
            data[i] = ( block[3 * i + 1] << 8 ) | block[3 * i + 2]
            status[i] = block[3 * i + 3]
        return (status, data)

    def readInitStatus(self, fpgano):
        self.__check_value(('readInitStatus', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        buf = [0] * 64
        buf[0] = 0x20
        buf[1] = fpgano
        self.__pyhidobj.writeHID(buf)
        data = self.__read_result(('readInitStatus',
                                   'read FPGA initialization status'),
                                  0x20, 2)
        return data[0] | ( data[1] << 8 )

    def readSysmonChannel(self, fpgano, channel):
        self.__check_value(('readSysmonChannel', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('readSysmonChannel', 'System monitor channel'),
                           channel, 0x00, 0x0C)
        buf = [0] * 64
        buf[0] = 0x21
        buf[1] = fpgano
        buf[2] = channel
        self.__pyhidobj.writeHID(buf)
        data = self.__read_result(('readSysmonChannel',
                                   'read FPGA system monitor channel'),
                                  0x21, 2)
        val = data[0] | ( data[1] << 8 )
        if channel == 0 or channel == 4 or channel == 8:
            return val / 64.
        return val / 4096.

    def enableHICANNChannel(self, fpgano, channel):
        self.__check_value(('enableHICANNChannel', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('enableHICANNChannel', 'HICANN channel'),
                           channel, 0x00, 0x07)
        buf = [0] * 64
        buf[0] = 0x22
        buf[1] = fpgano
        buf[2] = channel
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('enableHICANNChannel',
                            'enable HICANN channel'),
                           0x22, 0)

    def disableHICANNChannel(self, fpgano, channel):
        self.__check_value(('disableHICANNChannel', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('disableHICANNChannel', 'HICANN channel'),
                           channel, 0x00, 0x07)
        buf = [0] * 64
        buf[0] = 0x23
        buf[1] = fpgano
        buf[2] = channel
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('disableHICANNChannel',
                            'disable HICANN channel'),
                           0x23, 0)

    def enableHICANNEq(self, fpgano):
        self.__check_value(('enableHICANNEq', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        buf = [0] * 64
        buf[0] = 0x24
        buf[1] = fpgano
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('enableHICANNEq',
                            'enable HICANN channel equalizer'),
                           0x24, 0)

    def disableHICANNEq(self, fpgano):
        self.__check_value(('disableHICANNEq', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        buf = [0] * 64
        buf[0] = 0x25
        buf[1] = fpgano
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('disableHICANNEq',
                            'disable HICANN channel equalizer'),
                           0x25, 0)

    def enableHICANNChannelPE(self, fpgano, channel):
        self.__check_value(('enableHICANNChannelPE', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('enableHICANNChannelPE', 'HICANN channel'),
                           channel, 0x00, 0x07)
        buf = [0] * 64
        buf[0] = 0x26
        buf[1] = fpgano
        buf[2] = channel
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('enableHICANNChannelPE',
                            'enable HICANN channel emphasis'),
                           0x26, 0)

    def disableHICANNChannelPE(self, fpgano, channel):
        self.__check_value(('disableHICANNChannelPE', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('disableHICANNChannelPE', 'HICANN channel'),
                           channel, 0x00, 0x07)
        buf = [0] * 64
        buf[0] = 0x27
        buf[1] = fpgano
        buf[2] = channel
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('disableHICANNChannelPE',
                            'disable HICANN channel emphasis'),
                           0x27, 0)

    def readDDR3MTStatus(self, fpgano):
        self.__check_value(('readDDR3MTStatus', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        buf = [0] * 64
        buf[0] = 0x28
        buf[1] = fpgano
        self.__pyhidobj.writeHID(buf)
        data = self.__read_result(('readDDR3MTStatus',
                                   'read DDR3 Memory controller and ' +
                                   'Memory-Test status'),
                                  0x28, 2)
        return (data[0], data[1])

    def readDDR3Count(self, fpgano):
        self.__check_value(('readDDR3Count', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        buf = [0] * 64
        buf[0] = 0x29
        buf[1] = fpgano
        self.__pyhidobj.writeHID(buf)
        data = self.__read_result(('readDDR3Count',
                                   'read DDR3 Memory controller count.'),
                                  0x29, 1)
        return data[0]

    def readDDR3CtrlInfo(self, fpgano, ctrlno):
        self.__check_value(('readDDR3CtrlInfo', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('readDDR3CtrlInfo', 'DDR3 controller number'),
                           ctrlno, 0x00, 0x0F)
        buf = [0] * 64
        buf[0] = 0x2A
        buf[1] = fpgano
        buf[2] = ctrlno
        self.__pyhidobj.writeHID(buf)
        data = self.__read_result(('readDDR3CtrlInfo',
                                   'read DDR3 Memory controller info'),
                                  0x2A, 2)
        return ( ( data[0] + 1 ) << 3, data[1] )

    def startDDR3MT(self, fpgano, mask):
        self.__check_value(('starDDR3MT', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('startDDR3MT', 'DDR3 memory test mask'),
                           mask, 0x00, 0xFF)
        buf = [0] * 64
        buf[0] = 0x2B
        buf[1] = fpgano
        buf[2] = mask
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('startDDR3MT',
                            'start DDR3 memory tests'),
                           0x2B)

    def readDDR3MTResult(self, fpgano, ctrlno):
        self.__check_value(('readDDR3MTResult', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('readDDR3MTResult', 'DDR3 controller number'),
                           ctrlno, 0x00, 0x0F)
        buf = [0] * 64
        buf[0] = 0x2C
        buf[1] = fpgano
        buf[2] = ctrlno
        self.__pyhidobj.writeHID(buf)
        data = self.__read_result(('readDDR3MTResult',
                                   'read DDR3 Memory test result'),
                                  0x2C, 14)
        writecycles = ( data[0] | ( data[1] << 8 ) | ( data[2] << 16 ) |
                        ( data[3] << 24 ) | ( data[4] << 32 ) )
        readcycles  = ( data[5] | ( data[6] << 8 ) | ( data[7] << 16 ) |
                        ( data[8] << 24 ) | ( data[9] << 32 ) )
        errors      = ( data[10] | ( data[11] << 8 ) | ( data[12] << 16 ) |
                        ( data[13] << 24 ) )
        return (errors, writecycles, readcycles)

    def readEthMgmt(self, fpgano, addr):
        self.__check_value(('readEthMgmt', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('readEthMgmt', 'Management register'),
                           addr, 0x00, 0x1F)
        buf = [0] * 64
        buf[0] = 0x2D
        buf[1] = fpgano
        buf[2] = addr
        self.__pyhidobj.writeHID(buf)
        data = self.__read_result(('readEthMgmt',
                                   'read management register'),
                                  0x2D, 4)
        return ( ( data[0] << 24 ) | ( data[1] << 16 ) |
                 ( data[2] << 8 ) | data[3] )

    def writeEthMgmt(self, fpgano, addr, data):
        self.__check_value(('writeEthMgmt', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        self.__check_value(('writeEthMgmt', 'Management register'),
                           addr, 0x00, 0x1F)
        self.__check_value(('writeEthMgmt', 'Management data'),
                           data)
        buf = [0] * 64
        buf[0] = 0x2E
        buf[1] = fpgano
        buf[2] = addr
        buf[3] = ( data >> 24 ) & 0xFF
        buf[4] = ( data >> 16 ) & 0xFF
        buf[5] = ( data >> 8 ) & 0xFF
        buf[6] = data & 0xFF
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('writeEthMgmt',
                            'write management register'),
                           0x2E)

    def resetEthPhy(self):
        buf = [0] * 64
        buf[0] = 0x70
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('resetEthPhy', 'reset ethernet PHY'),
                           0x70)

    def readEthPhyMDIO(self, phyaddr, regaddr):
        self.__check_value(('readPhyMDIO', 'PHY address'), phyaddr,
                           0x00, 0x1F)
        self.__check_value(('readPhyMDIO', 'PHY register address'), regaddr,
                           0x00, 0x1F)
        buf = [0] * 64
        buf[0] = 0x71
        buf[1] = phyaddr
        buf[2] = regaddr
        self.__pyhidobj.writeHID(buf)
        buf = self.__read_result(('readPhyMDIO', 'read MDIO register'),
                                 0x71, 2)
        return buf[0] | ( buf[1] << 8 )

    def writeEthPhyMDIO(self, phyaddr, regaddr, data):
        self.__check_value(('writePhyMDIO', 'PHY address'), phyaddr,
                           0x00, 0x1F)
        self.__check_value(('writePhyMDIO', 'PHY register address'), regaddr,
                           0x00, 0x1F)
        self.__check_value(('writePhyMDIO', 'PHY register data'), data,
                           0x00, 0xFFFF)
        buf = [0] * 64
        buf[0] = 0x72
        buf[1] = phyaddr
        buf[2] = regaddr
        buf[3] = data & 0xFF
        buf[4] = ( data >> 8 ) & 0xFF
        self.__pyhidobj.writeHID(buf)
        self.__read_result(('writePhyMDIO', 'write MDIO register'),
                           0x71)

    def getEthPhyIrqStatus(self):
        buf = [0] * 64
        buf[0] = 0x72
        self.__pyhidobj.writeHID(buf)
        buf = self.__read_result(('getEthPhyIrqStatus',
                                  'read Ethernet PHY interrupt status'),
                                 0x72, 1)
        return buf[0]
