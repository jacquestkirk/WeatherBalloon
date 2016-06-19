################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Sensors/subdir.mk
-include src/subdir.mk
-include emusb/subdir.mk
-include emlib/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/efm32hg/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: WeatherBalloon.axf

# Tool invocations
WeatherBalloon.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -T "WeatherBalloon.ld" -Xlinker --gc-sections -Xlinker -Map="WeatherBalloon.map" --specs=nano.specs -o WeatherBalloon.axf "./src/Cli.o" "./src/InitDevice.o" "./src/Scheduler.o" "./src/descriptors.o" "./src/main.o" "./src/Sensors/pressure.o" "./src/Sensors/temperature.o" "./emusb/em_usbd.o" "./emusb/em_usbdch9.o" "./emusb/em_usbdep.o" "./emusb/em_usbdint.o" "./emusb/em_usbh.o" "./emusb/em_usbhal.o" "./emusb/em_usbhep.o" "./emusb/em_usbhint.o" "./emusb/em_usbtimer.o" "./emlib/em_acmp.o" "./emlib/em_adc.o" "./emlib/em_aes.o" "./emlib/em_assert.o" "./emlib/em_burtc.o" "./emlib/em_cmu.o" "./emlib/em_cryotimer.o" "./emlib/em_crypto.o" "./emlib/em_dac.o" "./emlib/em_dbg.o" "./emlib/em_dma.o" "./emlib/em_ebi.o" "./emlib/em_emu.o" "./emlib/em_gpcrc.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_idac.o" "./emlib/em_int.o" "./emlib/em_lcd.o" "./emlib/em_ldma.o" "./emlib/em_lesense.o" "./emlib/em_letimer.o" "./emlib/em_leuart.o" "./emlib/em_mpu.o" "./emlib/em_msc.o" "./emlib/em_opamp.o" "./emlib/em_pcnt.o" "./emlib/em_prs.o" "./emlib/em_rmu.o" "./emlib/em_rtc.o" "./emlib/em_rtcc.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./emlib/em_vcmp.o" "./emlib/em_wdog.o" "./Drivers/dmadrv.o" "./Drivers/gpiointerrupt.o" "./Drivers/nvm.o" "./Drivers/nvm_hal.o" "./Drivers/rtcdriver.o" "./Drivers/sleep.o" "./Drivers/spidrv.o" "./Drivers/uartdrv.o" "./CMSIS/efm32hg/startup_gcc_efm32hg.o" "./CMSIS/efm32hg/system_efm32hg.o" "./BSP/bsp_bcc.o" "./BSP/bsp_dk_3200.o" "./BSP/bsp_dk_3201.o" "./BSP/bsp_dk_leds.o" "./BSP/bsp_dk_mcuboard.o" "./BSP/bsp_stk.o" "./BSP/bsp_stk_leds.o" "./BSP/bsp_trace.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: WeatherBalloon.hex'
	arm-none-eabi-objcopy -O ihex "WeatherBalloon.axf" "WeatherBalloon.hex"
	@echo ' '

	@echo 'Building bin file: WeatherBalloon.bin'
	arm-none-eabi-objcopy -O binary "WeatherBalloon.axf" "WeatherBalloon.bin"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "WeatherBalloon.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) WeatherBalloon.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
