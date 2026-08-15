#pragma once

#include <string>
#include <optional>

using namespace std;

struct ShaderFiles
{
	const string vertex;
	const string fragment;
	const optional<string> geometry = nullopt;
};
