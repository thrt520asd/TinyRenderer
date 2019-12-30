SYSCONF_LINK = g++
CPPfLAGS = 
LDfLAGS = 
LIBS = -lm
DESTDIR = ./
TARGET = main
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
all: $(DESTDIR)$(TARGET)
$(DESTDIR)$(TARGET): $(OBJECTS)
		$(SYSCONF_LINK) -Wall $(LDfLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp
		$(SYSCONF_LINK) -Wall $(CPPfLAGS) -c $(CFLAGS) $< -o $@
clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f *.tga