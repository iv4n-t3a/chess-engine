CXX := g++
CPPFLAGS := -g -std=c++20 -O2 -march=native
LIBS := -mbmi2 -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS := $(LIBS)
SRCEXT := cpp
HEADEREXT := h
OBJDIR := obj
SRCDIR := src
TARGET := CHESS

SOURCES := $(shell find  $(SRCDIR) -type f -name '*.$(SRCEXT)')
HEADERS := $(shell find  $(SRCDIR) -type f -name '*.$(HEADEREXT)')
OBJECTS := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SOURCES:.$(SRCEXT)=.o))


all: $(TARGET)

install: $(TARGET)
	@install $(TARGET) /bin/
	@cp CHESS.desktop /usr/share/applications/CHESS.desktop
	@mkdir -p /etc/CHESS/
	@cp -r etc/* /etc/CHESS/

uninstall:
	@rm -f /usr/share/applications/CHESS.desktop
	@rm -f /bin/$(TARGET)
	@rm -rf /bin/CHESS

clean:
	@echo "Cleaning..."
	rm -rf $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking" $(TARGET) "..."
	$(CXX) $(CPPFLAGS) $^ -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/attack.o: $(SRCDIR)/attack.cpp
	@echo "Compiling" $@ "..."
	@mkdir -p $(OBJDIR)
	$(CXX) $(CPPFLAGS) -fconstexpr-ops-limit=1000000000 -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Compiling" $@ "..."
	@mkdir -p $(OBJDIR)
	$(CXX) $(CPPFLAGS) -c -o $@ $<

.PHONY: all, clean, install, uninstall
