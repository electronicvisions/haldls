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
import hashlib

class pyhid_cube(pyhid.pyhidaccess):

    def __init__(self, vid=0x0451, pid=0x4253):
        pyhid.pyhidaccess.__init__(self)
        self.openHID(vid=vid, pid=pid)
        self.__usb_timeout = 1000
        self.__usb_response_retries = 5

    def __del__(self):
        self.closeHID()

    def closeDevice(self):
        self.closeHID()

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
        buf = self.readHID(64, self.__usb_timeout)
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
        self.writeHID(buf)
        self.__read_result(('powerControlPMIC', 'enable/disable PMIC'),
                           0x02, 0)

    def setPassword(self, passwd):
        pw = hashlib.sha256(passwd).hexdigest()
        buf = [0] * 64
        buf[0] = 0x01
        buf[1] = len(pw) / 2
        for i in range(0, len(pw)/2):
            buf[i+2] = int(pw[2*i:2*i+2],16)
        self.writeHID(buf)
        self.__read_result(('setPassword', 'set access password'),
                           0x01)

    def resetPMIC(self):
        buf = [0] * 64
        buf[0] = 0x03
        self.writeHID(buf)
        self.__read_result(('resetPMIC', 'reset PMICs'),
                           0x03)

    def readBytePMIC(self, ucdno, cc, page=None):
        self.__check_pmic_params('readBytePMIC', ucdno, cc, page)
        buf = [0] * 64
        buf[0] = 0x10
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        self.writeHID(buf)
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
        self.writeHID(buf)
        self.__read_result(('writeBytePMIC', 'write byte to PMIC'),
                           0x11, 0)

    def readWordPMIC(self, ucdno, cc, page=None):
        self.__check_pmic_params('readWordPMIC', ucdno, cc, page)
        buf = [0] * 64
        buf[0] = 0x12
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        self.writeHID(buf)
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
        self.writeHID(buf)
        self.__read_result(('writeWordPMIC', 'write word to PMIC'),
                           0x13, 0)

    def readBlockPMIC(self, ucdno, cc, page=None):
        self.__check_pmic_params('readBlockPMIC', ucdno, cc, page)
        buf = [0] * 64
        buf[0] = 0x14
        buf[1] = ucdno
        buf[2] = 0xFF if page is None else page
        buf[3] = cc
        self.writeHID(buf)
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
        self.writeHID(buf)
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
        self.writeHID(buf)
        self.__read_result(('sendBytePMIC', 'send byte to PMIC'),
                           0x16, 0)

    def readPMICStatus(self, ucdno):
        self.__check_value('PMIC number', ucdno, 0, 3)
        buf = [0] * 64
        buf[0] = 0x30
        buf[1] = ucdno
        self.writeHID(buf)
        block = self.__read_result(('readPMICStatus', 'read PMIC status'),
                                    0x30, 28)
        valid = ( block[0] << 8 ) | block[1]
        data = [None] * 13
        for i in range(0, 13):
            if ( valid & ( 1 << i ) ):
                data[i] = ( block[2 * i + 2] << 8 ) | block[2 * i + 3]
        return data

    def readFPGAStatus(self, fpgano):
        self.__check_value('FPGA number', fpgano, 0, 3)
        buf = [0] * 64
        buf[0] = 0x31
        buf[1] = fpgano
        self.writeHID(buf)
        block = self.__read_result(('readFPGAStatus', 'read FPGA status'),
                                    0x31, 30)
        valid = ( block[0] << 8 ) | block[1]
        data = [None] * 14
        for i in range(0, 14):
            if ( valid & ( 1 << i ) ):
                data[i] = ( block[2 * i + 2] << 8 ) | block[2 * i + 3]
        return data

    def readInitStatus(self, fpgano):
        self.__check_value(('readInitStatus', 'FPGA number'),
                           fpgano, 0x00, 0x03)
        buf = [0] * 64
        buf[0] = 0x20
        buf[1] = fpgano
        self.writeHID(buf)
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
        self.writeHID(buf)
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
        self.writeHID(buf)
        self.__read_result(('enableHICANNChannel',
                            'enable HICANN channel'),
                           0x22, 0)

    def resetEthPhy(self):
        buf = [0] * 64
        buf[0] = 0x70
        self.writeHID(buf)
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
        self.writeHID(buf)
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
        self.writeHID(buf)
        self.__read_result(('writePhyMDIO', 'write MDIO register'),
                           0x71)

    def getEthPhyIrqStatus(self):
        buf = [0] * 64
        buf[0] = 0x72
        self.writeHID(buf)
        buf = self.__read_result(('getEthPhyIrqStatus',
                                  'read Ethernet PHY interrupt status'),
                                 0x72, 1)
        return buf[0]
