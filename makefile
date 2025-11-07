CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -Wall
LDSCRIPT = STM32F410RBTX_FLASH.ld

build:
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c core/src/main.c $(CFLAGS) -o build/main.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c core/src/globals.c $(CFLAGS) -o build/globals.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c config/config.c $(CFLAGS) -o build/config.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c core/src/interrupt.c $(CFLAGS) -o build/interrupt.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c core/src/syscalls.c $(CFLAGS) -o build/syscalls.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c core/src/system_clock.c $(CFLAGS) -o build/system_clock.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c core/src/irq.c $(CFLAGS) -o build/irq.o
# 	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c drivers/peripherals/dht11.c $(CFLAGS) -o build/dht11.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c drivers/peripherals/gpio.c $(CFLAGS) -o build/gpio.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c drivers/peripherals/dma.c $(CFLAGS) -o build/dma.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c drivers/peripherals/tim.c $(CFLAGS) -o build/tim.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c drivers/peripherals/i2c.c $(CFLAGS) -o build/i2c.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c drivers/peripherals/ssd1306.c $(CFLAGS) -o build/ssd1306.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c core/src/system_stm32f4xx.c $(CFLAGS) -o build/system_stm32f4xx.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I config -I core/inc -c startup/startup_stm32f410rbtx.s $(CFLAGS) -o build/startup_stm32f410rbtx.o
	$(CC) -T $(LDSCRIPT) build/*.o -o build/firmware.elf -lc -lm -lnosys -u _printf_float -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
	$(OBJCOPY) -O binary build/firmware.elf build/firmware.bin

debug:
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c core/src/main.c $(CFLAGS) -o build/main.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c core/src/globals.c $(CFLAGS) -o build/globals.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c config/config.c $(CFLAGS) -o build/config.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c core/src/interrupt.c $(CFLAGS) -o build/interrupt.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c core/src/syscalls.c $(CFLAGS) -o build/syscalls.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c core/src/system_clock.c $(CFLAGS) -o build/system_clock.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c core/src/irq.c $(CFLAGS) -o build/irq.o
# 	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c drivers/peripherals/dht11.c $(CFLAGS) -o build/dht11.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c drivers/peripherals/gpio.c $(CFLAGS) -o build/gpio.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c drivers/peripherals/dma.c $(CFLAGS) -o build/dma.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c drivers/peripherals/tim.c $(CFLAGS) -o build/tim.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c drivers/peripherals/i2c.c $(CFLAGS) -o build/i2c.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c drivers/peripherals/ssd1306.c $(CFLAGS) -o build/ssd1306.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c core/src/system_stm32f4xx.c $(CFLAGS) -o build/system_stm32f4xx.o
	$(CC) -I drivers/CMSIS -I drivers/device -I drivers/peripherals -I core/inc -g -c startup/startup_stm32f410rbtx.s $(CFLAGS) -o build/startup_stm32f410rbtx.o
	$(CC) -T $(LDSCRIPT) build/*.o -o build/firmware.elf -lc -lm -lnosys -u _printf_float -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
	$(OBJCOPY) -O binary build/firmware.elf build/firmware.bin

flash:
	st-flash write ./build/firmware.bin 0x8000000

clean:
	rm -f build/*.o build/*.elf build/*.bin

.PHONY: build debug flash clean
