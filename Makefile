all: voxel3d

voxel3d: src/main.cpp
	g++ -std=c++17 -O2 -lpthread src/main.cpp -o voxel3d

clean:
	rm -f voxel3d