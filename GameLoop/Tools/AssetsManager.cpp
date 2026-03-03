#include "AssetsManager.hpp"

AssetsManager::AssetsManager(void)
{
	textures = new std::map<std::string, sf::Texture*>;
	sounds = new std::map<std::string, sf::SoundBuffer*>;
	musics = new std::map<std::string, sf::Music*>;
}

sf::Texture* AssetsManager::GetTexture(std::string _path)
{
	if ((*textures)[_path] != nullptr)
	{
		return (*textures)[_path];
	}

	sf::Texture* texture = new sf::Texture;
	if (texture->loadFromFile(_path))
	{
		//texture->setSmooth(true);
		texture->setRepeated(true);
		(*textures)[_path] = texture;
		return (*textures)[_path];
	}
	else
	{
		if ((*textures)["Assets/Missing.png"] != nullptr)
		{
			return (*textures)["Assets/Missing.png"];
		}
		else
		{
			sf::Texture* texture = new sf::Texture;
			texture->loadFromFile("Assets/Missing.png");
			texture->setSmooth(true);
			texture->setRepeated(true);
			(*textures)["Assets/Missing.png"] = texture;
			return (*textures)["Assets/Missing.png"];
		}
	}

	return nullptr;
}

sf::SoundBuffer* AssetsManager::GetSoundBuffer(std::string _path)
{
	if ((*sounds)[_path] != nullptr)
	{
		return (*sounds)[_path];
	}

	sf::SoundBuffer* sound = new sf::SoundBuffer;
	if (sound->loadFromFile(_path))
	{
		(*sounds)[_path] = sound;
		return (*sounds)[_path];
	}

	return nullptr;
}

sf::Music* AssetsManager::GetMusic(std::string _path)
{
	if ((*musics)[_path] != nullptr)
	{
		return (*musics)[_path];
	}

	sf::Music* music = new sf::Music;
	if (music->openFromFile(_path))
	{
		(*musics)[_path] = music;
		return (*musics)[_path];
	}

	return nullptr;
}

void AssetsManager::Destroy(void)
{
	std::map<std::string, sf::Texture*>::iterator text;
	for (text = textures->begin(); text != textures->end(); text++)
	{
		delete text->second;
	}
	textures->clear();

	std::map<std::string, sf::SoundBuffer*>::iterator sound;
	for (sound = sounds->begin(); sound != sounds->end(); sound++)
	{
		delete sound->second;
	}
	sounds->clear();

	std::map<std::string, sf::Music*>::iterator music;
	for (music = musics->begin(); music != musics->end(); music++)
	{
		delete music->second;
	}
	musics->clear();
}
