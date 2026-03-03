#include "Motion.hpp"

#pragma region ValueMotion

#pragma region Smooth
float StartSmoothMovement(float x)
{
	return x * x * x;
}
float EndSmoothMovement(float x)
{
	return 1.0f - powf(1.0f - x, 3.0f);
}
float FullSmoothMovement(float x)
{
	return (x < 0.5f) ? 4.0f * x * x * x : 1.0f - powf(-2.0f * x + 2.0f, 3.0f) / 2.0f;
}
#pragma endregion

#pragma region Bounce

float EndBounceMovement(float x) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1.0f / d1)
	{
		return n1 * x * x;
	}
	else if (x < 2.0f / d1)
	{
		x -= 1.5f / d1;
		return n1 * x * x + 0.75f;
	}
	else if (x < 2.5f / d1)
	{
		x -= 2.25f / d1;
		return n1 * x * x + 0.9375f;
	}
	else
	{
		x -= 2.625f / d1;
		return n1 * x * x + 0.984375f;
	}
}

float StartBounceMovement(float x)
{
	return 1 - EndBounceMovement(1 - x);
}

float FullBounceMovement(float x)
{
	if (x < 0.5f) {
		return (1.0f - EndBounceMovement(1.0f - 2.0f * x)) / 2.0f;
	}
	else {
		return (1.0f + EndBounceMovement(2.0f * x - 1.0f)) / 2.0f;
	}
}
#pragma endregion

#pragma region Elastic
float StartElasticMovement(float x)
{
	const float c4 = (const float)(2 *  (float)M_PI) / 3;  // Constante c4

	if (x == 0.0f) {
		return 0.0f;  // Si x est 0, renvoyer 0
	}
	else if (x == 1.0f) {
		return 1.0f;  // Si x est 1, renvoyer 1
	}
	else {
		return -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * c4);
	}
}

float EndElasticMovement(float x)
{

	const float c4 = (const float)(2 *  (float)M_PI) / 3;

	if (x == 0.0f)
	{
		return 0.0f;
	}
	else if (x == 1.0f)
	{
		return 1.0f;
	}
	else
	{
		return powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
	}
}

