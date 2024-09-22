CROSS_COMPILE	?= arm-linux-gnueabihf-
TARGET			?= bin/bsp
OUTDIR			?= build/

CC				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY			:= $(CROSS_COMPILE)objcopy 
OBJDUMP			:= $(CROSS_COMPILE)objdump 
LEVEL			:=

LDFILE			:= imx6ull.lds
LDFLAGS += -Map=$(TARGET).map -T$(LDFILE)

INCDIRS        	:= 	lib/imx6ull/inc		\
                	lib/bsp/inc			\
                    project/inc			\

SRCDIRS         := 	lib/imx6ull/src		\
                    lib/bsp/src			\
                    project/src			\

DOWNLOADTOOL	:= ../imxdownload/imxdownload.out
DOWNLOADTOOLSRC := ../imxdownload/imxdownload.c

DEVSD			:= /dev/sdb
# 利用patsubst进行字符串修改，在INCLUDIRS每个路径前加-I \
$(patsubst <pattern>,<replacement>,<text>) \
名称:模式字符串替换函数——patsubst。\
功能:查找<text>中的单词(单词以“空格”、“Tab”或“回车”“换行”分隔)是否符\
合模式<pattern>,如果匹配的话,则以<replacement>替换。这里,<pattern>可以包括通\
配符“%”,表示任意长度的字串。如果<replacement>中也包含“%”,那么,<replacement>\
中的这个“%”将是<pattern>中的那个“%”所代表的字串。\
(可以用“\”来转义,以“\%”来表示真实含义的“%”字符)\
返回:函数返回被替换过后的字符串
INCLUDE				:=$(patsubst %, -I % ,$(INCDIRS))

OUTSRCDIRS			:= $(patsubst %, build/%,$(SRCDIRS))
#获取项目中.c、.s文件 \
foreach 类似BASH里的for循环，用法是 $(foreach <var>,<list>,<text>)    \
就是把list里的单词逐一取出放在变量var中，然后执行text的表达式内，每次text会返回一个字符串。            \
最后循环结束时，<text>返回值为每个字符串所组成的整个字符串，之间以空格分格，                \
wildcard为展开指定对象下文件集合

#获取带路径的文件名
SFILES			:= $(foreach dir ,$(SRCDIRS),$(wildcard $(dir)/*.s))
CFILES			:= $(foreach dir ,$(SRCDIRS),$(wildcard $(dir)/*.c))

#获取不带路径的文件名，用来修改后缀生成.o文件的文件名
SFILENDIR       := $(notdir $(SFILES))        # 不带路径的文件名,
CFILENDIR       := $(notdir $(CFILES))        # 不带路径的文件名

# 将不带路径的文件名转换成.o后缀名，对应编译后文件名
SOBJS            := $(patsubst %, build/%,$(SFILES:.s=.o))    #.s=.o是把.s用.o替换
COBJS            := $(patsubst %, build/%,$(CFILES:.c=.o))      #.c=.o是把.c用.o替换

OBJS             := $(SOBJS) $(COBJS)
VPATH            := $(SRCDIRS)            

$(TARGET): $(OUTSRCDIRS) $(TARGET).bin

#将所有依赖文件链接，生成.elf文件
#将elf转换为依赖的目标集合
#将elf文件反汇编
$(TARGET).bin : $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET).elf $^
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis


# 静态模式 <Targets...>:<tatgets-pattern>:<prereq-patterns...>下面两天为自写
# 将所有的.s文件编译成.o文件放在obj文件夹内
$(SOBJS):		$(OUTDIR)%.o:		%.s
	$(CC) -Wall -nostdlib -c ${LEVEL} $(INCLUDE) -o $@ $<

$(COBJS):		$(OUTDIR)%.o:		%.c
	$(CC) -Wall -nostdlib -c ${LEVEL}  $(INCLUDE) -o $@ $<

$(OUTSRCDIRS):
	mkdir -p $@

.PHONY: download
download: $(DOWNLOADTOOL) $(TARGET).bin
	$(DOWNLOADTOOL) -512MB $(TARGET).bin $(DEVSD)

$(DOWNLOADTOOL):	$(DOWNLOADTOOLSRC) 
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -rf build/*
	rm -rf $(TARGET).elf $(TARGET).bin $(TARGET).dis $(OBJS)

.PHONY: print
print:
	@echo INCLUDE = $(INCLUDE)
	@echo OBJS = $(OBJS)
	@echo $(TARGET).bin
	ls /dev/sd*