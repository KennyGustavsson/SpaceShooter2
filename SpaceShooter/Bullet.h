#pragma once
#include "Vector2.h"
#include <vector>

class Bullet 
{
public:
	Vector2 Location = Vector2(0, 0);
	Vector2 Velocity = Vector2(0, 0);
	float Radius = 5.0f;
	bool Active = false;

	/// <summary>
	/// Draws the bullet on the screen
	/// </summary>
	/// <param name="renderer"></param>
	void Draw(SDL_Renderer* renderer) 
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoint(renderer, Location.x, Location.y);
		SDL_RenderDrawPoint(renderer, Location.x + 1, Location.y);
		SDL_RenderDrawPoint(renderer, Location.x - 1, Location.y);
		SDL_RenderDrawPoint(renderer, Location.x, Location.y + 1);
		SDL_RenderDrawPoint(renderer, Location.x, Location.y - 1);
	}
};

/// <summary>
/// Bullet pool
/// </summary>
class Bullets 
{
public:
	std::vector<Bullet> BulletVector;
	int CurrentIndex = -1;

	/// <summary>
	/// Creates a bullet pool with x amount of bullets
	/// </summary>
	/// <param name="amount"></param>
	Bullets(int amount) 
	{
		BulletVector = std::vector<Bullet>(amount);
	}

	/// <summary>
	/// Call to fire a bullet from the pool
	/// </summary>
	/// <param name="loc"></param>
	void FireBullet(Vector2 loc) 
	{
		CurrentIndex = (CurrentIndex + 1) % BulletVector.size();

		auto& bul = BulletVector[CurrentIndex];
		bul.Location = loc;
		bul.Velocity = Vector2(0, -500);
		bul.Active = true;
	}

	/// <summary>
	/// Updates the Bullets and Render them
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="DeltaTime"></param>
	void UpdateBullet(SDL_Renderer* renderer, float const DeltaTime) 
	{
		for (auto& b : BulletVector) 
		{
			if (b.Active) 
			{
				b.Location += b.Velocity * DeltaTime;
				b.Draw(renderer);
			}
		}
	}

	/// <summary>
	/// Checks if colliding with any bullet
	/// </summary>
	/// <param name="Loc"></param>
	/// <param name="radius"></param>
	/// <returns></returns>
	bool CheckBulletCollision(Vector2 Loc, float radius) 
	{
		for (auto& b : BulletVector)
		{
			if (b.Active)
			{
				if (DoCirclesOverlap(Loc, radius, b.Location, b.Radius)) 
				{
					b.Active = false;
					return true;
				}
			}
		}

		return false;
	}

	/// <summary>
	/// Check if circels overlap
	/// </summary>
	/// <param name="p1"></param>
	/// <param name="r1"></param>
	/// <param name="p2"></param>
	/// <param name="r2"></param>
	/// <returns></returns>
	static bool DoCirclesOverlap(Vector2 p1, float r1, Vector2 p2, float r2)
	{
		return fabs((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)) <= (r1 + r2) * (r1 + r2);
	}
};