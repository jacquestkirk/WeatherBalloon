################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include CMSIS/efm32hg/subdir.mk
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
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -T "WeatherBalloon.ld" -Xlinker --gc-sections -Xlinker -Map="WeatherBalloon.map" --specs=nano.specs -o WeatherBalloon.axf "./src/InitDevice.o" "./src/Scheduler.o" "./src/main.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_system.o" "./CMSIS/efm32hg/startup_gcc_efm32hg.o" "./CMSIS/efm32hg/system_efm32hg.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
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
