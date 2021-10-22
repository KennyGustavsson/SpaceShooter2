#pragma once
#include "Vector2.h"

/// <summary>
/// Transform contains location, rotation, size and in this case velocity becuase every objects transform uses velocity.
/// </summary>
struct Transform
{
public:
	Vector2 Location = Vector2(0.0f, 0.0f);
	Vector2 Velocity = Vector2(0.0f, 0.0f);
	float Rotation = 0.0f;
	float Size = 1.0f;
};