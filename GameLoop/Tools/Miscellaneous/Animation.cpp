#include "Animation.hpp"

#include "../../GameData.hpp"
#include <fstream>
#include <sstream>

Animation::Animation(void)
{
	GameData* data = GameData::GetInstance();
	assets = data->assets;
}

void Animation::Create(sf::Sprite* _sprite, std::string _path, AnimType _type, std::string _animationName)
{
	sprite = _sprite;
	type = _type;
	isFinished = false;
	reversed = false;
	currentFrame = 0;
	timer = 0.f;

	texture = assets->GetTexture(_path + ".png");
	sprite->setTexture(*texture, true);

	std::string filePath = _path + "_Info.txt";

	if (LoadAnimationData(filePath, _animationName))
	{
		if (frames.size() > 0)
		{
			sf::IntRect rect = { frames[0].start.x, frames[0].start.y, frames[0].size.x, frames[0].size.y };
			sprite->setTextureRect(rect);
		}
	}
	else
	{
		std::cout << "Failed to load animation: " << _animationName << " from " << filePath << std::endl;
	}
}

void Animation::Create(sf::Sprite* _sprite, std::string _path, AnimType _type, int _selectedAnim)
{
	sprite = _sprite;
	type = _type;
	isFinished = false;
	reversed = false;
	currentFrame = 0;
	timer = 0.f;

	texture = assets->GetTexture(_path + ".png");
	sprite->setTexture(*texture, true);

	std::string filePath = _path + "_Info.txt";

	if (LoadAnimationData(filePath, _selectedAnim))
	{
		if (frames.size() > 0)
		{
			sf::IntRect rect = { frames[0].start.x, frames[0].start.y, frames[0].size.x, frames[0].size.y };
			sprite->setTextureRect(rect);
		}
	}
	else
	{
		std::cout << "Failed to load animation index: " << _selectedAnim << " from " << filePath << std::endl;
	}
}

bool Animation::LoadAnimationData(const std::string& _filePath, const std::string& _animationName)
{
	std::fstream file(_filePath, std::ios::in);

	if (!file.is_open())
	{
		std::cout << "Failed to open animation info file: " << _filePath << std::endl;
		return false;
	}

	frames.clear();
	maxFrame = 0;

	std::string line;
	bool foundAnimation = false;

	while (std::getline(file, line))
	{
		// Skip empty lines and comments
		if (line.empty() || line[0] == '#')
			continue;

		// Check for animation section
		if (line.find("[ANIMATION]") != std::string::npos)
		{
			// If we already found our animation, we're done
			if (foundAnimation)
				break;

			// Read the next lines to find NAME
			while (std::getline(file, line))
			{
				if (line.empty())
					continue;

				std::istringstream iss(line);
				std::string keyword;
				iss >> keyword;

				if (keyword == "NAME")
				{
					std::string animName;
					iss >> animName;

					if (animName == _animationName)
					{
						foundAnimation = true;
						// Continue reading this animation's data
						break;
					}
					else
					{
						// Not the animation we're looking for, skip to next [ANIMATION]
						break;
					}
				}
			}

			if (!foundAnimation)
				continue;
		}

		if (foundAnimation)
		{
			std::istringstream iss(line);
			std::string keyword;
			iss >> keyword;

			if (keyword == "START")
			{
				iss >> start.x >> start.y;
			}
			else if (keyword == "FRAMES")
			{
				iss >> maxFrame;
			}
			else if (keyword == "FRAME")
			{
				AnimFrame frame;
				iss >> frame.start.x >> frame.start.y >> frame.size.x >> frame.size.y >> frame.time;
				frames.push_back(frame);
			}
		}
	}

	file.close();
	return foundAnimation && frames.size() > 0;
}

