#pragma once

#include <string>
#include <vector>

using namespace std;

struct Texture;

Texture loadTexture(const string path, bool shouldFlip);

Texture loadCubemap(vector<string> faces, bool shouldFlip);
