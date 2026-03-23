all: voxel3d

voxel3d: src/main.cpp
	g++ src/main.cpp -o voxel3d

clean:
	rm -f voxel3d