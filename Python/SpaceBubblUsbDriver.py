## To get pyusb working use Zadig's libusb-win32 driver


import usb.core
import usb.util

class SpaceBubblUsbDriver:


    def __init__(self):
        self.timeout = 100
        self.read_address = 0x81
        self.write_address = 0x1
        self.vid = 0x10C4
        self.pid = 0x3

        self.error_message_size = 100

    def FindDevice(self):
        # find our device
        self.dev = usb.core.find(idVendor=self.vid, idProduct = self.pid)

        # was it found?
        if self.dev is None:
            raise ValueError('Device not found')

        else:
            print(self.dev)


    def WriteString(self,dataToSend):
        self.dev.write(self.write_address, dataToSend, self.timeout)

    def ReadString(self):
        read_bytes = self.dev.read(self.read_address, 100, self.timeout) #Right not 100 is just some arbitrary magic number, not sure what this number should be
        #todo: Add some error handling: 1)There is nothing in the buffer (i.e. timeout),
        read_str = ""
        for n in range(0,len(read_bytes)):
            read_str += chr(read_bytes[n])
        return(read_str)

    def QueryTemp(self):
        self.WriteString("temperature")
        data_size_bytes = 8
        read_bytes = self.dev.read(self.read_address, data_size_bytes + self.error_message_size, self.timeout)  # Right not 100 is just some arbitrary magic number, not sure what this number should be

        temp1 = (read_bytes[0] << 8) + read_bytes[1];
        temp2 = (read_bytes[2] << 8) + read_bytes[3];
        temp3 = (read_bytes[4] << 8) + read_bytes[5];
        temp4 = (read_bytes[6] << 8) + read_bytes[7];

        message_start = 8;
        message = "Message: "
        for n in range(message_start,len(read_bytes)):
            message += chr(read_bytes[n])

        print("Temp 1: ", hex(temp1))
        print("Temp 2: ", hex(temp2))
        print("Temp 3: ", hex(temp3))
        print("Temp 4: ", hex(temp4))
        print(message)



