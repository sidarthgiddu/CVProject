################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
# -include hw/subdir.mk
-include src/subdir.mk
-include hw/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
manr_sobel.elf.size \


# All Target
all: manr_sobel.elf secondary-outputs

# Tool invocations
manr_sobel.elf: $(OBJS)  $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linux gcc linker'
	sds++ -o"manr_sobel.elf"  $(OBJS) $(USER_OBJS) $(SDIFLAGS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

./hw/manr.o: ../hw/manr.c
	sdscc -c $(SDIFLAGS) -o"$@" "$<" 

./hw/sobel_filter.o: ../hw/sobel_filter.cpp
	sds++ -c $(SDIFLAGS) -o"$@" "$<" 

./hw/rgb_pad2ycbcr.o: ../hw/rgb_pad2ycbcr.c
	sdscc -c $(SDIFLAGS) -o"$@" "$<" 

./hw/ycbcr2rgb_pad.o: ../hw/ycbcr2rgb_pad.c
	sds++ -c $(SDIFLAGS) -o"$@" "$<" 

./hw/median_filter.o: ../hw/median_filter.cpp
	sds++ -c $(SDIFLAGS) -o"$@" "$<"

./hw/median_filter_time.o: ../hw/median_filter_time.cpp
	sds++ -c $(SDIFLAGS) -o"$@" "$<"

./hw/corner_detect.o: ../hw/corner_detect.cpp
	sds++ -c $(SDIFLAGS) -o"$@" "$<"

manr_sobel.elf.size: manr_sobel.elf
	@echo 'Invoking: ARM Linux Print Size'
	arm-xilinx-linux-gnueabi-size manr_sobel.elf  |tee "manr_sobel.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(S_UPPER_DEPS) manr_sobel.elf manr_sobel.elf.bit
	-@echo ' '

ultraclean:
	-$(RM) -rf _sds sd_card 
	-@echo ' '

secondary-outputs: $(ELFSIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
