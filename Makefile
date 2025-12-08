stub:
	@echo "dont call make by itself"

CMAKE_FILES := CMakeLists.txt src/CMakeLists.txt
SRC_FILES := src/main.cpp
GENERATOR := Ninja
BUILD_DIR := build
EXEC := BigEngine

$(BUILD_DIR)/build.ninja: $(CMAKE_FILES)
	@mkdir -p build
	@cmake -D GLFW_BUILD_X11=1 -D GLFW_BUILD_WAYLAND=0 DCMAKE_EXPORT_COMPILE_COMMANDS -S . -B $(BUILD_DIR) -G $(GENERATOR) 

$(BUILD_DIR)/src/$(EXEC) $(BUILD_DIR)/test/tests: $(SRC_FILES) $(BUILD_DIR)/build.ninja
	@cmake --build $(BUILD_DIR)

build_cmake: $(BUILD_DIR)/build.ninja

build: $(BUILD_DIR)/src/$(EXEC)

run: $(BUILD_DIR)/src/$(EXEC)
	@./$(BUILD_DIR)/src/$(EXEC)
	
test: $(BUILD_DIR)/test/tests
	@cd $(BUILD_DIR) && ctest

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: run build build_cmake
