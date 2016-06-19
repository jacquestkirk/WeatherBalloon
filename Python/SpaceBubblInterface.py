from SpaceBubblUsbDriver import *


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
    Cmd_ERROR = 255

class SpaceBubbl:

    def __init__(self):
        self.driver = SpaceBubblUsbDriver()

    def initiate(self):
        self.driver.FindDevice()


    def ReadPressure(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadPress])
        data_size_bytes = 8
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadPress, "Bubbl responded with an invalid response"

        # parse the response
        [pressure, startIndex] = self._Read32bit(read_bytes, startIndex)        #(read_bytes[0] << 24) + (read_bytes[1] << 16) + (read_bytes[2] << 8) + read_bytes[3];
        [temperature, startIndex] = self._Read32bit(read_bytes, startIndex)     #(read_bytes[4] << 24) + (read_bytes[5] << 16) + (read_bytes[6] << 8) + read_bytes[7];

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print("Pressure: ", hex(pressure))
        print("Temperature: ", hex(temperature))
        print(message)

    def ReadTemp(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadTemp])
        data_size_bytes = 8
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        #Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if(commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadTemp, "Bubbl responded with an invalid response"

        #parse the response
        [temp1, startIndex] = self._Read16bit(read_bytes, startIndex)  # (read_bytes[0] << 8) + read_bytes[1];
        [temp2, startIndex] = self._Read16bit(read_bytes, startIndex)  # (read_bytes[2] << 8) + read_bytes[3];
        [temp3, startIndex] = self._Read16bit(read_bytes, startIndex)  # (read_bytes[4] << 8) + read_bytes[5];
        [temp4, startIndex] = self._Read16bit(read_bytes, startIndex)  # (read_bytes[6] << 8) + read_bytes[7];

        # message_start = 8;
        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])


        print("Temp 1: ", hex(temp1))
        print("Temp 2: ", hex(temp2))
        print("Temp 3: ", hex(temp3))
        print("Temp 4: ", hex(temp4))
        print(message)

    def ParseError(self, read_bytes, startIndex):
        message = "Bubbl Error: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        raise Exception(message)

    def _Read8bit(self, read_bytes, startIndex):
        result = read_bytes[startIndex]
        nextIndex = startIndex + 1
        return [result, nextIndex]

    def _Read16bit(self, read_bytes, startIndex):
        result = (read_bytes[startIndex] << 8) + (read_bytes[startIndex + 1])
        nextIndex = startIndex + 2
        return [result, nextIndex]

    def _Read32bit(self, read_bytes, startIndex):
        result = (read_bytes[startIndex] << 24) + \
                 (read_bytes[startIndex + 1] << 16) + \
                 (read_bytes[startIndex + 2] << 8) + \
                 (read_bytes[startIndex + 3])
        nextIndex = startIndex + 4
        return [result, nextIndex]
