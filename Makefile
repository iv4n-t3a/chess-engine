CXX := g++
CPPFLAGS := -g -std=c++20 -Ofast -flto -march=native -frename-registers -funroll-loops -openmp -ftree-parallelize-loops=16
LIBS := -mbmi2 -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS := $(LIBS)
SRCEXT := cpp
HEADEREXT := h
OBJDIR := obj
SRCDIR := src
TARGET := chess

SOURCES := $(shell find  $(SRCDIR) -type f -name '*.$(SRCEXT)')
HEADERS := $(shell find  $(SRCDIR) -type f -name '*.$(HEADEREXT)')
OBJECTS := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SOURCES:.$(SRCEXT)=.o))


all: $(TARGET)

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

.PHONY: all, clean
