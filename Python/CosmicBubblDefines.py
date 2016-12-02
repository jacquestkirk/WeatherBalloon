# This maps enums from firmware to python

class Enum_Commands:
    Cmd_ReadPress = 0
    Cmd_ReadTemp = 1
    Cmd_ReadImu = 2
    Cmd_ReadMag = 3
    Cmd_BlinkLed1 = 4
    Cmd_BlinkLed2 = 5
    Cmd_ReadTimestamp = 6
    Cmd_EraseFlash = 7
    Cmd_ReadFlashLoc = 8
    Cmd_ReadLastData = 9
    Cmd_ReadStartAddr = 10
    Cmd_ReadImuRegister = 11
    Cmd_WriteImuRegister = 12
    Cmd_StartRecording = 13
    Cmd_StopRecording = 14
    Cmd_StartImuStream = 15
    Cmd_StopImuStream = 16
    Cmd_ReadMagRegister = 17
    Cmd_WriteMagRegister = 18
    Cmd_ReadPress2Byte = 19
    Cmd_ReadPress3Byte = 20
    Cmd_WritePressRegister = 21
    Cmd_ReadFlashPage = 22
    Cmd_WriteFlashPage = 23
    Cmd_ERROR = 255

class Enum_Flash_TestData:
    allzeros = 0
    allones = 1
    primes = 2
    random = 3


