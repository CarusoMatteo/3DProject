#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace std;

struct Texture;

class TextureFactory
{
public:
	static optional<shared_ptr<Texture>> none();

	static shared_ptr<Texture> brick();
	static shared_ptr<Texture> cement();
	static shared_ptr<Texture> flag();
	static shared_ptr<Texture> mickey();
	static shared_ptr<Texture> pois();
	static shared_ptr<Texture> sea();
	static shared_ptr<Texture> steve();

private:
	// Instances of singletons for every texture
	static optional<shared_ptr<Texture>> brickTexture;
	static optional<shared_ptr<Texture>> cementTexture;
	static optional<shared_ptr<Texture>> flagTexture;
	static optional<shared_ptr<Texture>> mickeyTexture;
	static optional<shared_ptr<Texture>> poisTexture;
	static optional<shared_ptr<Texture>> seaTexture;
	static optional<shared_ptr<Texture>> steveTexture;

	// Paths to the texture files
	static const string TEXTURE_PATH;
	static const string BRICK_PATH;
	static const string CEMENT_PATH;
	static const string FLAG_PATH;
	static const string MICKEY_PATH;
	static const string POIS_PATH;
	static const string SEA_PATH;
	static const string STEVE_PATH;
};
