#include "../../Header Files/Model/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/stb_image/stb_image.h"
#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

const vector<string> TextureFactory::names = {
	"Cement",
	"Sea",
	"Italian flag",
	"Brick wall",
	"Mickey Mouse",
	"No texture"};

Texture TextureFactory::loadTexture(const string path, bool shouldFlip)
{
	Texture texture = {0};
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
		throw runtime_error("Texture failed to load at path: " + path);
		stbi_image_free(data);
	}

	return texture;
}

Texture TextureFactory::loadCubeMap(const vector<string> facesPaths, bool shouldFlip)
{
	Texture texture = {0};
	glGenTextures(1, &texture.id);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < facesPaths.size(); i++)
	{
		stbi_set_flip_vertically_on_load(shouldFlip);
		unsigned char *data = stbi_load(facesPaths.at(i).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			throw runtime_error("Cubemap texture failed to load at path: " + facesPaths.at(i));
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
