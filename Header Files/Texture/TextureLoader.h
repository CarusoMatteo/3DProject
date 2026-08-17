#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

struct Texture;

Texture loadTexture(const string path, bool shouldFlip);

Texture loadCubemap(vector<string> faces, bool shouldFlip);

void saveTexture(ivec2 size, vector<float> pixelsFloat, const string filename);
