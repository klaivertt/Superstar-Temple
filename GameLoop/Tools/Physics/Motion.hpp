#pragma once
#ifndef MOTION_HPP
#define MOTION_HPP

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <array>
#include <variant>
#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include "Math.h"

// Motion Path
//#include "../ProjetSFML/Code/Tools/Motion/Motion.hpp"

enum class MotionAroundType
{
	SHAKE,
	LEMNISCATE,
	CIRCULAR,
	ELLIPSE_OSCILLATE,
	HORIZONTAL_WAVE,
	VERTICAL_WAVE,
	COUNT
};

enum class MotionType
{
	LINEAR,
	START_SMOOTH,
	END_SMOOTH,
	FULL_SMOOTH,
	START_BOUNCE,
	END_BOUNCE,
	FULL_BOUNCE,
	START_ELASTIC,
	END_ELASTIC,
	FULL_ELASTIC,
	START_BACK,
	END_BACK,
	FULL_BACK,
	COUNT
};

class ValueMotion
{
public:
	ValueMotion(float _startValue, float _targetValue, float _duration, MotionType _motionType, bool _loop = false, float _timeOffset = 0.f);
	//return updated value
	float UpdateValue(float _dt);
	bool UpdateFinished(void);
	void Reset(void);

	//Setters
	void ChangeStartValue(float _value) { startValue = _value; }
	void ChangeTargetValue(float _value) { targetValue = _value; }
	void ChangeDuration(float _duration) { duration = _duration >= 0 ? _duration : 0.0f; }
	void ChangeTimeElapsed(float _time) { timeElapsed = _time; }

	// Getters
	float GetTimeElapsed() const { return timeElapsed; }
	float GetDuration() const { return duration; }
	float GetStartValue() const { return startValue; }
	// Returns value between 0.0f and 1.0f
	float GetProgression() const { return (timeElapsed / duration); }
	float GetValue() { return UpdateValue(0.0f); }

private:
	MotionType motionType = MotionType::LINEAR;
	bool loop = false;
	float timeElapsed = 0.0f;
	float targetValue = 0.0f;
	float duration = 0.0f;

	float startValue = 0.0f;
	float timeOffset = 0.0f;
};


class MotionAround
{
public:
	MotionAround(sf::Vector2f _centerPoint, sf::Vector2f _amplitude, float _duration, MotionAroundType _motionAroundType, MotionType _motionType = MotionType::LINEAR, bool _loop = true);
	// Return updated position
	sf::Vector2f UpdateValue(float _dt);
	void Reset(void);

	// Setters
	void SetCenterPoint(sf::Vector2f _centerPoint) { centerPoint = _centerPoint; }
	void SetAmplitude(sf::Vector2f _amplitude) { amplitude = _amplitude; }
	void SetDuration(float _duration) { duration = _duration >= 0 ? _duration : 0.0f; }

	// Getters
	sf::Vector2f GetCenterPoint() const { return centerPoint; }
	sf::Vector2f GetAmplitude() const { return amplitude; }
	float GetDuration() const { return duration < 0 ? -duration : duration; }
	bool IsLooping() const { return loop; }
	bool UpdateFinished(void) const { return timeElapsed >= duration && !loop; }

private:
	sf::Vector2f MoveShapeAroundPos_VerticalWave(float _dt);
	sf::Vector2f MoveShapeAroundPos_HorizontalWave(float _dt);
	sf::Vector2f MoveShapeAroundPos_EllipseOscillate(float _dt);
	sf::Vector2f MoveShapeAroundPos_Circle(float _dt);
	sf::Vector2f MoveShapeAroundPos_Lemniscate(float _dt);
	sf::Vector2f MoveShapeAroundPos_Shake(float _dt);

private:
	MotionAroundType motionAroundType = MotionAroundType::SHAKE;
	MotionType motionType = MotionType::LINEAR;
	sf::Vector2f centerPoint = { 0.f, 0.f };
	sf::Vector2f amplitude = { 0.f, 0.f };
	float duration = 0.0f;
	bool loop = true;
	float timeElapsed = 0.0f;

	ValueMotion motion[2] = {
		ValueMotion(0.f, 1.f, 1.f, MotionType::LINEAR, true),
		ValueMotion(0.f, 1.f, 1.f, MotionType::LINEAR, true)
	};
	int count = 0;
};

float ConvergeValue(float _currentValue, float _targetValue, float _valuePerSec, float _dt);
sf::Vector2f ConvergeVector2(sf::Vector2f _currentValue, sf::Vector2f _targetValue, sf::Vector2f _valuePerSec, float _dt);

#endif