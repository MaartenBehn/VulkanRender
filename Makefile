
SRC_DIR := ./src
OBJ_DIR := ./build
SRC_FILES := $(shell find $(SRC_DIR) -type f -name \*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS = -std=c++17  $(INC_FLAGS)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi

debug: CPPFLAGS += -O0 -g -DDEBUG
debug: clean VulkanTest

VulkanTest:
	g++ $(CPPFLAGS) -o VulkanTest $(SRC_FILES) $(LDFLAGS)

run: CPPFLAGS += -Ofast
run: clean VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest