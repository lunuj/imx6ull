./bin/led.bin:./src/led.s
	arm-linux-gnueabihf-gcc -g -c ./src/led.s -o ./build/led.o
	arm-linux-gnueabihf-ld -Ttext 0X87800000 ./build/led.o -o ./build/led.elf
	arm-linux-gnueabihf-objcopy -O binary -S -g ./build/led.elf ./bin/led.bin
	arm-linux-gnueabihf-objdump -D ./build/led.elf > ./build/led.dis
clean:
	rm ./build/*

download:../imxdownload/imxdownload.out ./bin/led.bin
	../imxdownload/imxdownload.out -512MB ./bin/led.bin /dev/sdc

imxdownload.out:../imxdownload/imxdownload.c
	gcc -o ../imxdownload/imxdownload.out ../imxdownload/imxdownload.c
