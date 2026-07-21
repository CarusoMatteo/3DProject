#pragma once

#include <string>
#include <vector>

using namespace std;

struct Texture
{
	unsigned int id;
};

enum TextureType
{
	CEMENT,
	SEA,
	ITALIAN_FLAG,
	BRICK_WALL,
	MICKEY_MOUSE,
	NO_TEXTURE,
	count
};

class TextureFactory
{
public:
	static const vector<string> names;

	static Texture loadTexture(const string path, bool shouldFlip);
	static Texture loadCubeMap(const vector<string> paths, bool shouldFlip);
};
