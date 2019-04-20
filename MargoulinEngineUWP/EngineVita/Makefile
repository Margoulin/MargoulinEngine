TARGET          := libEngineVita
SOURCES         := ../Engine/src src ../Engine/OpenFBX ../Engine/cereal

SHADERS := shaders

INCLUDES := -I../Engine/headers  -I../Engine/external -I../EngineVita/headers

CFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CXXFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.cpp))
GXPFILES := $(foreach dir,$(SHADERS), $(wildcard $(dir)/*.gxp))
OBJS     := $(CFILES:.c=.o) $(CXXFILES:.cpp=.o) $(GXPFILES:.gxp=.o)

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CXX      = $(PREFIX)-g++
AR      = $(PREFIX)-ar
CFLAGS  = -g -Wl,-q -O3 -ffast-math -mtune=cortex-a9 -mfpu=neon -flto -DENABLE_LOG
CXXFLAGS  = -Wl,-q -Wall -std=c++11 -DVITA $(INCLUDES) -I$(VITASDK)/arm-vita-eabi/include/freetype2 -ffat-lto-objects -flto
ASFLAGS = $(CXXFLAGS)

all: $(TARGET).a

$(TARGET).a: $(OBJS)
	$(AR) -rc $@ $^

%.o : %.cpp
	arm-vita-eabi-g++ $(CXXFLAGS) -c -o $@ $<	
	
%.o : %.gxp
	$(PREFIX)-ld -r -b binary -o $@ $^
	
install: all
	@mkdir -p $(VITASDK)/$(PREFIX)/lib/
	cp $(TARGET).a $(VITASDK)/$(PREFIX)/lib/
	@mkdir -p $(VITASDK)/$(PREFIX)/include/
	@mkdir -p $(VITASDK)/$(PREFIX)/include/MargoulinEngine/
	cp ../Engine/headers/Engine.hpp $(VITASDK)/$(PREFIX)/include/MargoulinEngine/
	
clean:
	@rm -rf $(TARGET).a $(TARGET).elf $(OBJS)
	
re: clean all

rei: re install
