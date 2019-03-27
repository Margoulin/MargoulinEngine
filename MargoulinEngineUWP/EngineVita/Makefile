TARGET          := libEngineVita
SOURCES         := ../Engine/src ../Engine/OpenFBX ../Engine/cereal

INCLUDES = ../Engine/headers

CFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CXXFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.cpp))
OBJS     := $(CFILES:.c=.o) $(CXXFILES:.cpp=.o)

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CXX      = $(PREFIX)-g++
AR      = $(PREFIX)-ar
CFLAGS  = -g -Wl,-q -O3 -ffast-math -mtune=cortex-a9 -mfpu=neon -flto -DENABLE_LOG
CXXFLAGS  = -Wl,-q -Wall -std=c++11 -DVITA -I$(INCLUDES) -I$(VITASDK)/arm-vita-eabi/include/freetype2 -ffat-lto-objects -flto
ASFLAGS = $(CXXFLAGS)

all: $(TARGET).a

$(TARGET).a: $(OBJS)
	$(AR) -rc $@ $^

%.o : %.cpp
	arm-vita-eabi-g++ $(CXXFLAGS) -c -o $@ $<	
	
install: all
	
	
clean:
	@rm -rf $(TARGET).a $(TARGET).elf $(OBJS)
	
re: clean all

rei: re install