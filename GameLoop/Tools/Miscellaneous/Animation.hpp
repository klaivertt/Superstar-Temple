#ifndef ANIMATION_H
#define ANIMATION_H

#include "../../GameLoop/GameData.hpp"

class AssetsManager;

typedef enum class AnimType
{
	LOOP,
	LOOP_ONCE,
	BOOMERANG,
	BOOMERANG_ONCE,

	NB_OF_DIFF_ANIM_TYPE // Keep last
}AnimType;

typedef enum class AnimDirection
{
	HORIZONTAL,
	VERTICAL
}AnimDirection;

struct AnimFrame
{
	sf::Vector2i start = { 0,0 };
	sf::Vector2i size = { 0,0 };
	float time = 0.f;
};

class Animation
{
public:
	// Constructor
	Animation(void);

	// For the path please remove the extension, this function will add the .png by herself
	// Use animation name (e.g., "Animation_0") or index
	// If you use the name, it will search for the animation with that name in the _Info.txt file
	void Create(sf::Sprite* _sprite, std::string _path, AnimType _type, std::string _animationName);
	// If you use the index, it will search for the animation at that index in the _Info.txt file (starting from 0)
	void Create(sf::Sprite* _sprite, std::string _path, AnimType _type, int _selectedAnim);

	void Update(float _dt);
	// This function allows you to force the animation to a specific frame, useful for syncing animations or jumping to a specific point
	void ForceFrame(int _frame);
	// Reset to the first frame and restart the timer
	void Reset(void);

	// Utility functions
	bool IsFinished() const { return isFinished; }
	int GetCurrentFrame() const { return currentFrame; }
	int GetMaxFrame() const { return maxFrame; }

private:
	void UpdateLoopAnim(float _dt);
	void UpdateLoopOnceAnim(float _dt);
	void UpdateBoomerangAnim(float _dt);
	void UpdateBoomerangOnceAnim(float _dt);

	bool LoadAnimationData(const std::string& _filePath, const std::string& _animationName);
	bool LoadAnimationData(const std::string& _filePath, int _animationIndex);

private:
	sf::Sprite* sprite = nullptr;
	sf::Texture* texture = nullptr;
	sf::Vector2i start;
	int currentFrame = 0;
	int maxFrame = 0;
	int frameRate = 0;
	float timer = 0.f;

	AssetsManager* assets = nullptr;

	std::vector<AnimFrame> frames;

	AnimDirection direction = AnimDirection::HORIZONTAL;
	AnimType type = AnimType::LOOP;
	bool isFinished = false;
	bool reversed = false;
};

#endif // !ANIMATION_H