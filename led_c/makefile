objs = ./build/start.o ./build/main.o 
level =
inc = -Iinc
./bin/ledc.bin:$(objs)
	arm-linux-gnueabihf-ld -Ttext 0x87800000 $^ -o ./build/ledc.elf
	arm-linux-gnueabihf-objcopy -O binary -S ./build/ledc.elf $@
	arm-linux-gnueabihf-objdump -D -m arm ./build/ledc.elf > ./build/ledc.dis

build/%.o: ./src/%.c
	arm-linux-gnueabihf-gcc -Wall $(inc) -nostdlib -c $(level) -o $@ $<
build/%.o: ./src/%.s
	arm-linux-gnueabihf-gcc -Wall $(inc) -nostdlib -c $(level) -o $@ $<

clean:
	rm ./build/*

download:../imxdownload/imxdownload.out ./bin/ledc.bin
	../imxdownload/imxdownload.out -512MB ./bin/ledc.bin /dev/sdc

imxdownload.out:../imxdownload/imxdownload.c
	gcc -o ../imxdownload/imxdownload.out ../imxdownload/imxdownload.c
