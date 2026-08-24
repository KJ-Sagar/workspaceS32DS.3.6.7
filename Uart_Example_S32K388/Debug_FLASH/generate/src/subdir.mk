################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../generate/src/CDD_Mcl_Cfg.c \
../generate/src/CDD_Mcl_PBcfg.c \
../generate/src/CDD_Uart_PBcfg.c \
../generate/src/Clock_Ip_Cfg.c \
../generate/src/Clock_Ip_PBcfg.c \
../generate/src/Dma_Ip_Cfg.c \
../generate/src/Dma_Ip_PBcfg.c \
../generate/src/Emios_Mcl_Ip_PBcfg.c \
../generate/src/Flexio_Mcl_Ip_PBcfg.c \
../generate/src/Flexio_Uart_Ip_PBcfg.c \
../generate/src/Igf_Port_Ip_Cfg.c \
../generate/src/Igf_Port_Ip_PBcfg.c \
../generate/src/IntCtrl_Ip_Cfg.c \
../generate/src/Lcu_Ip_Cfg.c \
../generate/src/Lcu_Ip_PBcfg.c \
../generate/src/Lpuart_Uart_Ip_PBcfg.c \
../generate/src/Mcu_Cfg.c \
../generate/src/Mcu_PBcfg.c \
../generate/src/Mpu_M7_Ip_Cfg.c \
../generate/src/OsIf_Cfg.c \
../generate/src/Platform_Cfg.c \
../generate/src/Platform_Ipw_Cfg.c \
../generate/src/Port_Cfg.c \
../generate/src/Port_PBcfg.c \
../generate/src/Power_Ip_Cfg.c \
../generate/src/Power_Ip_PBcfg.c \
../generate/src/Ram_Ip_Cfg.c \
../generate/src/Ram_Ip_PBcfg.c \
../generate/src/Siul2_Port_Ip_PBcfg.c \
../generate/src/Trgmux_Ip_Cfg.c \
../generate/src/Trgmux_Ip_PBcfg.c \
../generate/src/Tspc_Port_Ip_PBcfg.c \
../generate/src/Uart_Ipw_PBcfg.c 

OBJS += \
./generate/src/CDD_Mcl_Cfg.o \
./generate/src/CDD_Mcl_PBcfg.o \
./generate/src/CDD_Uart_PBcfg.o \
./generate/src/Clock_Ip_Cfg.o \
./generate/src/Clock_Ip_PBcfg.o \
./generate/src/Dma_Ip_Cfg.o \
./generate/src/Dma_Ip_PBcfg.o \
./generate/src/Emios_Mcl_Ip_PBcfg.o \
./generate/src/Flexio_Mcl_Ip_PBcfg.o \
./generate/src/Flexio_Uart_Ip_PBcfg.o \
./generate/src/Igf_Port_Ip_Cfg.o \
./generate/src/Igf_Port_Ip_PBcfg.o \
./generate/src/IntCtrl_Ip_Cfg.o \
./generate/src/Lcu_Ip_Cfg.o \
./generate/src/Lcu_Ip_PBcfg.o \
./generate/src/Lpuart_Uart_Ip_PBcfg.o \
./generate/src/Mcu_Cfg.o \
./generate/src/Mcu_PBcfg.o \
./generate/src/Mpu_M7_Ip_Cfg.o \
./generate/src/OsIf_Cfg.o \
./generate/src/Platform_Cfg.o \
./generate/src/Platform_Ipw_Cfg.o \
./generate/src/Port_Cfg.o \
./generate/src/Port_PBcfg.o \
./generate/src/Power_Ip_Cfg.o \
./generate/src/Power_Ip_PBcfg.o \
./generate/src/Ram_Ip_Cfg.o \
./generate/src/Ram_Ip_PBcfg.o \
./generate/src/Siul2_Port_Ip_PBcfg.o \
./generate/src/Trgmux_Ip_Cfg.o \
./generate/src/Trgmux_Ip_PBcfg.o \
./generate/src/Tspc_Port_Ip_PBcfg.o \
./generate/src/Uart_Ipw_PBcfg.o 

C_DEPS += \
./generate/src/CDD_Mcl_Cfg.d \
./generate/src/CDD_Mcl_PBcfg.d \
./generate/src/CDD_Uart_PBcfg.d \
./generate/src/Clock_Ip_Cfg.d \
./generate/src/Clock_Ip_PBcfg.d \
./generate/src/Dma_Ip_Cfg.d \
./generate/src/Dma_Ip_PBcfg.d \
./generate/src/Emios_Mcl_Ip_PBcfg.d \
./generate/src/Flexio_Mcl_Ip_PBcfg.d \
./generate/src/Flexio_Uart_Ip_PBcfg.d \
./generate/src/Igf_Port_Ip_Cfg.d \
./generate/src/Igf_Port_Ip_PBcfg.d \
./generate/src/IntCtrl_Ip_Cfg.d \
./generate/src/Lcu_Ip_Cfg.d \
./generate/src/Lcu_Ip_PBcfg.d \
./generate/src/Lpuart_Uart_Ip_PBcfg.d \
./generate/src/Mcu_Cfg.d \
./generate/src/Mcu_PBcfg.d \
./generate/src/Mpu_M7_Ip_Cfg.d \
./generate/src/OsIf_Cfg.d \
./generate/src/Platform_Cfg.d \
./generate/src/Platform_Ipw_Cfg.d \
./generate/src/Port_Cfg.d \
./generate/src/Port_PBcfg.d \
./generate/src/Power_Ip_Cfg.d \
./generate/src/Power_Ip_PBcfg.d \
./generate/src/Ram_Ip_Cfg.d \
./generate/src/Ram_Ip_PBcfg.d \
./generate/src/Siul2_Port_Ip_PBcfg.d \
./generate/src/Trgmux_Ip_Cfg.d \
./generate/src/Trgmux_Ip_PBcfg.d \
./generate/src/Tspc_Port_Ip_PBcfg.d \
./generate/src/Uart_Ipw_PBcfg.d 


# Each subdirectory must supply rules for building sources it contributes
generate/src/%.o: ../generate/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@generate/src/CDD_Mcl_Cfg.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


