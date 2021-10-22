#pragma once
#include "SDL.h"

/// <summary>
/// Vector2 contains 2 float values.
/// </summary>
struct Vector2
{
public:
	float x = 0;
	float y = 0;

	Vector2()
	{
		this->x = 0;
		this->y = 0;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void operator+=(const Vector2& v)
	{
		this->x += v.x;
		this->y += v.y;
	}

	void operator-=(const Vector2& v)
	{
		this->x -= v.x;
		this->y -= v.y;
	}

	void operator*=(const Vector2& v)
	{
		this->x *= v.x;
		this->y *= v.y;
	}

	void operator/=(const Vector2& v)
	{
		this->x /= v.x;
		this->y /= v.y;
	}

	Vector2 operator+(const Vector2& v)
	{
		return Vector2(this->x + v.x, this->y + v.y);
	}

	Vector2 operator-(const Vector2& v)
	{
		return Vector2(this->x - v.x, this->y - v.y);
	}

	Vector2 operator*(const Vector2& v)
	{
		return Vector2(this->x * v.x, this->y * v.y);
	}

	Vector2 operator/(const Vector2& v)
	{
		return Vector2(this->x / v.x, this->y / v.y);
	}

	Vector2 operator*(const float& f)
	{
		return Vector2(this->x * f, this->y * f);
	}

	Vector2 operator/(const float& f)
	{
		return Vector2(this->x / f, this->y / f);
	}

	Vector2 operator*=(const float& f)
	{
		return Vector2(this->x * f, this->y * f);
	}

	Vector2 operator+(const float& f)
	{
		return Vector2(this->x + f, this->y + f);
	}

	/// <summary>
	/// Returns the magnitude of this vector using square root.
	/// </summary>
	/// <returns></returns>
	float Magnitude()
	{
		return (float)sqrt(this->x * this->x + this->y * this->y);
	};

	/// <summary>
	/// Normalizes the vector.
	/// </summary>
	/// <returns></returns>
	Vector2 Normalize()
	{
		float distance = Magnitude();
		return Vector2(this->x / distance, this->y / distance);
	}
};