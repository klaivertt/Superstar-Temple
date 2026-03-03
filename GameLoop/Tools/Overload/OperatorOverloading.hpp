#pragma once
#ifndef OPERATOR_OVERLOADING_HPP
#define OPERATOR_OVERLOADING_HPP

#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

template<typename T>
concept sfVectorConcept = !std::same_as<T, char>;

template<typename T>
concept integerValue = std::is_integral_v<T> && !std::is_same_v<T, bool>;

#pragma region sf::Vector

#pragma region sf::Vector2

//sf::Vector Draw On Console

template<sfVectorConcept T>
inline std::ostream& operator << (std::ostream& _os, const sf::Vector2<T>& _vec)
{
	return _os << "(" << _vec.x << ", " << _vec.y << ")";
}

//sf::Vector Multiplication
template<sfVectorConcept T>
inline sf::Vector2<T> operator * (const sf::Vector2<T>& _vec1, const sf::Vector2<T>& _vec2)
{
	return sf::Vector2<T>(_vec1.x * _vec2.x, _vec1.y * _vec2.y);
}

template<sfVectorConcept T>
inline sf::Vector2<T> operator *= (sf::Vector2<T>& _vec1, const sf::Vector2<T>& _vec2)
{
	_vec1 = _vec1 * _vec2;
	return _vec1;
}

template<sfVectorConcept T>
inline sf::Vector2<T> operator /= (sf::Vector2<T>& _vec1, const sf::Vector2<T>& _vec2)
{
	_vec1 = { _vec1.x / _vec2.x, _vec1.y / _vec2.y };
	return _vec1;
}

#pragma endregion

#pragma region sf::Vector3

//sf::Vector Draw On Console

template<sfVectorConcept T>
inline std::ostream& operator << (std::ostream& _os, const sf::Vector3<T>& _vec)
{
	return _os << "(" << _vec.x << ", " << _vec.y << ", " << _vec.z << ")";
}

//sf::Vector Multiplication
template<sfVectorConcept T>
inline sf::Vector2<T> operator * (const sf::Vector3<T>& _vec1, const sf::Vector3<T>& _vec2)
{
	return sf::Vector2<T>(_vec1.x * _vec2.x, _vec1.y * _vec2.y, _vec1.z * _vec2.z);
}

template<sfVectorConcept T>
inline sf::Vector2<T> operator *= (sf::Vector3<T>& _vec1, const sf::Vector3<T>& _vec2)
{
	_vec1 = _vec1 * _vec2;
	return _vec1;
}

#pragma endregion

#pragma endregion

#pragma region std::Vector

//std::Vector Draw On Console

template<typename T>
inline std::ostream& operator << (std::ostream& _os, const std::vector<T>& _vec)
{
	_os << "[\n";
	for (size_t i = 0; i < _vec.size(); i++)
	{
		_os << i << " : " << _vec[i] << "\n";
	}
	_os << "]";
	return _os;
}

//std::Vector Addition

template<typename T>
inline std::vector<T> operator + (const std::vector<T>& _vec1, const std::vector<T>& _vec2)
{
	std::vector<T> newVec = _vec1;

	for (size_t i = 0; i < _vec2.size(); i++)
	{
		newVec.push_back(_vec2[i]);
	}
	return newVec;
}

template<typename T>
inline std::vector<T> operator += (const std::vector<T>& _vec1, const std::vector<T>& _vec2)
{
	return _vec1 + _vec2;
}

//std::Vector Multiplication

template<typename T, integerValue M>
inline std::vector<T> operator * (const std::vector<T>& _vec1, M _multiply)
{
	std::vector<T> newVec;

	for (int j = 0; j < _multiply; j++)
	{
		for (size_t i = 0; i < _vec1.size(); i++)
		{
			newVec.push_back(_vec1[i]);
		}
	}
}

template<typename T, integerValue M>
inline std::vector<T> operator *= (const std::vector<T>& _vec1, M _multiply)
{
	return _vec1 * _multiply;
}

#pragma endregion


#endif