bool Animation::LoadAnimationData(const std::string& _filePath, int _animationIndex)
{
	std::fstream file(_filePath, std::ios::in);

	if (!file.is_open())
	{
		std::cout << "Failed to open animation info file: " << _filePath << std::endl;
		return false;
	}

	frames.clear();
	maxFrame = 0;

	std::string line;
	int currentAnimIndex = -1;
	bool isReadingTargetAnim = false;

	while (std::getline(file, line))
	{
		// Skip empty lines and comments
		if (line.empty() || line[0] == '#')
			continue;

		// Check for animation section
		if (line.find("[ANIMATION]") != std::string::npos)
		{
			currentAnimIndex++;

			if (currentAnimIndex == _animationIndex)
			{
				isReadingTargetAnim = true;
			}
			else if (currentAnimIndex > _animationIndex)
			{
				// We've passed our target, stop reading
				break;
			}
			else
			{
				isReadingTargetAnim = false;
			}
			continue;
		}

		if (isReadingTargetAnim)
		{
			std::istringstream iss(line);
			std::string keyword;
			iss >> keyword;

			if (keyword == "NAME")
			{
				// Skip the name, we're using index
				continue;
			}
			else if (keyword == "START")
			{
				iss >> start.x >> start.y;
			}
			else if (keyword == "FRAMES")
			{
				iss >> maxFrame;
			}
			else if (keyword == "FRAME")
			{
				AnimFrame frame;
				iss >> frame.start.x >> frame.start.y >> frame.size.x >> frame.size.y >> frame.time;
				frames.push_back(frame);
			}
		}
	}

	file.close();
	return currentAnimIndex >= _animationIndex && frames.size() > 0;
}

void Animation::Update(float _dt)
{
	if (frames.size() == 0)
		return;

	if (isFinished)
		return;

	timer += _dt;
	int oldFrame = currentFrame;
	float delay = frames[currentFrame].time;

	if (timer >= delay)
	{
		switch (type)
		{
		case AnimType::LOOP:
			UpdateLoopAnim(_dt);
			break;
		case AnimType::LOOP_ONCE:
			UpdateLoopOnceAnim(_dt);
			break;
		case AnimType::BOOMERANG:
			UpdateBoomerangAnim(_dt);
			break;
		case AnimType::BOOMERANG_ONCE:
			UpdateBoomerangOnceAnim(_dt);
			break;
		default:
			break;
		}

		sf::IntRect newRect;
		newRect.left = frames[currentFrame].start.x;
		newRect.top = frames[currentFrame].start.y;
		newRect.width = frames[currentFrame].size.x;
		newRect.height = frames[currentFrame].size.y;

		sprite->setTextureRect(newRect);
		timer -= frames[oldFrame].time;
	}
}

void Animation::ForceFrame(int _frame)
{
	if (_frame < 0 || _frame >= (int)frames.size())
	{
		std::cout << "ForceFrame: Frame " << _frame << " is out of range (0-" << frames.size() - 1 << ")" << std::endl;
		return;
	}

	currentFrame = _frame;
	timer = 0.f;

	sf::IntRect newRect;
	newRect.left = frames[currentFrame].start.x;
	newRect.top = frames[currentFrame].start.y;
	newRect.width = frames[currentFrame].size.x;
	newRect.height = frames[currentFrame].size.y;

	sprite->setTextureRect(newRect);
}

void Animation::Reset(void)
{
	currentFrame = 0;
	timer = 0.f;

	isFinished = false;
	reversed = false;

	if (frames.size() > 0)
	{
		sprite->setTexture(*texture, true);
		sf::IntRect rect = { frames[0].start.x, frames[0].start.y, frames[0].size.x, frames[0].size.y };
		sprite->setTextureRect(rect);
	}
}

// Update Anim

void Animation::UpdateLoopAnim(float _dt)
{
	if (reversed)
	{
		currentFrame--;
	}
	else
	{
		currentFrame++;
	}

	if (currentFrame >= maxFrame)
	{
		currentFrame = 0;
	}
	else if (currentFrame < 0)
	{
		currentFrame = maxFrame - 1;
	}
}

void Animation::UpdateLoopOnceAnim(float _dt)
{
	if (reversed)
	{
		currentFrame--;
	}
	else
	{
		currentFrame++;
	}

	if (currentFrame >= maxFrame)
	{
		currentFrame = maxFrame - 1;
		isFinished = true;
	}
	else if (currentFrame < 0)
	{
		currentFrame = 0;
		isFinished = true;
	}
}

void Animation::UpdateBoomerangAnim(float _dt)
{
	if (reversed)
	{
		currentFrame--;
	}
	else
	{
		currentFrame++;
	}

	if (currentFrame >= maxFrame)
	{
		currentFrame = maxFrame - 1;
		reversed = true;
	}
	else if (currentFrame < 0)
	{
		currentFrame = 0;
		reversed = false;
	}
}

void Animation::UpdateBoomerangOnceAnim(float _dt)
{
	if (reversed)
	{
		currentFrame--;
	}
	else
	{
		currentFrame++;
	}

	if (currentFrame >= maxFrame)
	{
		currentFrame = maxFrame - 1;
		reversed = true;
	}
	else if (currentFrame < 0)
	{
		currentFrame = 0;
		isFinished = true;
	}
}