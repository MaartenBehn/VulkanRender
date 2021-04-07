CFLAGS = -std=c++17 -O0
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi
CFILES = $(wildcard */*.cpp) *cpp
HFILES = $(wildcard */*.hpp) *hpp

debug: CFLAGS += -g -DDEBUG
debug: build

release: build

build: $(CFILES) $(HFILES)
	g++ $(CFLAGS) -o VulkanTest $(CFILES) $(LDFLAGS)

run: release
	./VulkanTest

clean:
	rm -f VulkanTest