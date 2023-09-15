import serial
import sys
import os
import ctypes
import time
from pprint import pprint

class XTransferHeader(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("ioId", ctypes.c_char * 2),
        ("type", ctypes.c_int32),
        ("size", ctypes.c_uint32),
        ("xId", ctypes.c_char * 2)
    ]
    
    def __repr__(self):
        return "XTransferHeader:\n" + \
               "    ioId: %c%c\n" % (self.ioId[0], self.ioId[1]) + \
               "    type: %04x\n" % (self.type) + \
               "    size: %04x\n" % (self.size) + \
               "    xId:  %c%c\n" % (self.xId[0], self.xId[1])
               
class GbRomInfo(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("romName", ctypes.c_char * 0x10),
        ("mbcType", ctypes.c_int8),
        ("cartridgeType", ctypes.c_uint8),
        ("isGBC", ctypes.c_uint8),
        ("isSGB", ctypes.c_uint8),
        ("isRTC", ctypes.c_uint8),
        ("isRumble", ctypes.c_uint8),
        ("isBatteryBufferd", ctypes.c_uint8),
        ("romVersion", ctypes.c_uint8),
        ("romSize", ctypes.c_uint8),
        ("ramSize", ctypes.c_uint8),
        ("romBanks", ctypes.c_uint16),
        ("ramBanks", ctypes.c_uint16)
    ]
    
    def __repr__(self):
        return "RomInfo:\n" + \
               "    romName:       %s\n" % (self.romName) + \
               "    mbcType:       %04x\n" % (self.mbcType) + \
               "    cartridgeType: %04x\n" % (self.cartridgeType) + \
               "    Features:      isGBC:  %d, isSGB: %d, isRTC: %d, isRumble: %d, isBatteryBufferd: %d\n" % (self.isGBC, self.isSGB, self.isRTC, self.isRumble, self.isBatteryBufferd) + \
               "    romVersion:    %d\n" % (self.romVersion) + \
               "    romSize:       %d\n" % (self.romSize) + \
               "    ramSize:       %d\n" % (self.ramSize) + \
               "    romBanks:      %d\n" % (self.romBanks) + \
               "    ramBanks:      %d\n" % (self.ramBanks)

def sendData(tty, data):
    for n in range(0, len(data)):
        r = data[n]
        sys.stdout.write("%02x " % (r))
        tty.write(r)
        time.sleep(0.1)
    sys.stdout.write("\n")

with serial.Serial("COM17", 9600, timeout=1) as tty:
    recvBuffer = b''
    romInfo = None
    while True:
        recvBuffer = tty.read(1)
        if len(recvBuffer) > 0:
            if(recvBuffer.decode() == 'X'):
                recvBuffer = recvBuffer + tty.read(ctypes.sizeof(XTransferHeader)-1)
                header = XTransferHeader.from_buffer_copy(recvBuffer)
                print(header)
                hIo = header.ioId.decode()
                hx  = header.xId.decode()
                if hIo[1] == 'T' and hx[0] == 'E' and hx[1] == 'T':
                    txHeader = XTransferHeader()
                    txHeader.ioId = b'XT'
                    txHeader.xId = b'ET'
                    if(header.size > 0):
                        dataRaw = tty.read(header.size)
                        if header.type == 10:
                            if not(header.size == ctypes.sizeof(GbRomInfo)):
                                print("Invalid RomInfo Size! %d != %d" % (header.size, ctypes.sizeof(GbRomInfo)))
                            else:
                                romInfo = GbRomInfo.from_buffer_copy(dataRaw)
                                print(romInfo)
                                """
                                txHeader.type = header.type
                                txHeader.size = 0
                                print(bytearray(txHeader))
                                print(bytearray(recvBuffer))
                                tty.write(bytearray(txHeader))
                                print(txHeader)
                                print("Accepting Cartridge!")
                                time.sleep(1)
                                """
                                txHeader.type = 11
                                txHeader.size = 0
                                sendData(tty, bytearray(txHeader))
                                print(txHeader)
                                print("Waiting for RomDump!")
                        elif header.type == 11:
                            if not(romInfo):
                                print("No RomInfo!")
                            else:
                                fileName = romInfo.romName+".gb"
                                with open(fileName, 'wb') as file:
                                    file.write(dataRaw)
                                print("Saved new rom File: %s" % (fileName))
                        else:
                            print("Unknown Header!")
                            print(dataRaw)