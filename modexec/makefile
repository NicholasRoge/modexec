BUILD_DIR=./bin
BUILD_FILES=./src/Main.cpp ./src/Dll.cpp
CFLAGS=--std=c++14 -rdynamic -Wl,--no-as-needed
LIBS=-ldl
TARGET=modexec

all:
	mkdir -p $(BUILD_DIR)
	g++ $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/$(TARGET) $(BUILD_FILES)

clean:
	rm -rf $(BUILD_DIR)/$(TARGET)
