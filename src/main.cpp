#include <iostream>
#include "input.hpp"
#include "parser.hpp"
#include <utility>

int main() {
  std::pair<std::string, int> userData = inputObj();

  std::string pathFile = userData.first;
  int depth = userData.second;

  Mesh mesh = validate_obj(pathFile);

  AABB initialSquare = computeAABB(mesh);
  AABB finalSquare = expandToCube(initialSquare);

  std::cout << "Jumlah Vertex : " << mesh.vertices.size() << std::endl;
  std::cout << "Jumlah Triangle : " << mesh.triangles.size() << std::endl;
  std::cout << "Bounding Box Terkecil : " << finalSquare.min.x << ", "
            << finalSquare.min.y << ", " << finalSquare.min.z << ", "
            << finalSquare.max.x << ", " << finalSquare.max.y << ", "
            << finalSquare.max.z << std::endl;
  return 0;
}