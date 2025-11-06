CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -Wall
LDSCRIPT = STM32F410RBTX_FLASH.ld

build:
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Src/main.c $(CFLAGS) -o Build/main.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Src/gpio.c $(CFLAGS) -o Build/gpio.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Src/irq.c $(CFLAGS) -o Build/irq.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Src/dma.c $(CFLAGS) -o Build/dma.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Src/i2c.c $(CFLAGS) -o Build/i2c.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Src/ssd1306.c $(CFLAGS) -o Build/ssd1306.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Src/syscalls.c $(CFLAGS) -o Build/syscalls.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Src/system_stm32f4xx.c $(CFLAGS) -o Build/system_stm32f4xx.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -c Startup/startup_stm32f410rbtx.s $(CFLAGS) -o Build/startup_stm32f410rbtx.o
	$(CC) -T $(LDSCRIPT) Build/*.o -o Build/firmware.elf -lc -lm -lnosys -u _printf_float -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
	$(OBJCOPY) -O binary Build/firmware.elf Build/firmware.bin

debug:
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Src/main.c $(CFLAGS) -o Build/main.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Src/gpio.c $(CFLAGS) -o Build/gpio.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Src/irq.c $(CFLAGS) -o Build/irq.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Src/dma.c $(CFLAGS) -o Build/dma.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Src/i2c.c $(CFLAGS) -o Build/i2c.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Src/ssd1306.c $(CFLAGS) -o Build/ssd1306.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Src/syscalls.c $(CFLAGS) -o Build/syscalls.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Src/system_stm32f4xx.c $(CFLAGS) -o Build/system_stm32f4xx.o
	$(CC) -I Drivers/CMSIS -I Drivers/Device -I Inc -g -c Startup/startup_stm32f410rbtx.s $(CFLAGS) -o Build/startup_stm32f410rbtx.o
	$(CC) -T $(LDSCRIPT) Build/*.o -o Build/firmware.elf -lc -lm -lnosys -u _printf_float -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
	$(OBJCOPY) -O binary Build/firmware.elf Build/firmware.bin

flash:
	st-flash write ./Build/firmware.bin 0x8000000

clean:
	rm -f Build/*.o Build/*.elf Build/*.bin

.PHONY: build debug flash clean
