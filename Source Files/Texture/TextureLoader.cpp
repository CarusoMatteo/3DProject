#include "../../Header Files/Texture/TextureLoader.h"
#include "../../Header Files/Texture/Texture.h"
#include <algorithm>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../lib/stb_image/stb_image_write.h"

using namespace std;
using namespace glm;

#define GRAYSCALE 1
#define GRAYSCALE_ALPHA 2
#define RGB 3
#define RGBA 4

Texture loadTexture(const string path, bool shouldFlip)
{
	Texture texture = {0, false};
	glGenTextures(1, &texture.id);

	int width, height, numberOfComponents;
	stbi_set_flip_vertically_on_load(shouldFlip);
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &numberOfComponents, 0);
	if (data)
	{
		GLenum format = 0;
		if (numberOfComponents == 1)
			format = GL_RED;
		else if (numberOfComponents == 3)
			format = GL_RGB;
		else if (numberOfComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture.id);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		cerr << "Texture failed to load at path: " << path << endl;
		throw runtime_error("Texture failed to load at path: " + path);
		stbi_image_free(data);
	}

	return texture;
}

Texture loadCubemap(vector<string> faces, bool shouldFlip)
{
	Texture texture = {0, true};
	glGenTextures(1, &texture.id);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		stbi_set_flip_vertically_on_load(shouldFlip);
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			cerr << "Cubemap texture failed to load at path: " << faces[i] << endl;
			throw runtime_error("Cubemap texture failed to load at path: " + faces[i]);
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture;
}

void saveTexture(ivec2 size, vector<float> pixelsFloat, const string filename)
{
	vector<unsigned char> pixels8(size.x * size.y * 4);
	for (size_t i = 0; i < pixelsFloat.size(); i++)
	{
		float v = std::clamp(pixelsFloat[i], 0.0f, 1.0f);
		pixels8[i] = static_cast<unsigned char>(v * 255.0f);
	}

	// Vertical flip
	vector<unsigned char> flipped(size.x * size.y * 4);
	int rowSize = size.x * 4;
	for (int y = 0; y < size.y; y++)
	{
		memcpy(&flipped[y * rowSize],
			   &pixels8[(size.y - 1 - y) * rowSize],
			   rowSize);
	}
	if (!stbi_write_bmp(filename.c_str(), size.x, size.y, RGBA, flipped.data()))
	{
		cerr << "stbi_write_bmp failed for image: " << filename << endl;
		throw runtime_error("stbi_write_bmp failed for image" + filename);
	}
	else
	{
		cout << "Saved image: \"" << filename << "\"" << endl;
	}
}
