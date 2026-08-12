#include "../../Header Files/Texture/TextureFactory.h"
#include "../../Header Files/Texture/Texture.h"
#include "../../Header Files/Texture/TextureLoader.h"
#include <glad/glad.h>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

using namespace std;

// Paths to the texture files

const string TextureFactory::TEXTURE_PATH = "assets/textures/";
const string TextureFactory::BRICK_PATH = TEXTURE_PATH + "brick.jpg";
const string TextureFactory::CEMENT_PATH = TEXTURE_PATH + "cement.jpg";
const string TextureFactory::FLAG_PATH = TEXTURE_PATH + "flag.jpg";
const string TextureFactory::MICKEY_PATH = TEXTURE_PATH + "mickey.jpg";
const string TextureFactory::POLKA_DOT_PATH = TEXTURE_PATH + "polka_dot.jpg";
const string TextureFactory::SEA_PATH = TEXTURE_PATH + "sea.jpg";
const string TextureFactory::STEVE_PATH = TEXTURE_PATH + "steve.jpg";

// Texture singletons

optional<shared_ptr<Texture>> TextureFactory::brickTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::cementTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::flagTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::mickeyTexture = nullopt;
optional<shared_ptr<Texture>> TextureFactory::polkaDotTexture = nullopt;
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

shared_ptr<Texture> TextureFactory::polkaDot()
{
	if (!polkaDotTexture.has_value())
		polkaDotTexture = make_shared<Texture>(loadTexture(POLKA_DOT_PATH, true));
	return polkaDotTexture.value();
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

shared_ptr<Texture> TextureFactory::fromFile(const string path, const bool shouldFlip){
	return make_shared<Texture>(loadTexture(path, shouldFlip));
}
