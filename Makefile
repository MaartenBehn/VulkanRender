CFLAGS = -std=c++17 -O0
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi

debug: CFLAGS += -g -DDEBUG
debug: build

release: build

build: *.cpp *.hpp
	g++ $(CFLAGS) -o VulkanTest *.cpp $(LDFLAGS)

run: release
	./VulkanTest

clean:
	rm -f VulkanTest