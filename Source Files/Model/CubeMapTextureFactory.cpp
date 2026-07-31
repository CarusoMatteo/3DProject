#include "../../Header Files/Model/CubeMapTextureFactory.h"
#include "../../Header Files/Model/Texture.h"
#include "../../Header Files/Model/TextureLoader.h"
#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace std;

static vector<string> operator+(const vector<string> &lhs, const vector<string> &rhs)
{
	assert(lhs.size() == rhs.size() && "Vectors must be of the same size to be concatenated!");

	vector<string> result;
	result.reserve(lhs.size());

	for (size_t i = 0; i < lhs.size(); ++i)
	{
		result.push_back(lhs[i] + rhs[i]);
	}

	return result;
}

// Paths to the texture files

const vector<string> CubeMapTextureFactory::CUBEMAP_TEXTURE_PATHS = vector<string>(6, "assets/skyboxes/");
const vector<string> CubeMapTextureFactory::BEACH_PATH = {"beach/posx.jpg", "beach/negx.jpg", "beach/posy.jpg", "beach/negy.jpg", "beach/posz.jpg", "beach/negz.jpg"};
const vector<string> CubeMapTextureFactory::CLOUDS_PATH = {"clouds/posx.jpg", "clouds/negx.jpg", "clouds/posy.jpg", "clouds/negy.jpg", "clouds/posz.jpg", "clouds/negz.jpg"};
const vector<string> CubeMapTextureFactory::DARK_PATH = {"dark/posx.jpg", "dark/negx.jpg", "dark/posy.jpg", "dark/negy.jpg", "dark/posz.jpg", "dark/negz.jpg"};
const vector<string> CubeMapTextureFactory::FIELD_PATH = {"field/posx.jpg", "field/negx.jpg", "field/posy.jpg", "field/negy.jpg", "field/posz.jpg", "field/negz.jpg"};
const vector<string> CubeMapTextureFactory::HEART_PATH = {"heart/posx.jpg", "heart/negx.jpg", "heart/posy.jpg", "heart/negy.jpg", "heart/posz.jpg", "heart/negz.jpg"};
const vector<string> CubeMapTextureFactory::ICEBERGS_PATH = {"icebergs/right.jpg", "icebergs/left.jpg", "icebergs/top.jpg", "icebergs/bottom.jpg", "icebergs/front.jpg", "icebergs/back.jpg"};
const vector<string> CubeMapTextureFactory::LOFT_PATH = {"loft/loft.hdr"};
const vector<string> CubeMapTextureFactory::MASKONAIVE_PATH = {"maskonaive/posx.jpg", "maskonaive/negx.jpg", "maskonaive/posy.jpg", "maskonaive/negy.jpg", "maskonaive/posz.jpg", "maskonaive/negz.jpg"};
const vector<string> CubeMapTextureFactory::STARS_PATH = {"stars/right.jpg", "stars/left.jpg", "stars/top.jpg", "stars/bottom.jpg", "stars/front.jpg", "stars/back.jpg"};

// Texture singletons

optional<shared_ptr<Texture>> CubeMapTextureFactory::beachTexture = nullopt;
optional<shared_ptr<Texture>> CubeMapTextureFactory::cloudsTexture = nullopt;
optional<shared_ptr<Texture>> CubeMapTextureFactory::darkTexture = nullopt;
optional<shared_ptr<Texture>> CubeMapTextureFactory::fieldTexture = nullopt;
optional<shared_ptr<Texture>> CubeMapTextureFactory::heartTexture = nullopt;
optional<shared_ptr<Texture>> CubeMapTextureFactory::icebergsTexture = nullopt;
optional<shared_ptr<Texture>> CubeMapTextureFactory::loftTexture = nullopt;
optional<shared_ptr<Texture>> CubeMapTextureFactory::maskonaiveTexture = nullopt;
optional<shared_ptr<Texture>> CubeMapTextureFactory::starsTexture = nullopt;

optional<shared_ptr<Texture>> CubeMapTextureFactory::none()
{
	return nullopt;
}

shared_ptr<Texture> CubeMapTextureFactory::beach()
{
	if (!beachTexture.has_value())
		beachTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + BEACH_PATH, false));
	return beachTexture.value();
}

shared_ptr<Texture> CubeMapTextureFactory::clouds()
{
	if (!cloudsTexture.has_value())
		cloudsTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + CLOUDS_PATH, false));
	return cloudsTexture.value();
}

shared_ptr<Texture> CubeMapTextureFactory::dark()
{
	if (!darkTexture.has_value())
		darkTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + DARK_PATH, false));
	return darkTexture.value();
}

shared_ptr<Texture> CubeMapTextureFactory::field()
{
	if (!fieldTexture.has_value())
		fieldTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + FIELD_PATH, false));
	return fieldTexture.value();
}

shared_ptr<Texture> CubeMapTextureFactory::heart()
{
	if (!heartTexture.has_value())
		heartTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + HEART_PATH, false));
	return heartTexture.value();
}

shared_ptr<Texture> CubeMapTextureFactory::icebergs()
{
	if (!icebergsTexture.has_value())
		icebergsTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + ICEBERGS_PATH, false));
	return icebergsTexture.value();
}

shared_ptr<Texture> CubeMapTextureFactory::loft()
{
	if (!loftTexture.has_value())
		loftTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + LOFT_PATH, false));
	return loftTexture.value();
}

shared_ptr<Texture> CubeMapTextureFactory::maskonaive()
{
	if (!maskonaiveTexture.has_value())
		maskonaiveTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + MASKONAIVE_PATH, false));
	return maskonaiveTexture.value();
}

shared_ptr<Texture> CubeMapTextureFactory::stars()
{
	if (!starsTexture.has_value())
		starsTexture = make_shared<Texture>(loadCubemap(CUBEMAP_TEXTURE_PATHS + STARS_PATH, false));
	return starsTexture.value();
}
