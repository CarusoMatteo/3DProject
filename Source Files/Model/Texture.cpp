#include "../../Header Files/Model/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/stb_image/stb_image.h"
#include <glad/glad.h>
#include <optional>
#include <stdexcept>
#include <string>

using namespace std;

#pragma region Singleton instances of textures

// Paths to the texture files

const string TextureFactory::TEXTURE_PATH = "assets/textures/";
const string TextureFactory::BRICK_PATH = TEXTURE_PATH + "brick.jpg";
const string TextureFactory::CEMENT_PATH = TEXTURE_PATH + "cement.jpg";
const string TextureFactory::FLAG_PATH = TEXTURE_PATH + "flag.jpg";
const string TextureFactory::MICKEY_PATH = TEXTURE_PATH + "mickey.jpg";
const string TextureFactory::POIS_PATH = TEXTURE_PATH + "pois.jpg";
const string TextureFactory::SEA_PATH = TEXTURE_PATH + "sea.jpg";
const string TextureFactory::STEVE_PATH = TEXTURE_PATH + "steve.jpg";

// Texture singletons

optional<shared_ptr<Texture>> TextureFactory::brickTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::cementTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::flagTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::mickeyTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::poisTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::seaTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::steveTexture = nullopt;

optional<shared_ptr<Texture>> TextureFactory::none()
{
	return nullopt;
}

shared_ptr<Texture> TextureFactory::brick()
{
	if (!brickTexture.has_value())
		brickTexture = make_shared<Texture>(loadTexture(BRICK_PATH, true));
	return brickTexture.value();
}

shared_ptr<Texture> TextureFactory::cement()
{
	if (!cementTexture.has_value())
		cementTexture = make_shared<Texture>(loadTexture(CEMENT_PATH, true));
	return cementTexture.value();
}

shared_ptr<Texture> TextureFactory::flag()
{
	if (!flagTexture.has_value())
		flagTexture = make_shared<Texture>(loadTexture(FLAG_PATH, true));
	return flagTexture.value();
}

shared_ptr<Texture> TextureFactory::mickey()
{
	if (!mickeyTexture.has_value())
		mickeyTexture = make_shared<Texture>(loadTexture(MICKEY_PATH, true));
	return mickeyTexture.value();
}

shared_ptr<Texture> TextureFactory::pois()
{
	if (!poisTexture.has_value())
		poisTexture = make_shared<Texture>(loadTexture(POIS_PATH, true));
	return poisTexture.value();
}

shared_ptr<Texture> TextureFactory::sea()
{
	if (!seaTexture.has_value())
		seaTexture = make_shared<Texture>(loadTexture(SEA_PATH, true));
	return seaTexture.value();
}

shared_ptr<Texture> TextureFactory::steve()
{
	if (!steveTexture.has_value())
		steveTexture = make_shared<Texture>(loadTexture(STEVE_PATH, true));
	return steveTexture.value();
}

#pragma endregion

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
