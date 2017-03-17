from SpaceBubblUsbDriver import *
import math
import numpy as np
import matplotlib.pyplot as plt
from PressureCalculator import *
from CosmicBubblDefines import *


class SpaceBubbl:

    def __init__(self):
        self.driver = SpaceBubblUsbDriver()

    def initiate(self):
        self.driver.FindDevice()


    def ReadPressure(self):

        ##Read calibration Values
        self.driver.WriteData([Enum_Commands.Cmd_ReadPressCal])
        data_size_bytes = 13
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadPressCal, "Bubbl responded with an invalid response"

        # parse the response
        [C1, startIndex] = self._Read16bit(read_bytes, startIndex)
        [C2, startIndex] = self._Read16bit(read_bytes, startIndex)
        [C3, startIndex] = self._Read16bit(read_bytes, startIndex)
        [C4, startIndex] = self._Read16bit(read_bytes, startIndex)
        [C5, startIndex] = self._Read16bit(read_bytes, startIndex)
        [C6, startIndex] = self._Read16bit(read_bytes, startIndex)

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(hex(C1))
        print(hex(C2))
        print(hex(C3))
        print(hex(C4))
        print(hex(C5))
        print(hex(C6))
        print(message)


        ## Read Pressure and Temperature

        self.driver.WriteData([Enum_Commands.Cmd_ReadPress])
        data_size_bytes = 8
        read_bytes = self.driver.ReadData(data_size_bytes, timeout_ms=5000)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadPress, "Bubbl responded with an invalid response"

        # parse the response
        [D1, startIndex] = self._Read32bit(read_bytes, startIndex)
        [D2, startIndex] = self._Read32bit(read_bytes, startIndex)

        tempRef = C5 * math.pow(2, 8)
        deltaTemp = D2 - tempRef
        temp = 2000 + deltaTemp * C6 / math.pow(2, 23)
        temp_degC = temp / 100

        pressOffset = C2 * math.pow(2, 17) + C4 * deltaTemp / math.pow(2, 6)
        pressSens = C1 * math.pow(2, 16) + (C3 * deltaTemp) / math.pow(2, 7)
        pressure = (D1 * pressSens / pow(2, 21) - pressOffset) / math.pow(2, 15)
        pressure_mbar = pressure / 100


        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print("Pressure: ", pressure_mbar)
        print("Temperature: ", temp_degC)
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

        mag_scale_g = 4.0;

        x_mag_g = mag_scale_g * self._Convert2sComplement(x_mag, 16) / 32768.0
        y_mag_g = mag_scale_g * self._Convert2sComplement(y_mag, 16) / 32768.0
        z_mag_g = mag_scale_g * self._Convert2sComplement(z_mag, 16) / 32768.0

        total_mag_g = math.sqrt(math.pow(x_mag_g, 2) + math.pow(y_mag_g, 2) + math.pow(z_mag_g, 2))

        print("X Magnetic Field: ", x_mag_g, " gauss")
        print("Y Magnetic Field: ", y_mag_g, " gauss")
        print("Z Magnetic Field: ", z_mag_g, " gauss")
        print("Total Magnetic Field: ", total_mag_g, " gauss")

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
        data_size_bytes = 4
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadTimestamp, "Bubbl responded with an invalid response"

        [timestamp, startIndex] = self._Read32bit(read_bytes, startIndex)

        print("Timestamp: " + str(timestamp))

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

    def ReadPressureTemp (self):
        press = PressureCalculator()
        pressResult = press.ReadPressureSensor( self.ReadPress2Byte, self.ReadPress3Byte, self.WritePressRegister)
        print("Pressure: {:.2f} mbar".format(pressResult.pressure_mbar))
        print("Temperature: {:.2f} degC".format(pressResult.temperature_degC))

    def ReadPress2Byte(self, regToRead):
        self.driver.WriteData([Enum_Commands.Cmd_ReadPress2Byte, regToRead])
        data_size_bytes = 2
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadPress2Byte, "Bubbl responded with an invalid response"

        [registerContents, startIndex] = self._Read16bit(read_bytes, startIndex);
        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print("Register " + hex(regToRead) + " :    " + hex(registerContents))
        print(message)
        return registerContents

    def ReadPress3Byte(self, regToRead):
        self.driver.WriteData([Enum_Commands.Cmd_ReadPress3Byte, regToRead])
        data_size_bytes = 4
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadPress3Byte, "Bubbl responded with an invalid response"

        [registerContents, startIndex] = self._Read32bit(read_bytes, startIndex);
        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print("Register " + hex(regToRead) + " :    " + hex(registerContents))
        print(message)
        return registerContents

    def WritePressRegister(self, regToWrite):
        self.driver.WriteData([Enum_Commands.Cmd_WritePressRegister, regToWrite])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_WritePressRegister, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def ReadMagRegister(self, regToRead):
        self.driver.WriteData([Enum_Commands.Cmd_ReadMagRegister, regToRead])
        data_size_bytes = 1
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadMagRegister, "Bubbl responded with an invalid response"

        [registerContents, startIndex] = self._Read8bit(read_bytes, startIndex);
        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print("Register " + hex(regToRead) + " :    " + hex(registerContents))
        print(message)

    def WriteMagRegister(self, regToWrite, dataToWrite):
        self.driver.WriteData([Enum_Commands.Cmd_WriteMagRegister, regToWrite, dataToWrite])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_WriteMagRegister, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def ReadImuRegister(self, regToRead):
        self.driver.WriteData([Enum_Commands.Cmd_ReadImuRegister, regToRead])
        data_size_bytes = 1
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0


        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadImuRegister, "Bubbl responded with an invalid response"

        [registerContents, startIndex] = self._Read8bit(read_bytes, startIndex);
        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print("Register " + hex(regToRead) + " :    " + hex(registerContents))
        print(message)

    def WriteImuRegister(self, regToWrite, dataToWrite):
        self.driver.WriteData([Enum_Commands.Cmd_WriteImuRegister, regToWrite, dataToWrite])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_WriteImuRegister, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

    def ReadFlashPage(self, pageNumber):

        assert(pageNumber< 2**15)

        pageNumber_high = (pageNumber >> 8) & 0xFF
        pageNumber_low = pageNumber & 0xFF

        self.driver.WriteData([Enum_Commands.Cmd_ReadFlashPage, pageNumber_high, pageNumber_low])
        data_size_bytes = 257
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadFlashPage, "Bubbl responded with an invalid response"

        page_contents = []

        for i in range(0,256):
            [value, startIndex] = self._Read8bit(read_bytes, startIndex)
            page_contents.append(value)
            #print(value)

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

        return page_contents

    def WriteFlashPage(self, pageNumber, flashTestDataEnum):
        assert (pageNumber < 2 ** 15)

        pageNumber_high = (pageNumber >> 8) & 0xFF
        pageNumber_low = pageNumber & 0xFF

        data_to_write= [Enum_Commands.Cmd_WriteFlashPage, pageNumber_high, pageNumber_low, flashTestDataEnum] #first elements of the data to write

        self.driver.WriteData(data_to_write)
        data_size_bytes = 1
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_WriteFlashPage, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

        return

    def StartImuStream(self):
        self.driver.WriteData([Enum_Commands.Cmd_StartImuStream])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_StartImuStream, "Bubbl responded with an invalid response"

        message = "Message: "
        for n in range(startIndex, len(read_bytes)):
            message += chr(read_bytes[n])

        print(message)

        while(True):
            self._PollForImuData()

    def _PollForImuData(self):

        log_size = 1028
        xaccels = np.zeros(log_size)
        yaccels = np.zeros(log_size)
        zaccels = np.zeros(log_size)
        totalAccels = np.zeros(log_size)

        sampleRate_Hz = 14.9
        times = np.arange(log_size) * 1/sampleRate_Hz

        plt.ion()
        count = 0;

        while(True):
            data_size_bytes = 12
            read_bytes = self.driver.ReadData(data_size_bytes, 5000) #This is blocking waiting till data comes in
            startIndex = 0


            #Add IMU stream data flag here?

            #parse the response
            [x_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
            [y_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
            [z_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
            [x_gyro, startIndex]  = self._Read16bit(read_bytes, startIndex)
            [y_gyro, startIndex]  = self._Read16bit(read_bytes, startIndex)
            [z_gyro, startIndex]  = self._Read16bit(read_bytes, startIndex)

            accel_scale_g = 8.0;
            gyro_scale_dps = 500.0;

            x_accel_g = accel_scale_g * self._Convert2sComplement(x_accel , 16) / 32768.0
            y_accel_g = accel_scale_g * self._Convert2sComplement(y_accel , 16) / 32768.0
            z_accel_g = accel_scale_g * self._Convert2sComplement(z_accel , 16) / 32768.0

            total_accel_g = math.sqrt(math.pow(x_accel_g, 2) + math.pow( y_accel_g, 2) + math.pow(z_accel_g, 2))

            #Append the new value to the end and remove the first value
            xaccels = np.append(xaccels, x_accel_g)
            yaccels = np.append(yaccels, y_accel_g)
            zaccels = np.append(zaccels, z_accel_g)
            totalAccels = np.append(totalAccels, total_accel_g)

            xaccels = np.delete(xaccels, 0)
            yaccels = np.delete(yaccels, 0)
            zaccels = np.delete(zaccels, 0)
            totalAccels = np.delete(totalAccels, 0)


            count += 1
            print("Count: ", count)

            if (count%21 == 0):
                accelFig = plt.figure(1)
                accelFig.clear();
                plt.subplot(211)
                plt.plot(times, xaccels, times, yaccels, times, zaccels)
                plt.subplot(212)
                plt.plot(times, totalAccels)
                plt.pause(0.05)

                #calculate and plot fft
                fftFig = plt.figure(2)
                fftFig.clear()
                fftTotalAccel = np.fft.fft(totalAccels)
                freq = np.fft.fftfreq(log_size, 1.0/sampleRate_Hz)
                plt.plot(freq, 20*np.log10(fftTotalAccel))

            print("X Acceletation: ", x_accel_g, " g")
            print("Y Acceletation: ", y_accel_g, " g")
            print("Z Acceletation: ", z_accel_g, " g")
            print("Total Acceleration: ", total_accel_g, " g")

            print("X Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(x_gyro, 16) / 32768.0, " dps")
            print("Y Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(y_gyro, 16) / 32768.0, " dps")
            print("Z Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(z_gyro, 16) / 32768.0, " dps")

    def ReadImu(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadImu])
        data_size_bytes = 12
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadImu, "Bubbl responded with an invalid response"

        # parse the response
        [x_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
        [y_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
        [z_accel, startIndex] = self._Read16bit(read_bytes, startIndex)
        [x_gyro, startIndex] = self._Read16bit(read_bytes, startIndex)
        [y_gyro, startIndex] = self._Read16bit(read_bytes, startIndex)
        [z_gyro, startIndex] = self._Read16bit(read_bytes, startIndex)

        accel_scale_g = 8.0;
        gyro_scale_dps = 500.0;

        x_accel_g = accel_scale_g * self._Convert2sComplement(x_accel, 16) / 32768.0
        y_accel_g = accel_scale_g * self._Convert2sComplement(y_accel, 16) / 32768.0
        z_accel_g = accel_scale_g * self._Convert2sComplement(z_accel, 16) / 32768.0

        total_accel_g = math.sqrt(math.pow(x_accel_g, 2) + math.pow(y_accel_g, 2) + math.pow(z_accel_g, 2))

        print("X Acceleration: ", hex(x_accel))
        print("Y Acceleration: ", hex (y_accel))
        print("Z Acceleration: ", hex(z_accel))

        print("X Acceleration: ", x_accel_g, " g")
        print("Y Acceleration: ", y_accel_g, " g")
        print("Z Acceleration: ", z_accel_g, " g")
        print("Total Acceleration: ", total_accel_g, " g")

        print("X Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(x_gyro, 16) / 32768.0, " dps")
        print("Y Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(y_gyro, 16) / 32768.0, " dps")
        print("Z Angular Velocity: ", gyro_scale_dps * self._Convert2sComplement(z_gyro, 16) / 32768.0, " dps")





    def StopImuStream(self):
        self.driver.WriteData([Enum_Commands.Cmd_StopImuStream])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_StopImuStream, "Bubbl responded with an invalid response"

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

    def ReadFlashId(self):
        self.driver.WriteData([Enum_Commands.Cmd_ReadFlashId])
        data_size_bytes = 0
        read_bytes = self.driver.ReadData(data_size_bytes)

        startIndex = 0

        # Error Handling
        [commandEcho, startIndex] = self._Read8bit(read_bytes, startIndex)
        if (commandEcho == 255):
            self.ParseError(read_bytes, startIndex)
        assert commandEcho == Enum_Commands.Cmd_ReadFlashId, "Bubbl responded with an invalid response"

        [id, startIndex] = self._Read32bit(read_bytes, startIndex)

        print (hex(id))

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



if __name__ == "__main__":
    from SpaceBubblInterface import *
    sb = SpaceBubbl()
    sb.initiate()