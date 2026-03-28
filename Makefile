all: voxel3d

voxel3d: src/main.cpp
	g++ -std=c++17 -O2 $(shell sdl2-config --cflags --libs) src/main.cpp -o voxel3d -lpthread

clean:
	rm -f voxel3d