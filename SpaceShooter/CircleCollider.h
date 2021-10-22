#pragma once
#include "Vector2.h"

/// <summary>
/// Collider with a circle shape
/// </summary>
class CircleCollider
{
public:
	Vector2 CenterPosition = Vector2(0.0f, 0.0f);
	float Radius = 20.0f;
};