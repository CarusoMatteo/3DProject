#pragma once

#include <string>
#include <vector>

using namespace std;

struct Texture
{
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
};
