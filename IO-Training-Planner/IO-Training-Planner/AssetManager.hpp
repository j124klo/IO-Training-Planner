#pragma once

#include <map>
#include <SFML/Graphics.hpp>

using namespace std;

class AssetManager
{
private:
	map<string, sf::Texture> _textures;
	map<string, sf::Font> _fonts;

public:
	AssetManager() {}
	~AssetManager() {}

	void loadTexture(const string& name, const string& fileName);
	sf::Texture& getTexture(const string& name);

	void loadFont(const string& name, const string& fileName);
	sf::Font& getFont(const string& name);
};