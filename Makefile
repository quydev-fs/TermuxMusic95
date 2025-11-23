CXX      := g++
CXXFLAGS := -std=c++17 -Wall -O2 `pkg-config --cflags gtk+-3.0 gstreamer-1.0`
LDFLAGS  := `pkg-config --libs gtk+-3.0 gstreamer-1.0`

SRC_DIR := src
INC_DIR := include
OBJ_DIR := build/obj
BIN_DIR := build/bin

TARGET  := $(BIN_DIR)/TermuxMusic95
SRCS    := $(wildcard $(SRC_DIR)/*.cpp)
OBJS    := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: directories $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

clean:
	rm -rf build

.PHONY: all clean directories
