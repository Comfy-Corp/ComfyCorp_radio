TARGET	= ipac

# Application source en include includes
SRC_DIR	= ./source
INC_DIR = ./include

# NutOS location (includes and libs)
NUT_INC = /Users/guube/ethernut/nut/include
NUT_BUILD_INC = /Users/guube/ethernut/build/gcc/atmega2561/lib/include
NUT_LIB_DIR = /Users/guube/ethernut/build/gcc/atmega2561/lib

# WinAvr includes
AVR_INC = /Users/avr/include


# Compiler, assembler & linker (flags)
CC		= 	avr-gcc
CFLAGS	= 	-mmcu=atmega2561 -Os -Wall -Wstrict-prototypes -DNUT_CPU_FREQ=14745600 \
			-D__HARVARD_ARCH__ -DNUTOS_VERSION=433 \
			-Wa,-ahlms=$(SRC_DIR)/$*lst
ASFLAGS = 	-mmcu=atmega2561 -I. -x assembler-with-cpp -Wa,-ahlms=$(SRC_DIR)/$*lst,-gstabs 
LDFLAGS	=	-mmcu=atmega2561 -Wl,--defsym=main=0,-Map=TIStreamer.map,--cref


# =================================================================================
# Source files
CFILES =        main.c			\
				ui.c 			\
				uart0driver.c	\
				player.c	    \
				log.c			\
                led.c			\
				keyboard.c		\
				lcd.c		    \
                vs10xx.c		\
                remcon.c		\
                watchdog.c		\
				mmc.c			\
				spidrv.c        \
                mmcdrv.c        \
                fat.c			\
				rtc.c 			\
				userInput.c		\
				dispatcher.c 	\
				ethernet.c 		\
				storage.c 		\
				bankmem.c 		\
				heap.c 			\
				thread.c 		\
				alarmControl.c	\
				customChars.c	\
				speakingClock.c	\
				weatherReport.c	
				
# Header files.
HFILES =        keyboard.h      \
				ui.h 			\
				userInput.h     \
				player.h 		\
				ethernet.h 		\
                led.h           \
                portio.h 		\
                remcon.h 		\
                log.h 			\
                system.h 		\
                settings.h     	\
                inet.h         	\
                platform.h 		\
                version.h 		\
                update.h       	\
                uart0driver.h 	\
                typedefs.h     	\
                vs10xx.h 		\
                audio.h        	\
                watchdog.h 		\
                mmc.h           \
                flash.h     	\
                spidrv.h    	\
                command.h      	\
                parse.h        	\
                mmcdrv.h      	\
                fat.h          	\
                fatdrv.h		\
                rtc.h 			\
				dispatcher.h 	\
				storage.h 		\
				bankmem.h 		\
				heap.h 			\
				os.h 			\
				thread.h 		\
				alarmControl.h	\
				customChars.h 	\
				weatherReport.h \
				speakingClock.h 
				

# Alle source files in de ./source dir
SRCS =	$(addprefix $(SRC_DIR)/,$(CFILES))
OBJS = 	$(SRCS:.c=.o)

NUT_LIBS = $(NUT_LIB_DIR)/nutinit.o -lnutpro -lnutnet -lnutpro -lnutfs -lnutos -lnutdev -lnutarch -lnutnet -lnutcrt -lnutdev


# Alle includes (header files) in de ./header dir
INCS =	$(addprefix $(INC_DIR)/,$(HFILES))

# Linking rule. All *.o to elf file. Then convert to *.hex
$(TARGET):	$(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -L$(NUT_LIB_DIR) $(NUT_LIBS) -o $@.elf
	avr-objcopy -O ihex $@.elf $@.hex
#	hex2bin -ebin $@.hex

# Compiling the objs's. avr-gcc autocalls assembler	
$(SRC_DIR)/%o:	$(SRC_DIR)/%c 
	$(CC) -c $< $(CFLAGS) -I$(INC_DIR) -I$(NUT_INC) -I$(AVR_INC) -o $@

	
all: $(TARGET)

debug:
	
	@echo $(OBJS)


.PHONY: clean
clean:
	-rm -f $(OBJS)
	-rm -f $(SRCS:.c=.lst)
	-rm -f *.hex *.elf *.map *.bin


