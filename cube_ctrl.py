# -*- coding: utf-8 -*-
##-----------------------------------------------------------------
##
## Copyright (c) 2014 TU-Dresden  All rights reserved.
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
## Filename          :   cube_ctrl.py
## Project Name      :   MSP430
## Description       :   Cube Control and Monitor Software
##-----------------------------------------------------------------
## Version   |  Author    |  Date         |  Comment
## ----------------------------------------------------------------
## 0.1       | hartmann   | 19 Jun 2013   |  initial version
## ----------------------------------------------------------------
import curses
import time
import pyhid_cube
import threading
import pyhid

class cube_thread(threading.Thread):

    def __init__(self):
        threading.Thread.__init__(self)

        self.__pmic_rvoltage    = [[None for x in xrange(4)] for x in xrange(4)]
        self.__pmic_rtemp       = [[None for x in xrange(2)] for x in xrange(4)]
        self.__pmic_rcurrent    = [[None for x in xrange(4)] for x in xrange(4)]
        self.__pmic_temp        = [None]*4
        self.__pmic_voltage     = [None]*4
        self.__pmic_status      = [[None for x in xrange(13)] for x in xrange(4)]
        self.__pmic_status_word = [None]*4
        self.__pmic             = 0
        self.__fpga             = 0

        self.__fpga_sysmon      = [[None for x in xrange(13)] for x in xrange(4)]
        self.__fpga_status      = [[None for x in xrange(15)] for x in xrange(4)]
        self.__fpga_init_status = [None]*4
        self.__fpga_wafer_id    = [None]*4
        self.__fpga_eth_status  = [None]*4
        self.__fpga_eth_ip      = [None]*4
        self.__fpga_eth_cnt     = [[None for x in xrange(6)] for x in xrange(4)]

        self.__stop             = False
        self.__lock             = threading.Lock()
        self.__hid              = None
        self.__fw_version       = [None]*2

        try:
            self.__hid = pyhid.pyhidaccess()
            self.__hid.openHID(vid=0x0451, pid=0x4253)
            self.__pyhid_cube = pyhid_cube.pyhid_cube(self.__hid)
            self.__fw_version = self.__pyhid_cube.readFirmwareVersion()
        except pyhid.HIDError as e:
            self.__pyhid_cube = None
            pass

    def __linear16ToFloat(self, data, exp=-12):
        if data is None:
            return None
        return data * ( 2 ** exp )

    def __linear11ToFloat(self, data):
        if data is None:
            return None
        data_exp = ( data >> 11 ) & 0x1F
        exponent = data_exp
        if ( data_exp & 0x10 ) == 0x10:
            exponent = -( ~data_exp & 0xF ) - 1
        return ( data & 0x7ff ) * 2 ** exponent

    def __formatIP(self, ip):
        if ip is None:
            return 'N/A'
        return ( '%d.%d.%d.%d' % ((ip >> 24) & 0xFF,
                                  (ip >> 16) & 0xFF,
                                  (ip >> 8) & 0xFF,
                                  ip & 0xFF) )

    def run(self):
        while 1:
            if self.__pyhid_cube is None:
                try:
                    if self.__hid is None:
                        self.__hid = pyhid.pyhidaccess()
                    self.__hid.openHID(vid=0x0451, pid=0x4253)
                    self.__pyhid_cube = pyhid_cube.pyhid_cube(self.__hid)
                    self.__fw_version = self.__pyhid_cube.readFirmwareVersion()
                except pyhid.HIDError:
                    self.__pyhid_cube = None
                    pass

            for pmic in range(0, 4):

                data = [0]*13
                status = [0x01]*13

                self.__lock.acquire()
                try:
                    if self.__pyhid_cube is not None:
                        (status,
                         data) = self.__pyhid_cube.readPMICStatus(ucdno=pmic)
                except pyhid.HIDError as e:
                    if e.args[1] == pyhid.LIBUSB_ERROR_NO_DEVICE:
                        self.__hid.closeHID()
                        self.__pyhid_cube = None
                    pass
                except:
                    pass
                self.__lock.release()

                self.__pmic_voltage[pmic] = self.__linear11ToFloat(data[0])
                self.__pmic_temp[pmic]    = self.__linear11ToFloat(data[1])

                for j in range(0, 4):
                    val = self.__linear16ToFloat(data[2+j])
                    self.__pmic_rvoltage[pmic][j] = val

                for j in range(0, 4):
                    val = self.__linear11ToFloat(data[6+j])
                    self.__pmic_rcurrent[pmic][j] = val

                val = self.__linear11ToFloat(data[10])
                self.__pmic_rtemp[pmic][0] = val

                val = self.__linear11ToFloat(data[11])
                self.__pmic_rtemp[pmic][1] = val

                self.__pmic_status_word[pmic] = data[12]

                for i in range(0, 13):
                    self.__pmic_status[pmic][i] = status[i];

                if self.__stop:
                    break

                for fpga in range(0, 4):
                    data = [0]*15
                    status = [0x01]*15
                    if self.__pmic_status_word[fpga] is not None:
                        if self.__pmic_status_word[fpga] & 0x800:
                            continue
                    else:
                        continue

                    self.__lock.acquire()
                    try:
                        if self.__pyhid_cube is not None:
                            (status,
                             data) = self.__pyhid_cube.readFPGAStatus(fpgano=fpga)
                            eth_status = self.__pyhid_cube.readEthMgmt(fpgano=fpga,
                                                                       addr=0x0C)
                            self.__fpga_eth_status[fpga] = eth_status
                            eth_ip = self.__pyhid_cube.readEthMgmt(fpgano=fpga,
                                                                   addr=0x00)
                            self.__fpga_eth_ip[fpga] = eth_ip
                    except pyhid.HIDError as e:
                        if e.args[1] == pyhid.LIBUSB_ERROR_NO_DEVICE:
                            self.__hid.closeHID()
                            self.__pyhid_cube = None
                        pass
                    except:
                        self.__fpga_eth_status[fpga] = None
                        self.__fpga_eth_ip[fpga] = None
                        pass

                    try:
                        if self.__pyhid_cube is not None:
                            eth_status = self.__pyhid_cube.readEthMgmt(fpgano=fpga,
                                                                       addr=0x0C)
                            self.__fpga_eth_status[fpga] = eth_status
                    except pyhid.HIDError as e:
                        if e.args[1] == pyhid.LIBUSB_ERROR_NO_DEVICE:
                            self.__hid.closeHID()
                            self.__pyhid_cube = None
                        pass
                    except:
                        self.__fpga_eth_status[fpga] = None
                        pass

                    try:
                        if self.__pyhid_cube is not None:
                            eth_ip = self.__pyhid_cube.readEthMgmt(fpgano=fpga,
                                                                   addr=0x00)
                            self.__fpga_eth_ip[fpga] = eth_ip
                    except pyhid.HIDError as e:
                        if e.args[1] == pyhid.LIBUSB_ERROR_NO_DEVICE:
                            self.__hid.closeHID()
                            self.__pyhid_cube = None
                        pass
                    except:
                        self.__fpga_eth_ip[fpga] = None
                        pass

                    for j in range(0,6):
                        try:
                            if self.__pyhid_cube is not None:
                                if j < 4:
                                    addr = j + 0x10
                                else:
                                    addr = j + 0x14
                                eth_cnt = self.__pyhid_cube.readEthMgmt(fpgano=fpga,
                                                                        addr=addr)
                                self.__fpga_eth_cnt[fpga][j] = eth_cnt
                        except pyhid.HIDError as e:
                            if e.args[1] == pyhid.LIBUSB_ERROR_NO_DEVICE:
                                self.__hid.closeHID()
                                self.__pyhid_cube = None
                            pass
                        except:
                            self.__fpga_eth_cnt[fpga][j] = None
                            pass
                    self.__lock.release()

                    for j in range(0, 13):
                        if ( j & 0x3 ) == 0 and j != 12:
                            val = data[j] / 64.
                        else:
                            val = data[j] / 4096.
                        self.__fpga_sysmon[fpga][j] = val
                        self.__fpga_status[fpga][j] = status[j]

                    self.__fpga_init_status[fpga] = data[13]
                    self.__fpga_status[fpga][13] = status[13]

                    self.__fpga_wafer_id[fpga] = data[14]
                    self.__fpga_status[fpga][14] = status[14]

                    if self.__stop:
                        break

            time.sleep(0.2)

            if self.__stop:
                break

    def __hicannChannelPower(self, channel, en):
        if self.__pyhid_cube is None:
            return
        self.__lock.acquire()
        try:
            if en:
                self.__pyhid_cube.enableHICANNChannel(fpgano=self.__fpga,
                                                      channel=channel)
            else:
                self.__pyhid_cube.disableHICANNChannel(fpgano=self.__fpga,
                                                       channel=channel)
        except pyhid.HIDError:
            pass
        self.__lock.release()

    def __hicannChannelPE(self, channel, en):
        if self.__pyhid_cube is None:
            return
        self.__lock.acquire()
        try:
            if en:
                self.__pyhid_cube.enableHICANNChannelPE(fpgano=self.__fpga,
                                                        channel=channel)
            else:
                self.__pyhid_cube.disableHICANNChannelPE(fpgano=self.__fpga,
                                                         channel=channel)
        except pyhid.HIDError:
            pass
        self.__lock.release()

    def stopThread(self):
        self.__stop = True

    def enablePowerPMIC(self):
        if self.__pyhid_cube is None:
            return
        self.__lock.acquire()
        try:
            self.__pyhid_cube.powerControlPMIC(ucdno=self.__pmic,
                                               powerup=True)
        except pyhid.HIDError:
            pass
        self.__lock.release()

    def disablePowerPMIC(self):
        if self.__pyhid_cube is None:
            return
        self.__lock.acquire()
        try:
            self.__pyhid_cube.powerControlPMIC(ucdno=self.__pmic,
                                               powerup=False)
        except pyhid.HIDError:
            pass
        self.__lock.release()

    def getPMICStatusWord(self):
        return (self.__pmic_status[self.__pmic][12],
                self.__pmic_status_word[self.__pmic])

    def setCurrentPMIC(self, pmic):
        self.__pmic = pmic

    def getPMICRailVoltage(self, domain):
        return (self.__pmic_status[self.__pmic][domain + 2],
                self.__pmic_rvoltage[self.__pmic][domain])

    def getPMICRailCurrent(self, domain):
        return (self.__pmic_status[self.__pmic][domain + 6],
                self.__pmic_rcurrent[self.__pmic][domain])

    def getPMICRailTemp(self, domain):
        return (self.__pmic_status[self.__pmic][domain + 10],
                self.__pmic_rtemp[self.__pmic][domain])

    def getPMICVoltage(self):
        return (self.__pmic_status[self.__pmic][0],
                self.__pmic_voltage[self.__pmic])

    def getPMICTemp(self):
        return (self.__pmic_status[self.__pmic][1],
                self.__pmic_temp[self.__pmic])

    def setCurrentFPGA(self, fpga):
        self.__fpga = fpga

    def getFPGASysmon(self, channel):
        return (self.__fpga_status[self.__fpga][channel],
                self.__fpga_sysmon[self.__fpga][channel])

    def getFPGAStatus(self):
        return (self.__fpga_status[self.__fpga][13],
                self.__fpga_init_status[self.__fpga])

    def getEthStatus(self):
        return self.__fpga_eth_status[self.__fpga]

    def getWaferID(self):
        return (self.__fpga_status[self.__fpga][14],
                self.__fpga_wafer_id[self.__fpga])

    def hicannPowerEnable(self, channel):
        self.__hicannChannelPower(channel, True)

    def hicannPowerDisable(self, channel):
        self.__hicannChannelPower(channel, False)

    def hicannEnableChannelPE(self, channel):
        self.__hicannChannelPE(channel, True)

    def hicannDisableChannelPE(self, channel):
        self.__hicannChannelPE(channel, False)

    def hidActive(self):
        if self.__pyhid_cube is None:
            return False
        return True

    def getFirmwareVersion(self):
        if self.__pyhid_cube is None:
            return 'N/A'
        return ( '%d.%d' % (self.__fw_version[0],
                            self.__fw_version[1]) )

    def getEthIP(self):
        return self.__formatIP(self.__fpga_eth_ip[self.__fpga])

    def getEthCnt(self, counter):
        return self.__fpga_eth_cnt[self.__fpga][counter]

