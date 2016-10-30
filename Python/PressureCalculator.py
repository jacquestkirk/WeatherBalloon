import math
import time

class PresResult:
    def __init(self):
        self.pressure_mbar = -1
        self.temperature_degC = 9999

class PressureCalculator:
    #this pressure calculator is based off of page 7 of the MS5607-02BA03 Pressure Sensor


    def __init__(self):
        self.C1 = -1    #Pressure Sensitivity
        self.C2 = -1    #Pressure Offset
        self.C3 = -1    #Temp Coefficient of Pressure Sensitivity
        self.C4 = -1    #Temp Coefficient of Pressure Offset
        self.C5 = -1    #Reference Temp
        self.C6 = -1    #Temperature coefficient of temp measurement
        self.D1 = -1    #Digital Pressure Value
        self.D2 = -1    #Digital Temp. Value


    def ReadPressureSensor(self, fn_ReadPress2Byte, fn_ReadPress3byte, fn_WritePressRegister ):
        fn_WritePressRegister(0x1E)
        time.sleep(1)
        self.ReadValues(fn_ReadPress3byte, fn_WritePressRegister)
        self.ReadCalibrations(fn_ReadPress2Byte)
        return self.CalculatePressure();


    def ReadCalibrations(self, fn_ReadPress2Byte):
        self.C1 = fn_ReadPress2Byte(0b10100010)
        self.C2 = fn_ReadPress2Byte(0b10100100)
        self.C3 = fn_ReadPress2Byte(0b10100110)
        self.C4 = fn_ReadPress2Byte(0b10101000)
        self.C5 = fn_ReadPress2Byte(0b10101010)
        self.C6 = fn_ReadPress2Byte(0b10101100)

    def ReadValues(self, fn_ReadPress3Byte, fn_WritePressRegister):
        fn_WritePressRegister(0x48)
        time.sleep(1)
        self.D1 = fn_ReadPress3Byte(0x00)
        fn_WritePressRegister(0x58)
        time.sleep(1)
        self.D2 = fn_ReadPress3Byte(0x00)

    def CalculatePressure(self):

        tempRef = self.C5 * math.pow(2,8)
        deltaTemp = self.D2- tempRef
        temp = 2000 + deltaTemp * self.C6 / math.pow(2, 23)
        temp_degC = temp/100

        pressOffset = self.C2* math.pow(2,17) + self.C4*deltaTemp/math.pow(2,6)
        pressSens = self.C1 * math.pow(2,16) + (self.C3*deltaTemp)/math.pow(2,7)
        pressure = (self.D1 * pressSens/pow(2,21) - pressOffset)/math.pow(2,15)
        pressure_mbar = pressure/100

        result = PresResult()
        result.pressure_mbar = pressure_mbar
        result.temperature_degC = temp_degC


        return result