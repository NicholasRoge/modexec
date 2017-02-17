BUILD_DIR=./bin
BUILD_FILES=./src/Main.cpp
CFLAGS=--std=c++14 -fPIC -rdynamic -shared
TARGET=sayhello.so

sayhello:
	mkdir -p $(BUILD_DIR)
	g++ $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(BUILD_FILES)

clean:
	rm -rf $(BUILD_DIR)/$(TARGET)
