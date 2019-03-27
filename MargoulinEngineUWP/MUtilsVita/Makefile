TARGET          := libMUtilsVita
SOURCES         := ../MUtils/MUtils ../MUtils/MUtils/Maths

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
	@mkdir -p $(VITASDK)/$(PREFIX)/lib/
	cp $(TARGET).a $(VITASDK)/$(PREFIX)/lib/
	@mkdir -p $(VITASDK)/$(PREFIX)/include/
	@mkdir -p $(VITASDK)/$(PREFIX)/include/MUtils/
	@mkdir -p $(VITASDK)/$(PREFIX)/include/MUtils/Maths
	cp ../MUtils/MUtils/String.hpp $(VITASDK)/$(PREFIX)/include/MUtils/
	cp ../MUtils/MUtils/Maths/Math.hpp $(VITASDK)/$(PREFIX)/include/MUtils/Maths/
	cp ../MUtils/MUtils/Maths/Matrix.hpp $(VITASDK)/$(PREFIX)/include/MUtils/Maths/
	cp ../MUtils/MUtils/Maths/Quaternion.hpp $(VITASDK)/$(PREFIX)/include/MUtils/Maths/
	cp ../MUtils/MUtils/Maths/Transform.hpp $(VITASDK)/$(PREFIX)/include/MUtils/Maths/
	cp ../MUtils/MUtils/Maths/Vector.hpp $(VITASDK)/$(PREFIX)/include/MUtils/Maths/
	
clean:
	@rm -rf $(TARGET).a $(TARGET).elf $(OBJS)
	
re: clean all

rei: re install