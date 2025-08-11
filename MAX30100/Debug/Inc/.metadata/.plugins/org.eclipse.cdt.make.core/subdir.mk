################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/.metadata/.plugins/org.eclipse.cdt.make.core/specs.c 

OBJS += \
./Inc/.metadata/.plugins/org.eclipse.cdt.make.core/specs.o 

C_DEPS += \
./Inc/.metadata/.plugins/org.eclipse.cdt.make.core/specs.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/.metadata/.plugins/org.eclipse.cdt.make.core/%.o Inc/.metadata/.plugins/org.eclipse.cdt.make.core/%.su Inc/.metadata/.plugins/org.eclipse.cdt.make.core/%.cyclo: ../Inc/.metadata/.plugins/org.eclipse.cdt.make.core/%.c Inc/.metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Inc-2f--2e-metadata-2f--2e-plugins-2f-org-2e-eclipse-2e-cdt-2e-make-2e-core

clean-Inc-2f--2e-metadata-2f--2e-plugins-2f-org-2e-eclipse-2e-cdt-2e-make-2e-core:
	-$(RM) ./Inc/.metadata/.plugins/org.eclipse.cdt.make.core/specs.cyclo ./Inc/.metadata/.plugins/org.eclipse.cdt.make.core/specs.d ./Inc/.metadata/.plugins/org.eclipse.cdt.make.core/specs.o ./Inc/.metadata/.plugins/org.eclipse.cdt.make.core/specs.su

.PHONY: clean-Inc-2f--2e-metadata-2f--2e-plugins-2f-org-2e-eclipse-2e-cdt-2e-make-2e-core