float FullElasticMovement(float x)
{
	const float c5 = (const float)(2.0f *  (float)M_PI) / 4.5f;

	if (x == 0.0f) {
		return 0.0f;
	}
	else if (x == 1.0f) {
		return 1.0f;
	}
	else if (x < 0.5f) {
		return -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f;
	}
	else {
		return (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
	}
}
#pragma endregion

#pragma region Back
float StartBackMovement(float x)
{
	const float s = 1.70158f;
	float back = x * x * ((s + 1) * x - s);
	float blend = x * x * (3 - 2 * x); // smoothstep de x

	return (1.0f - blend) * back + blend * x;
}

float EndBackMovement(float x)
{
	const float s = 1.70158f;
	return 1 + (--x) * x * ((s + 1) * x + s);
}

float FullBackMovement(float x)
{
	const float s = 1.70158f * 1.525f;
	return (x < 0.5f)
		? (powf(2 * x, 2) * ((s + 1) * 2 * x - s)) / 2
		: (powf(2 * x - 2, 2) * ((s + 1) * (x * 2 - 2) + s) + 2) / 2;
}
#pragma endregion

float GetMotionValue(MotionType _motionType, float _t)
{

	switch (_motionType)
	{
	case MotionType::LINEAR:
		return _t;
	case MotionType::START_SMOOTH:
		return StartSmoothMovement(_t);
	case MotionType::END_SMOOTH:
		return EndSmoothMovement(_t);
	case MotionType::FULL_SMOOTH:
		return FullSmoothMovement(_t);
	case MotionType::START_BOUNCE:
		return StartBounceMovement(_t);
	case MotionType::END_BOUNCE:
		return EndBounceMovement(_t);
	case MotionType::FULL_BOUNCE:
		return FullBounceMovement(_t);
	case MotionType::START_ELASTIC:
		return StartElasticMovement(_t);
	case MotionType::END_ELASTIC:
		return EndElasticMovement(_t);
	case MotionType::FULL_ELASTIC:
		return FullElasticMovement(_t);
	case MotionType::START_BACK:
		return StartBackMovement(_t);
	case MotionType::END_BACK:
		return EndBackMovement(_t);
	case MotionType::FULL_BACK:
		return FullBackMovement(_t);
	default:
		return 0;
	}
}

#pragma endregion

#pragma region ValueMotion Class

ValueMotion::ValueMotion(float _startValue, float _targetValue, float _duration, MotionType _motionType, bool _loop, float _timeOffset)
{
	startValue = _startValue;
	targetValue = _targetValue;
	duration = _duration;
	motionType = _motionType;
	loop = _loop;
	timeOffset = _timeOffset;
	timeElapsed = timeOffset;
}

float ValueMotion::UpdateValue(float _dt)
{
	timeElapsed += _dt;
	float t = timeElapsed / duration;

	if (duration <= 0 || t >= 1.0f)
	{
		if (!loop)
		{
			return targetValue;
		}
		else
		{
			int iteration = (int)t;

			if (iteration % 2 == 0)
			{
				t -= (float)(iteration);
			}
			else
			{
				t = 1.0f - (t - (float)(iteration));
			}
		}
	}

	float usedT = GetMotionValue(motionType, t);

	return startValue + (targetValue - startValue) * usedT;
}

bool ValueMotion::UpdateFinished(void)
{
	return timeElapsed >= duration;
}

void ValueMotion::Reset(void)
{
	timeElapsed = timeOffset;
}

#pragma endregion


#pragma region MotionAround

sf::Vector2f MotionAround::MoveShapeAroundPos_Shake(float _dt)
{
	sf::Vector2f position = { centerPoint.x, centerPoint.y };

	if (count <= 0)
	{
		sf::Vector2f randomPoint = {
			((float)rand() / RAND_MAX) * 2.f - 1.f,
			((float)rand() / RAND_MAX) * 2.f - 1.f
		};

		motion[0] = ValueMotion(motion[0].GetValue(), randomPoint.x * amplitude.x, duration, motionType, false);
		motion[1] = ValueMotion(motion[1].GetValue(), randomPoint.y * amplitude.y, duration, motionType, false);

		count++;
	}

	for (int i = 0; i < 2; i++)
	{
		motion[i].ChangeDuration(duration);
	}

	position.x = motion[0].UpdateValue(_dt);
	position.y = motion[1].UpdateValue(_dt);

	if (motion[0].UpdateFinished() && motion[1].UpdateFinished())
	{
		count = 0;
	}

	return position;
}

sf::Vector2f MotionAround::MoveShapeAroundPos_Lemniscate(float _dt)
{
	float deltaAngle = (2.f * (float)M_PI / duration) * _dt;

	if (timeElapsed >= 2 * (float)M_PI)
	{
		timeElapsed -= 2.f * (float)M_PI;
	}
	else if (timeElapsed < 0)
	{
		timeElapsed += 2.f * (float)M_PI;
	}

	timeElapsed += deltaAngle;

	float normalizedT = fmodf(timeElapsed, 2 * (float)M_PI) / (2 * (float)M_PI);
	float motionT = GetMotionValue(motionType, normalizedT);
	float finalT = motionT * 2 * (float)M_PI;

	sf::Vector2f position = {
		centerPoint.x + amplitude.x * sinf(finalT),
		centerPoint.y + amplitude.y * sinf(2 * finalT)
	};

	return position;
}

sf::Vector2f MotionAround::MoveShapeAroundPos_Circle(float _dt)
{
	float deltaAngle = (2 * (float)M_PI / duration) * _dt;
	if (timeElapsed >= 2 * (float)M_PI)
	{
		timeElapsed -= 2 * (float)M_PI;
	}
	timeElapsed += deltaAngle;
	float normalizedT = fmodf(timeElapsed, 2 * (float)M_PI) / (2 * (float)M_PI);
	float motionT = GetMotionValue(motionType, normalizedT);
	float finalT = motionT * 2 * (float)M_PI - ((float)M_PI / 2);

	sf::Vector2f position =
	{
		centerPoint.x + amplitude.x * cosf(finalT),
		centerPoint.y + amplitude.y * sinf(finalT)
	};

	return position;
}

sf::Vector2f MotionAround::MoveShapeAroundPos_EllipseOscillate(float _dt)
{
	float deltaAngle = (2 * (float)M_PI / duration) * _dt;

	if (timeElapsed >= 2 * (float)M_PI)
	{
		timeElapsed -= 2 * (float)M_PI;
	}

	timeElapsed += deltaAngle;
	float t = timeElapsed;

	float normalizedT = fmodf(t, 2 * (float)M_PI) / (2 * (float)M_PI);
	if (normalizedT < 0)
	{
		normalizedT += 1.0f;
	}
	float motionT = GetMotionValue(motionType, normalizedT);
	float finalT = motionT * 2 * (float)M_PI;

	sf::Vector2f position = {
		centerPoint.x + amplitude.x * sinf(finalT),
		centerPoint.y + amplitude.y * cosf(finalT * 2.0f) - amplitude.y
	};

	return position;
}

sf::Vector2f MotionAround::MoveShapeAroundPos_HorizontalWave(float _dt)
{
	float deltaAngle = (2 * (float)M_PI / duration) * _dt;

	timeElapsed += deltaAngle;

	if (timeElapsed >= 2 * (float)M_PI)
	{
		timeElapsed -= 2 * (float)M_PI;
	}

	float normalizedT = fmodf(timeElapsed, 2 * (float)M_PI) / (2 * (float)M_PI);
	float motionT = GetMotionValue(motionType, normalizedT);
	float finalT = motionT * 2 * (float)M_PI;

	sf::Vector2f position =
	{
		centerPoint.x + amplitude.x * sinf(finalT),
		centerPoint.y
	};

	return position;
}

sf::Vector2f MotionAround::MoveShapeAroundPos_VerticalWave(float _dt)
{
	float deltaAngle = (2 * (float)M_PI / duration) * _dt;

	timeElapsed += deltaAngle;

	if (timeElapsed >= 2 * (float)M_PI)
	{
		timeElapsed -= 2 * (float)M_PI;
	}

	float normalizedT = fmodf(timeElapsed, 2 * (float)M_PI) / (2 * (float)M_PI);
	float motionT = GetMotionValue(motionType, normalizedT);
	float finalT = motionT * 2 * (float)M_PI;

	sf::Vector2f position =
	{
		centerPoint.x,
		centerPoint.y + amplitude.y * sinf(finalT)
	};

	return position;
}

#pragma endregion

#pragma region MotionAround Class

MotionAround::MotionAround(sf::Vector2f _centerPoint, sf::Vector2f _amplitude, float _duration, MotionAroundType _motionAroundType, MotionType _motionType, bool _loop)
{
	centerPoint = _centerPoint;
	amplitude = _amplitude;
	duration = _duration;
	motionAroundType = _motionAroundType;
	motionType = _motionType;
	loop = _loop;
	timeElapsed = 0.0f;
	motion;
}

sf::Vector2f MotionAround::UpdateValue(float _dt)
{
	if (duration == 0.0f) return centerPoint;

	switch (motionAroundType)
	{
	case MotionAroundType::SHAKE:
		return MoveShapeAroundPos_Shake(_dt);
	case MotionAroundType::LEMNISCATE:
		return MoveShapeAroundPos_Lemniscate(_dt);
	case MotionAroundType::CIRCULAR:
		return MoveShapeAroundPos_Circle(_dt);
	case MotionAroundType::ELLIPSE_OSCILLATE:
		return MoveShapeAroundPos_EllipseOscillate(_dt);
	case MotionAroundType::HORIZONTAL_WAVE:
		return MoveShapeAroundPos_HorizontalWave(_dt);
	case MotionAroundType::VERTICAL_WAVE:
		return MoveShapeAroundPos_VerticalWave(_dt);
	default:
		return centerPoint;
		break;
	}
}

void MotionAround::Reset(void)
{
	timeElapsed = 0.0f;
	for (int i = 0; i < 2; i++)
	{
		motion[i].Reset();
	}
	count = 0;
}

#pragma endregion


float ConvergeValue(float _currentValue, float _targetValue, float _valuePerSec, float _dt)
{
	if (_currentValue < _targetValue)
	{
		_currentValue += _valuePerSec * _dt;
		if (_currentValue > _targetValue)
		{
			_currentValue = _targetValue;
		}
	}
	else if (_currentValue > _targetValue)
	{
		_currentValue -= _valuePerSec * _dt;
		if (_currentValue < _targetValue)
		{
			_currentValue = _targetValue;
		}
	}
	return _currentValue;
}

sf::Vector2f ConvergeVector2(sf::Vector2f _currentValue, sf::Vector2f _targetValue, sf::Vector2f _valuePerSec, float _dt)
{
	float angleRad = atan2f(_targetValue.y - _currentValue.y, _targetValue.x - _currentValue.x);
	float distance = sqrtf(powf(_targetValue.x - _currentValue.x, 2) + powf(_targetValue.y - _currentValue.y, 2));
	float moveDistance = sqrtf(powf(_valuePerSec.x * _dt, 2) + powf(_valuePerSec.y * _dt, 2));
	if (distance <= moveDistance)
	{
		return _targetValue;
	}
	else
	{
		_currentValue.x += cosf(angleRad) * moveDistance;
		_currentValue.y += sinf(angleRad) * moveDistance;
		return _currentValue;
	}
}
