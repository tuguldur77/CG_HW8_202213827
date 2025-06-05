#pragma once
#include <string>
#include <vector>

struct Vector3 {
    float x, y, z;
};

struct Triangle {
    unsigned int indices[3];
};

extern std::vector<Vector3> gPositions;
extern std::vector<Vector3> gNormals;
extern std::vector<Triangle> gTriangles;

void load_mesh(std::string fileName);