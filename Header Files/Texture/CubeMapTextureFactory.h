#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace std;

struct Texture;

class CubeMapTextureFactory
{
public:
	static optional<shared_ptr<Texture>> none();

	static shared_ptr<Texture> beach();
	static shared_ptr<Texture> clouds();
	static shared_ptr<Texture> dark();
	static shared_ptr<Texture> field();
	static shared_ptr<Texture> heart();
	static shared_ptr<Texture> icebergs();
	static shared_ptr<Texture> loft();
	static shared_ptr<Texture> maskonaive();
	static shared_ptr<Texture> stars();

private:
	// Instances of singletons for every texture
	static optional<shared_ptr<Texture>> beachTexture;
	static optional<shared_ptr<Texture>> cloudsTexture;
	static optional<shared_ptr<Texture>> darkTexture;
	static optional<shared_ptr<Texture>> fieldTexture;
	static optional<shared_ptr<Texture>> heartTexture;
	static optional<shared_ptr<Texture>> icebergsTexture;
	static optional<shared_ptr<Texture>> loftTexture;
	static optional<shared_ptr<Texture>> maskonaiveTexture;
	static optional<shared_ptr<Texture>> starsTexture;

	// Paths to the texture files
	static const vector<string> CUBEMAP_TEXTURE_PATHS;
	static const vector<string> BEACH_PATH;
	static const vector<string> CLOUDS_PATH;
	static const vector<string> DARK_PATH;
	static const vector<string> FIELD_PATH;
	static const vector<string> HEART_PATH;
	static const vector<string> ICEBERGS_PATH;
	static const vector<string> LOFT_PATH;
	static const vector<string> MASKONAIVE_PATH;
	static const vector<string> STARS_PATH;
};
