#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include "../Common.hpp"

class AssetsManager
{
public:
	AssetsManager(void);
	~AssetsManager(void) {};

	// Get a pointer of a texture
	sf::Texture* GetTexture(std::string _path);
	// Get a pointer of a sound buffer
	sf::SoundBuffer* GetSoundBuffer(std::string _path);
	// Get a pointer of a music
	sf::Music* GetMusic(std::string _path);

	void Destroy(void);
private:

	std::map<std::string, sf::Texture*>* textures;
	std::map<std::string, sf::SoundBuffer*>* sounds;
	std::map<std::string, sf::Music*>* musics;

private:
};

#endif // !ASSETS_MANAGER_H