class cube_ctrl(object):

    WND_PMIC    = 1
    WND_FPGA    = 2
    WND_POWER   = 3
    WND_CHANNEL = 4
    WND_HICANN  = 5

    def __init__(self):
        self.__stdscr      = None
        self.__mainwin     = None
        self.__thread      = None

        self.__subpos      = 0
        self.__subwin      = None
        self.__subitems    = []
        self.__pmic        = 0
        self.__fpga        = 0
        self.__pmic_active = True
        self.__hicann_op   = 0
        self.__fpga_page   = 0

    def __navigateSubmenu(self, direction):
        self.__subpos += direction
        if self.__subpos < 0:
            self.__subpos = len(self.__subitems) - 1
        elif self.__subpos >= len(self.__subitems):
            self.__subpos = 0

    def __printSubmenuItems(self):
        for i in range(0, len(self.__subitems)):
            if i == self.__subpos:
                mode = curses.A_REVERSE
            else:
                mode = curses.A_NORMAL
            self.__subwin.addstr(3+i, 1, self.__subitems[i], mode)

    def __drawSubWindowForm(self, wndtype):
        xpos1 = 33
        xpos2 = 14
        if wndtype == cube_ctrl.WND_POWER:
            self.__subitems = ['    Off    ', '    On     ']
        elif wndtype == cube_ctrl.WND_HICANN:
            self.__subitems = [' Power Up Channel          ',
                               ' Power Down Channel        ',
                               ' Enable Channel Emphasis   ',
                               ' Disable Channel Emphasis  ']
            xpos1 = 25
            xpos2 = 30
        elif wndtype == cube_ctrl.WND_CHANNEL:
            self.__subitems = ['  Channel 1   ', '  Channel 2   ',
                               '  Channel 3   ', '  Channel 4   ',
                               '  Channel 5   ', '  Channel 6   ',
                               '  Channel 7   ', '  Channel 8   ',
                               ' All channels ']
            xpos1 = 34
            xpos2 = 16
        else:
            self.__subitems = ['  Socket 1 ', '  Socket 2 ',
                               '  Socket 3 ', '  Socket 4 ']

        self.__subwin = self.__mainwin.subwin(4 + len(self.__subitems),
                                              xpos2, 10 - len(self.__subitems) / 2,
                                              xpos1)
        self.__subwin.nodelay(1)
        self.__subwin.erase()
        self.__subwin.border()
        self.__subwin.hline(2, 1, 0, xpos2 - 2)

        if wndtype == cube_ctrl.WND_POWER:
            self.__subwin.addstr(1, 1, '   Power')
        elif wndtype == cube_ctrl.WND_PMIC:
            self.__subwin.addstr(1, 1, '    PMIC')
        elif wndtype == cube_ctrl.WND_CHANNEL:
            self.__subwin.addstr(1, 1, '   Channel')
        elif wndtype == cube_ctrl.WND_HICANN:
            self.__subwin.addstr(1, 1, '      HICANN Interface')
        else:
            self.__subwin.addstr(1, 1, '    FPGA')

        self.__printSubmenuItems()
        self.__subwin.refresh()

    def __drawSubWindow(self, wndtype):
        self.__drawSubWindowForm(wndtype=wndtype)

        while 1:
            inch = self.__subwin.getch()
            if inch == 27:
                self.__subwin.getch()
                inch2 = self.__subwin.getch()
                if inch2 == 65 or inch2 == 66:
                    if inch2 == 65:
                        self.__navigateSubmenu(-1)
                    else:
                        self.__navigateSubmenu(1)
                elif inch2 == -1:
                    break
            elif inch == 10:
                return True

            self.__updateData()
            self.__drawSubWindowForm(wndtype=wndtype)
            self.__printSubmenuItems()
            self.__subwin.refresh()

            time.sleep(0.05)

        return False

    def __drawMainWindow(self, hidActive, fwVersion):
        (y, x) = self.__stdscr.getmaxyx()
        if y < 25:
            raise Exception('Terminal height is too small. '
                            'Must be at least 25 rows.')
        if x < 80:
            raise Exception('Terminal width is too small. '
                            'Must be at least 80 columns.')
        if self.__mainwin is None:
            self.__mainwin = self.__stdscr.subwin(25, 80, 0, 0)

        if self.__pmic_active:
            device = 'PMIC'
            socket = self.__pmic + 1
        else:
            device = 'FPGA'
            socket = self.__fpga + 1

        self.__mainwin.erase()
        self.__mainwin.border()
        self.__mainwin.hline(2, 1, 0, 78)
        self.__mainwin.addstr(1, 16, 'Cube System Monitor and Control ('
                              '%s Socket %d)' % (device,socket))
        self.__mainwin.addstr(1, 70, 'FW: ' + fwVersion)
        self.__mainwin.hline(22, 1, 0, 78)

        if not hidActive:
            self.__mainwin.addstr(23, 60, 'Q or X: Quit')
            self.__mainwin.addstr(10, 26, 'No valid HID device found.')
        else:
            self.__mainwin.addstr(23, 2,
                                  '1: Select PMIC     '
                                  '2: Select FPGA     '
                                  '3: %s Control     '
                                  'Q or X: Quit' % device)
            if self.__pmic_active:
                self.__mainwin.addstr(4 , 3 , 'Input Voltage    : ')
                self.__mainwin.addstr(4 , 43, 'PMIC Temperature : ')
                self.__mainwin.addstr(6 , 3 , 'VDD Core         : ')
                self.__mainwin.addstr(8 , 3 , 'VDD 1.5V         : ')
                self.__mainwin.addstr(10, 3 , 'VDD 1.8V         : ')
                self.__mainwin.addstr(12, 3 , 'VDD 3.3V         : ')
                self.__mainwin.addstr(14, 3 , 'Current Core     : ')
                self.__mainwin.addstr(16, 3 , 'Current 1.5V     : ')
                self.__mainwin.addstr(18, 3 , 'Current 1.8V     : ')
                self.__mainwin.addstr(20, 3 , 'Current 3.3V     : ')
                self.__mainwin.addstr(6 , 43, 'Temp. Core/1.5V  : ')
                self.__mainwin.addstr(8 , 43, 'Temp. 1.8V/3.3V  : ')
                self.__mainwin.addstr(10, 43, 'Output OFF       : ')
                self.__mainwin.addstr(12, 43, 'Power Good       : ')
            elif self.__fpga_page == 0:
                self.__mainwin.addstr(4 , 3 , 'Current Temp.   : ')
                self.__mainwin.addstr(6 , 3 , 'Current VCCINT  : ')
                self.__mainwin.addstr(8 , 3 , 'Current VCCAUX  : ')
                self.__mainwin.addstr(10, 3 , 'Current VCCBRAM : ')
                self.__mainwin.addstr(12, 3 , 'Min. Temp.      : ')
                self.__mainwin.addstr(14, 3 , 'Min. VCCINT     : ')
                self.__mainwin.addstr(16, 3 , 'Min. VCCAUX     : ')
                self.__mainwin.addstr(18, 3 , 'Min. VCCBRAM    : ')
                self.__mainwin.addstr(20, 3 , 'VCCEXT          : ')
                self.__mainwin.addstr(4 , 43, 'Max. Temp.      : ')
                self.__mainwin.addstr(6 , 43, 'Max. VCCINT     : ')
                self.__mainwin.addstr(8 , 43, 'Max. VCCAUX     : ')
                self.__mainwin.addstr(10, 43, 'Max. VCCBRAM    : ')
                self.__mainwin.addstr(12, 43, 'Init. Done      : ')
                self.__mainwin.addstr(14, 43, 'Init. Error     : ')
                self.__mainwin.addstr(16, 43, 'Wafer ID        : ')
                self.__mainwin.addstr(18, 43, 'Edge/Socket ID  : ')
                self.__mainwin.addstr(20, 43, 'IP LSB          : ')
            elif self.__fpga_page == 1:
                self.__mainwin.addstr(4 , 3 , 'IP              : ')
                self.__mainwin.addstr(6 , 3 , 'Subnet          : ')
                self.__mainwin.addstr(8 , 3 , 'Gateway         : ')
                self.__mainwin.addstr(10, 3 , 'MAC-Address     : ')
                self.__mainwin.addstr(12, 3 , 'Init. Done      : ')
                self.__mainwin.addstr(14, 3 , 'Init. Error     : ')
                self.__mainwin.addstr(16, 3 , 'Wafer ID Locked : ')
                self.__mainwin.addstr(18, 3 , 'System Reset    : ')
                self.__mainwin.addstr(20, 3 , 'PLL Reset       : ')
                self.__mainwin.addstr(4 , 43, 'PLL Error       : ')
                self.__mainwin.addstr(6 , 43, 'PLL Lock Error  : ')
                self.__mainwin.addstr(8 , 43, 'PLL Lock Lost   : ')
                self.__mainwin.addstr(10, 43, 'PLL Init. Error : ')
                self.__mainwin.addstr(12, 43, 'Eth. Init. Err. : ')
                self.__mainwin.addstr(14, 43, 'Eth. PHY Error  : ')
                self.__mainwin.addstr(16, 43, 'Eth. PHY Valid  : ')
                self.__mainwin.addstr(18, 43, 'Eth. PHY Link   : ')
                self.__mainwin.addstr(20, 43, 'Eth. PHY Speed  : ')
            else:
                self.__mainwin.addstr(4 , 3 , 'RX Frames       : ')
                self.__mainwin.addstr(6 , 3 , 'RX Errors       : ')
                self.__mainwin.addstr(4 , 43, 'TX Frames       : ')
                self.__mainwin.addstr(6 , 43, 'TX Errors       : ')
                self.__mainwin.addstr(8 , 3 , 'RX IP Frames    : ')
                self.__mainwin.addstr(10, 3 , 'RX IP Errors    : ')

            if not self.__pmic_active:
                self.__mainwin.addstr(3 , 78, '^')
                self.__mainwin.addstr(21, 78, 'v')

        self.__mainwin.refresh()

    def __toString(self, string, value):
        if value is None:
            val = 'N/A     '
        else:
            val = string % value
        return val

    def __toStringYesNo(self, value, bit, invert=False):
        if value is None:
            val = 'N/A'
        else:
            mask = 1 << bit
            bit = True if value & mask else False
            if invert:
                bit = not bit
            if bit:
                val = 'Yes'
            else:
                val = 'No '
        return val

    def __toStringActive(self, value, bit):
        if value is None:
            val = 'N/A'
        else:
            mask = 1 << bit
            bit = True if value & mask else False
            if bit:
                val = 'Active  '
            else:
                val = 'Inactive'
        return val

    def __updateData(self):
        if not self.__thread.hidActive():
            return
        if self.__pmic_active:
            (status, voltage) = self.__thread.getPMICVoltage()
            if status != 0x00:
                voltage = None
            self.__mainwin.addstr(4, 24,
                                  self.__toString('%2.3f V',
                                                  voltage))
            (status, temp) = self.__thread.getPMICTemp()
            if status != 0x00:
                temp = None
            self.__mainwin.addstr(4, 64,
                                  self.__toString('%1.2f C',
                                                  temp))
            for domain in range(0, 4):
                (status, voltage) = self.__thread.getPMICRailVoltage(domain)
                if status != 0x00:
                    voltage = None
                self.__mainwin.addstr(6 + 2 * domain, 24,
                                      self.__toString('%2.3f V', voltage))
                (status, current) = self.__thread.getPMICRailCurrent(domain)
                if status != 0x00:
                    current = None
                self.__mainwin.addstr(14 + 2 * domain, 24,
                                      self.__toString('%2.3f A', current))
            for domain in range(0, 2):
                (status, temp) = self.__thread.getPMICRailTemp(domain)
                if status != 0x00:
                    temp = None
                self.__mainwin.addstr(6 + 2 * domain, 64,
                                      self.__toString('%1.2f C', temp))

            (status, status_word) = self.__thread.getPMICStatusWord()
            if status != 0x00:
                status_word = None
            self.__mainwin.addstr(10, 64,
                                  self.__toStringYesNo(status_word, 6))
            self.__mainwin.addstr(12, 64,
                                  self.__toStringYesNo(status_word, 11, True))
        elif self.__fpga_page == 0:
            for channel in range(0, 13):
                (status, sysmon) = self.__thread.getFPGASysmon(channel)
                if status != 0x00:
                    sysmon = None
                if ( channel & 0x3 ) == 0 and channel != 12:
                    text = '%1.2f C'
                else:
                    text = '%2.3f V'
                if channel < 8 or channel == 12:
                    xpos = 24
                else:
                    xpos = 64
                if channel < 8:
                    ypos = 4 + 2 * channel
                elif channel == 12:
                    ypos = 20
                else:
                    ypos = 4 + 2 * ( channel - 8 )
                self.__mainwin.addstr(ypos, xpos,
                                      self.__toString(text, sysmon))

        if not self.__pmic_active:
            wafer_id  = None
            edge_id   = None
            socket_id = None
            (status, init_status) = self.__thread.getFPGAStatus()
            if status != 0x00:
                init_status = None
            if init_status is not None:
                if init_status & 0x4:
                    (wstatus, wafer_id) = self.__thread.getWaferID()
                    if wstatus != 0x00:
                        wafer_id = None
                edge_id   = ( init_status >> 9  ) & 0x3
                socket_id = ( init_status >> 11 ) & 0xF
                ip_lsb = ( edge_id << 5 ) | socket_id

            if self.__fpga_page == 0:
                self.__mainwin.addstr(12, 64,
                                      self.__toStringYesNo(init_status, 0))
                self.__mainwin.addstr(14, 64,
                                      self.__toStringYesNo(init_status, 1))
                self.__mainwin.addstr(16, 64,
                                      self.__toString('%d  ', wafer_id))
                if ( edge_id is not None and
                     socket_id is not None ):
                    self.__mainwin.addstr(18, 64,
                                          self.__toString('%d', edge_id) + '/' +
                                          self.__toString('%d', socket_id - 1))
                    self.__mainwin.addstr(20, 64,
                                          self.__toString('%d  ', ip_lsb))
                else:
                    self.__mainwin.addstr(18, 64, 'N/A')
                    self.__mainwin.addstr(20, 64, 'N/A')
            elif self.__fpga_page == 1:
                self.__mainwin.addstr(4, 24, self.__thread.getEthIP())
                if ( edge_id is not None and
                     socket_id is not None and
                     wafer_id is not None ) :
                    self.__mainwin.addstr(6, 24, '255.255.255.0')
                    self.__mainwin.addstr(8, 24, '192.168.%d.254' %
                                          (wafer_id))
                    self.__mainwin.addstr(10, 24, '5F:23:45:35:%02X:%02X' %
                                          (wafer_id, ip_lsb))
                else:
                    self.__mainwin.addstr(4, 24, 'N/A            ')
                    self.__mainwin.addstr(6, 24, 'N/A            ')
                    self.__mainwin.addstr(8, 24, 'N/A            ')
                    self.__mainwin.addstr(10, 24, 'N/A                ')

                self.__mainwin.addstr(12, 24,
                                      self.__toStringYesNo(init_status, 0))
                self.__mainwin.addstr(14, 24,
                                      self.__toStringYesNo(init_status, 1))
                self.__mainwin.addstr(16, 24,
                                      self.__toStringYesNo(init_status, 2))
                self.__mainwin.addstr(18, 24,
                                      self.__toStringActive(init_status, 3))
                self.__mainwin.addstr(20, 24,
                                      self.__toStringActive(init_status, 4))
                self.__mainwin.addstr(4, 64,
                                      self.__toStringYesNo(init_status, 5))
                self.__mainwin.addstr(6, 64,
                                      self.__toStringYesNo(init_status, 6))
                self.__mainwin.addstr(8, 64,
                                      self.__toStringYesNo(init_status, 8))
                self.__mainwin.addstr(10, 64,
                                      self.__toStringYesNo(init_status, 15))
                self.__mainwin.addstr(12, 64,
                                          self.__toStringYesNo(init_status, 7))
                eth_status = self.__thread.getEthStatus()
                self.__mainwin.addstr(14, 64,
                                          self.__toStringYesNo(eth_status, 0))
                self.__mainwin.addstr(16, 64,
                                          self.__toStringYesNo(eth_status, 1))
                if eth_status is not None:
                    if eth_status & 0x4:
                        self.__mainwin.addstr(18, 64, 'Up  ')
                    else:
                        self.__mainwin.addstr(18, 64, 'Down')
                    eth_speed = ( eth_status >> 4 ) & 0x3
                    if eth_speed == 0:
                        self.__mainwin.addstr(20, 64, '10 Mb/s  ')
                    elif eth_speed == 1:
                        self.__mainwin.addstr(20, 64, '100 Mb/s ')
                    elif eth_speed == 2:
                        self.__mainwin.addstr(20, 64, '1000 Mb/s')
                    else:
                        self.__mainwin.addstr(20, 64, 'N/A      ')
                else:
                    self.__mainwin.addstr(18, 64, 'N/A  ')
                    self.__mainwin.addstr(20, 64, 'N/A      ')
            else:
                cnt = self.__thread.getEthCnt(0)
                self.__mainwin.addstr(4, 24,
                                      self.__toString('%d', cnt))
                cnt = self.__thread.getEthCnt(1)
                self.__mainwin.addstr(6, 24,
                                      self.__toString('%d', cnt))
                cnt = self.__thread.getEthCnt(2)
                self.__mainwin.addstr(4, 64,
                                      self.__toString('%d', cnt))
                cnt = self.__thread.getEthCnt(3)
                self.__mainwin.addstr(6, 64,
                                      self.__toString('%d', cnt))
                cnt = self.__thread.getEthCnt(4)
                self.__mainwin.addstr(8, 24,
                                      self.__toString('%d', cnt))
                cnt = self.__thread.getEthCnt(5)
                self.__mainwin.addstr(10, 24,
                                      self.__toString('%d', cnt))

        self.__mainwin.refresh()

    def __doHicannOp(self):
        if self.__hicann_op == 0:
            doFunc = self.__thread.hicannPowerEnable
        elif self.__hicann_op == 1:
            doFunc = self.__thread.hicannPowerDisable
        elif self.__hicann_op == 2:
            doFunc = self.__thread.hicannEnableChannelPE
        elif self.__hicann_op == 3:
            doFunc = self.__thread.hicannDisableChannelPE

        if self.__subpos < 8:
            doFunc(channel=self.__subpos)
        else:
            for i in xrange(8):
                doFunc(channel=i)

    def __mainloop(self):
        self.__stdscr.nodelay(1)

        self.__thread = cube_thread()
        self.__thread.start()

        self.__drawMainWindow(self.__thread.hidActive(),
                              self.__thread.getFirmwareVersion())

        hidActive = self.__thread.hidActive()
        fwVersion = self.__thread.getFirmwareVersion()
        while 1:
            if self.__thread.hidActive() != hidActive:
                self.__drawMainWindow(self.__thread.hidActive(),
                                      self.__thread.getFirmwareVersion())
                hidActive = self.__thread.hidActive()
                fwVersion = self.__thread.getFirmwareVersion()
            inch = self.__stdscr.getch()
            if inch != -1:
                if inch == curses.KEY_RESIZE:
                    self.__drawMainWindow(self.__thread.hidActive(),
                                          fwVersion)
                elif ( not self.__pmic_active and inch == 66 and
                       self.__thread.hidActive() ):
                    self.__fpga_page += 1
                    if self.__fpga_page > 2:
                        self.__fpga_page = 0
                    self.__drawMainWindow(self.__thread.hidActive(),
                                          fwVersion)
                elif ( not self.__pmic_active and inch == 65 and
                       self.__thread.hidActive() ):
                    self.__fpga_page -= 1
                    if self.__fpga_page < 0:
                        self.__fpga_page = 2
                    self.__drawMainWindow(self.__thread.hidActive(),
                                          fwVersion)
                try:
                    instr = str(chr(inch))
                except:
                    pass
                else:
                    if instr.upper() == 'X' or instr.upper() == 'Q':
                        self.__updateData()
                        break
                    elif instr == '1' and hidActive:
                        self.__subpos = self.__pmic
                        if self.__drawSubWindow(cube_ctrl.WND_PMIC):
                            self.__pmic = self.__subpos
                            self.__thread.setCurrentPMIC(self.__pmic)
                            self.__pmic_active = True
                        self.__drawMainWindow(self.__thread.hidActive(),
                                              fwVersion)
                    elif instr == '2' and hidActive:
                        self.__subpos = self.__fpga
                        if self.__drawSubWindow(cube_ctrl.WND_FPGA):
                            self.__fpga = self.__subpos
                            self.__thread.setCurrentFPGA(self.__fpga)
                            self.__pmic_active = False
                            self.__fpga_page = 0
                        self.__drawMainWindow(self.__thread.hidActive(),
                                              fwVersion)
                    elif instr == '3' and hidActive:
                        if self.__pmic_active:
                            (status,
                             status_word) = self.__thread.getPMICStatusWord()
                            if status != 0:
                                self.__subpos = 0
                            elif ( status_word & 0x0840 ) == 0:
                                self.__subpos = 1
                            else:
                                self.__subpos = 0

                            if self.__drawSubWindow(cube_ctrl.WND_POWER):
                                if self.__subpos == 1:
                                    self.__thread.enablePowerPMIC()
                                else:
                                    self.__thread.disablePowerPMIC()
                            self.__drawMainWindow(self.__thread.hidActive(),
                                                  fwVersion)
                        else:
                            self.__subpos = self.__hicann_op
                            if self.__drawSubWindow(cube_ctrl.WND_HICANN):
                                self.__hicann_op = self.__subpos
                                if self.__subpos < 4:
                                    self.__subpos = 0
                                    self.__drawMainWindow(self.__thread.hidActive(),
                                                          fwVersion)
                                    if self.__drawSubWindow(cube_ctrl.WND_CHANNEL):
                                        self.__doHicannOp()
                                self.__drawMainWindow(self.__thread.hidActive(),
                                                      fwVersion)
                            else:
                                self.__drawMainWindow(self.__thread.hidActive(),
                                                      fwVersion)

            self.__updateData()

            time.sleep(0.1)

        self.__thread.stopThread()
        self.__thread.join()
        self.__thread = None

    def startup(self):
        try:
            self.__stdscr = curses.initscr()

            curses.noecho()
            curses.cbreak()
            curses.curs_set(0)

            self.__mainloop()
        finally:
            curses.echo()
            curses.nocbreak()
            curses.curs_set(1)
            curses.endwin()
            if self.__thread is not None:
                self.__thread.stopThread()
                self.__thread.join()

if __name__ == '__main__':
    ctrl = cube_ctrl()
    ctrl.startup()
