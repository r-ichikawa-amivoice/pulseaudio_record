PROJECT	  = pulseaudio_record
COMPILER  = g++
CFLAGS    = 
LDFLAGS   = -lpulse
LIBS      = 
INCLUDE   = -I../src/
TARGETDIR = ../bin/
ifeq "$(strip $(TARGETDIR))" ""
  TARGETDIR  = .
endif
TARGET    = $(PROJECT)
SRCDIR    = ../src/
ifeq "$(strip $(SRCDIR))" ""
  SRCDIR  = .
endif
SOURCES   = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR    = ../obj/
ifeq "$(strip $(OBJDIR))" ""
  OBJDIR  = .
endif
OBJECTS   = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
DEPENDS   = $(OBJECTS:.o=.d)

$(TARGETDIR)$(TARGET): $(OBJECTS) $(LIBS)
	-mkdir -p $(TARGETDIR)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

all: clean $(TARGET)

clean:
	-rm -rf $(OBJDIR) $(TARGETDIR)

-include $(DEPENDS)


