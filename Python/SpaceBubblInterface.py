from SpaceBubblUsbDriver import *
import math

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
        [pressure, startIndex] = self._Read32bit(read_bytes, startIndex)
        [temperature, startIndex] = self._Read32bit(read_bytes, startIndex)

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
        [temp1, startIndex] = self._Read16bit(read_bytes, startIndex)
        [temp2, startIndex] = self._Read16bit(read_bytes, startIndex)
        [temp3, startIndex] = self._Read16bit(read_bytes, startIndex)
        [temp4, startIndex] = self._Read16bit(read_bytes, startIndex)

        # message_start = 8;
        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])


        print("Temp 1: ", hex(temp1))
        print("Temp 2: ", hex(temp2))
        print("Temp 3: ", hex(temp3))
        print("Temp 4: ", hex(temp4))
        print(message)

    def ReadImu(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadImu])
        data_size_bytes = 12
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        #Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if(commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadImu, "Bubbl responded with an invalid response"

        #parse the response
        [x_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
        [y_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
        [z_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
        [x_gyro, startIndex]  = self._Read16bit(read_bytes, startIndex)
        [y_gyro, startIndex]  = self._Read16bit(read_bytes, startIndex)
        [z_gyro, startIndex]  = self._Read16bit(read_bytes, startIndex)

        # message_start = 8;
        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        accel_scale_g = 8.0;
        gyro_scale_dps = 500.0;

        x_accel_g = accel_scale_g * self._Convert2sComplement(x_accel , 16) / 32768.0
        y_accel_g = accel_scale_g * self._Convert2sComplement(y_accel , 16) / 32768.0
        z_accel_g = accel_scale_g * self._Convert2sComplement(z_accel , 16) / 32768.0

        total_accel_g = math.sqrt(math.pow(x_accel_g, 2) + math.pow( y_accel_g, 2) + math.pow(z_accel_g, 2))

        print("X Acceletation: ", x_accel_g, " g")
        print("Y Acceletation: ", y_accel_g, " g")
        print("Z Acceletation: ", z_accel_g, " g")
        print("Total Acceleration: ", total_accel_g, " g")

        print("X Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(x_gyro, 16) / 32768.0, " dps")
        print("Y Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(y_gyro, 16) / 32768.0, " dps")
        print("Z Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(z_gyro, 16) / 32768.0, " dps")

        print(message)

    def ReadMag(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadMag])
        data_size_bytes = 6
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadMag, "Bubbl responded with an invalid response"

        # parse the response
        [x_mag, startIndex] = self._Read16bit(read_bytes, startIndex)
        [y_mag, startIndex] = self._Read16bit(read_bytes, startIndex)
        [z_mag, startIndex] = self._Read16bit(read_bytes, startIndex)

        # message_start = 8;
        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print("X Magnetic Field: ", hex(x_mag))
        print("Y Magnetic Field: ", hex(y_mag))
        print("Z Magnetic Field: ", hex(z_mag))

        print(message)

    def BlinkLed1(self):
        self.driver.WriteData([Enum_Commands.Cmd_BlinkLed1])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_BlinkLed1, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def BlinkLed2(self):
        self.driver.WriteData([Enum_Commands.Cmd_BlinkLed2])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_BlinkLed2, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def ReadTimeStamp(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadTimestamp])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadTimestamp, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def EraseFlash(self):
        self.driver.WriteData([Enum_Commands.Cmd_EraseFlash])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_EraseFlash, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def ReadFlashLocations(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadFlashLoc])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadFlashLoc, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def ReadLastData(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadLastData])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadLastData, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def ReadStartAddr(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadStartAddr])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadStartAddr, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

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

    def _Convert2sComplement(self, number, num_bits):
        if number >> (num_bits - 1) >= 1:
            return number - (1 << num_bits)
        else:
            return number