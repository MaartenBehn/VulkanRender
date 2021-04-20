
SRC_DIR := ./src
SRC_FILES := $(shell find $(SRC_DIR) -type f -name \*.cpp)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS = -std=c++17  $(INC_FLAGS)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr

debug: CPPFLAGS += -O0 -g -DDEBUG
debug: clean VulkanTest

VulkanTest:
	g++ $(CPPFLAGS) -o VulkanTest $(SRC_FILES) $(LDFLAGS)

run: CPPFLAGS += -Ofast
run: clean VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest