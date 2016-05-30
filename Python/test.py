## To get pyusb working use Zadig's libusb-win32 driver

from SpaceBubblUsbDriver import *
sbusb = SpaceBubblUsbDriver()
sbusb.FindDevice()
sbusb.WriteString("Hello There")
print(sbusb.ReadString())
