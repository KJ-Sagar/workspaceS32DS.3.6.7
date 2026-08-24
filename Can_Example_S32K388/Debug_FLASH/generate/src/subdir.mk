################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../generate/src/CanIf_Cfg.c \
../generate/src/CanIf_VS_0_PBcfg.c \
../generate/src/Can_43_FLEXCAN_Ipw_VS_0_PBcfg.c \
../generate/src/Can_43_FLEXCAN_VS_0_PBcfg.c \
../generate/src/Clock_Ip_Cfg.c \
../generate/src/Clock_Ip_VS_0_PBcfg.c \
../generate/src/FlexCAN_Ip_VS_0_PBcfg.c \
../generate/src/Igf_Port_Ip_Cfg.c \
../generate/src/IntCtrl_Ip_Cfg.c \
../generate/src/Mcu_Cfg.c \
../generate/src/Mcu_VS_0_PBcfg.c \
../generate/src/Mpu_M7_Ip_Cfg.c \
../generate/src/OsIf_Cfg.c \
../generate/src/Platform_Cfg.c \
../generate/src/Platform_Ipw_Cfg.c \
../generate/src/Power_Ip_Cfg.c \
../generate/src/Power_Ip_VS_0_PBcfg.c \
../generate/src/Ram_Ip_Cfg.c \
../generate/src/Ram_Ip_VS_0_PBcfg.c 

OBJS += \
./generate/src/CanIf_Cfg.o \
./generate/src/CanIf_VS_0_PBcfg.o \
./generate/src/Can_43_FLEXCAN_Ipw_VS_0_PBcfg.o \
./generate/src/Can_43_FLEXCAN_VS_0_PBcfg.o \
./generate/src/Clock_Ip_Cfg.o \
./generate/src/Clock_Ip_VS_0_PBcfg.o \
./generate/src/FlexCAN_Ip_VS_0_PBcfg.o \
./generate/src/Igf_Port_Ip_Cfg.o \
./generate/src/IntCtrl_Ip_Cfg.o \
./generate/src/Mcu_Cfg.o \
./generate/src/Mcu_VS_0_PBcfg.o \
./generate/src/Mpu_M7_Ip_Cfg.o \
./generate/src/OsIf_Cfg.o \
./generate/src/Platform_Cfg.o \
./generate/src/Platform_Ipw_Cfg.o \
./generate/src/Power_Ip_Cfg.o \
./generate/src/Power_Ip_VS_0_PBcfg.o \
./generate/src/Ram_Ip_Cfg.o \
./generate/src/Ram_Ip_VS_0_PBcfg.o 

C_DEPS += \
./generate/src/CanIf_Cfg.d \
./generate/src/CanIf_VS_0_PBcfg.d \
./generate/src/Can_43_FLEXCAN_Ipw_VS_0_PBcfg.d \
./generate/src/Can_43_FLEXCAN_VS_0_PBcfg.d \
./generate/src/Clock_Ip_Cfg.d \
./generate/src/Clock_Ip_VS_0_PBcfg.d \
./generate/src/FlexCAN_Ip_VS_0_PBcfg.d \
./generate/src/Igf_Port_Ip_Cfg.d \
./generate/src/IntCtrl_Ip_Cfg.d \
./generate/src/Mcu_Cfg.d \
./generate/src/Mcu_VS_0_PBcfg.d \
./generate/src/Mpu_M7_Ip_Cfg.d \
./generate/src/OsIf_Cfg.d \
./generate/src/Platform_Cfg.d \
./generate/src/Platform_Ipw_Cfg.d \
./generate/src/Power_Ip_Cfg.d \
./generate/src/Power_Ip_VS_0_PBcfg.d \
./generate/src/Ram_Ip_Cfg.d \
./generate/src/Ram_Ip_VS_0_PBcfg.d 


# Each subdirectory must supply rules for building sources it contributes
generate/src/%.o: ../generate/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@generate/src/CanIf_Cfg.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


