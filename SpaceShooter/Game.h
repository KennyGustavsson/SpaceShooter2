#pragma once
#include "DisplayManager.h"
#include "Timer.h"
#include "Data.h"
#undef main // Just an SDL thing

class Game 
{
private:
	ObjectData _objectData = ObjectData();
	Timer _timer = Timer();
	DisplayManager _displayManager = DisplayManager(800, 500);
	float _deltaTime = 0.0f;
	float _timeElapsed = 0.0f;
	float _timeScale = 1.0f;
	float _shotTimer = 0.0f;
	float _shotDelay = 0.3f;
	float _asteroidSpeed = 1.0f;
	const Uint8* _currentKeyStates = SDL_GetKeyboardState(NULL);
	bool _restart = false;
	bool _quit = false;

public:

	/// <summary>
	/// Starts the game.
	/// </summary>
	/// <returns></returns>
	bool StartGame()
	{
		SDL_Event input;
		_timer.Start();

		// Create Player.
		int PlayerIndex = _objectData.AddObjectComponents();
		_objectData.meshes[PlayerIndex].LoadFromObjFile("Assets/alien.obj");
		_objectData.transforms[PlayerIndex].Location = Vector2(400, 400);

		// Create Big Asteroids.
		for (int i = 0; i < 10; i++)
		{
			int AsteroidIndex = _objectData.AddObjectComponents();
			_objectData.meshes[AsteroidIndex].LoadFromObjFile("Assets/asteroid_big.obj");
			_objectData.meshes[AsteroidIndex].Scale(50.0f);
			_objectData.transforms[AsteroidIndex].Location = Vector2((std::rand() % 750) + 25, (std::rand() % 100) - 100);
			_objectData.transforms[AsteroidIndex].Velocity = Vector2(0, (std::rand() % 100) + 10);
			_objectData.transforms[AsteroidIndex].Rotation = (std::rand() % 360);
			_objectData.colliders[AsteroidIndex].Radius = 40;
		}

		// Create Medium Asteroids.
		for (int i = 0; i < 5; i++)
		{
			int AsteroidIndex = _objectData.AddObjectComponents();
			_objectData.meshes[AsteroidIndex].LoadFromObjFile("Assets/asteroid_medium.obj");
			_objectData.meshes[AsteroidIndex].Scale(50.0f);
			_objectData.transforms[AsteroidIndex].Location = Vector2((std::rand() % 750) + 25, (std::rand() % 100) - 100);
			_objectData.transforms[AsteroidIndex].Velocity = Vector2(0, (std::rand() % 100) + 10);
			_objectData.transforms[AsteroidIndex].Rotation = (std::rand() % 360);
			_objectData.colliders[AsteroidIndex].Radius = 35;
		}

		// Create Small Asteroids.
		for (int i = 0; i < 5; i++)
		{
			int AsteroidIndex = _objectData.AddObjectComponents();
			_objectData.meshes[AsteroidIndex].LoadFromObjFile("Assets/asteroid_small.obj");
			_objectData.meshes[AsteroidIndex].Scale(50.0f);
			_objectData.transforms[AsteroidIndex].Location = Vector2((std::rand() % 750) + 25, (std::rand() % 100) - 100);
			_objectData.transforms[AsteroidIndex].Velocity = Vector2(0, (std::rand() % 100) + 10);
			_objectData.transforms[AsteroidIndex].Rotation = (std::rand() % 360);
			_objectData.colliders[AsteroidIndex].Radius = 30;
		}

		// Game Loop.
		while (!_quit)
		{
			// Handle SDL Quit.
			while (SDL_PollEvent(&input) > 0)
			{
				if (input.type == SDL_QUIT) _quit = true;
			}

			// Delta Time.
			DeltaTimer();

			// Update.
			Update(_deltaTime);	
		}

		// Returns if the game should restart.
		return _restart;
	}

#pragma region Updates
private:
	/// <summary>
	/// Main update for everything.
	/// </summary>
	/// <param name="DeltaTime"></param>
	void Update(float const DeltaTime)
	{
		_shotTimer += DeltaTime;
		_asteroidSpeed += DeltaTime * 0.05f;
		_displayManager.ClearRenderer();

		LimitAsteroidLocation();

		UpdatePlayer();
		{
			// Player Transform
			_objectData.transforms[0].Location += _objectData.transforms[0].Velocity * DeltaTime;
			Vector2 const Loc = _objectData.transforms[0].Location;
			float const Rot = _objectData.transforms[0].Rotation;

			// Player Collider
			_objectData.colliders[0].CenterPosition = Loc;

			// Player Mesh
			_objectData.meshes[0].DrawModel(_displayManager.Renderer, Rot, Loc);
		}

		for (int i = 1; i < _objectData.size + 1; i++)
		{
			// Asteroid Gravity if not traveling down
			if (_objectData.transforms[i].Velocity.y > 30) 
			{
				_objectData.transforms[i].Velocity.y + DeltaTime * 98.2f;
			}

			// Asteroid Transform
			_objectData.transforms[i].Location += _objectData.transforms[i].Velocity * DeltaTime * _asteroidSpeed;
			Vector2 const Loc = _objectData.transforms[i].Location;
			float const Rot = _objectData.transforms[i].Rotation;

			// Asteroid Collider
			_objectData.colliders[i].CenterPosition = Loc;

			// Asteroid Mesh
			_objectData.meshes[i].DrawModel(_displayManager.Renderer, Rot, Loc);
		}

		// Bullets
		_objectData.bullets.UpdateBullet(_displayManager.Renderer, DeltaTime);

		// Collision handeling
		Collision();

		// Present renderer
		_displayManager.PresentRenderer();
	}

	/// <summary>
	/// Update special things for player.
	/// </summary>
	void UpdatePlayer() 
	{
		auto& trans = _objectData.transforms[0];

		Vector2 NewVelocity = Vector2(0, 0);

		if (_currentKeyStates[SDL_SCANCODE_W] || _currentKeyStates[SDL_SCANCODE_UP])
		{
			NewVelocity += Vector2(0, -1);
		}

		if (_currentKeyStates[SDL_SCANCODE_S] || _currentKeyStates[SDL_SCANCODE_DOWN])
		{
			NewVelocity += Vector2(0, 1);
		}

		if (_currentKeyStates[SDL_SCANCODE_A] || _currentKeyStates[SDL_SCANCODE_LEFT])
		{
			NewVelocity += Vector2(-1, 0);
		}

		if (_currentKeyStates[SDL_SCANCODE_D] || _currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			NewVelocity += Vector2(1, 0);
		}

		NewVelocity.Normalize();
		trans.Velocity = NewVelocity * 200;

		trans.Location.x = Clamp(trans.Location.x, 25, 775);
		trans.Location.y = Clamp(trans.Location.y, 25, 475);

		if (_currentKeyStates[SDL_SCANCODE_SPACE] && _shotTimer > _shotDelay)
		{
			_objectData.bullets.FireBullet(trans.Location);
			_shotTimer = 0.0f;
		}
	}

	/// <summary>
	/// Limit the asteroids locations and respawns them if outside of bounds.
	/// </summary>
	void LimitAsteroidLocation() 
	{
		for (int i = 1; i < _objectData.size + 1; i++)
		{
			Vector2 Loc = _objectData.transforms[i].Location;
			if (Loc.y > 800 || Loc.x > 900 || Loc.x < -100)
			{
				ResetAsteroid(i);
			}
		}
	}

	/// <summary>
	/// Default reset for asteroid.
	/// </summary>
	/// <param name="index"></param>
	void ResetAsteroid(int index) 
	{
		Vector2 Loc = Vector2((std::rand() % 750) + 25, (std::rand() % 100) - 150);
		Vector2 Vel = Vector2(0, (std::rand() % 100) + 10);
		float Rot = (std::rand() % 360);

		_objectData.transforms[index].Location = Loc;
		_objectData.transforms[index].Velocity = Vel;
		_objectData.transforms[index].Rotation = Rot;
		_objectData.colliders[index].CenterPosition = Loc;
	}

	/// <summary>
	/// Handle collision.
	/// </summary>
	void Collision() 
	{
		// Collision Checking for Player and Bullets
		for (int i = 1; i < _objectData.size + 1; i++)
		{
			Vector2 Loc = _objectData.colliders[i].CenterPosition;
			float Rad = _objectData.colliders[i].Radius;

			// Bullet
			if (_objectData.bullets.CheckBulletCollision(Loc, Rad)) 
			{
				ResetAsteroid(i);
			}

			// Player
			if (DoCirclesOverlap(_objectData.colliders[0].CenterPosition, _objectData.colliders[0].Radius,
				Loc, Rad))
			{
				// Collided with player
				_restart = true;
				_quit = true;
			}
		}

		// Dynamic Collision Checking
		for (int i = 1; i < _objectData.size + 1; i++)
		{
			for (int j = 1; j < _objectData.size + 1; j++)
			{
				if (i != j)
				{
					Vector2 aPos = _objectData.colliders[i].CenterPosition;
					Vector2 bPos = _objectData.colliders[j].CenterPosition;

					float aRad = _objectData.colliders[i].Radius;
					float bRad = _objectData.colliders[j].Radius;

					if (DoCirclesOverlap(aPos, aRad, bPos, bRad))
					{
						// Add to update velocity
						_objectData.collidingPairs.push_back(std::make_pair(i, j));

						// Move the object so they don't overlap
						float distance = sqrtf((aPos.x - bPos.x) * (aPos.x - bPos.x) + (aPos.y - bPos.y) * (aPos.y - bPos.y));
						float overlap = 0.5f * (distance - aRad - bRad);

						_objectData.transforms[i].Location = aPos - (aPos - bPos) * overlap / distance;
						_objectData.transforms[j].Location = bPos + (aPos - bPos) * overlap / distance;
					}
				}
			}
		}

		HandleDynamicCollision();
	}

	/// <summary>
	/// Handle dynamic collisions.
	/// </summary>
	void HandleDynamicCollision()
	{
		for (size_t i = 0; i < _objectData.collidingPairs.size(); i++)
		{
			int b1 = _objectData.collidingPairs[i].first;
			int b2 = _objectData.collidingPairs[i].second;

			Vector2 posA = _objectData.colliders[b1].CenterPosition;
			Vector2 posB = _objectData.colliders[b2].CenterPosition;

			Vector2 vecA = _objectData.transforms[b1].Velocity;
			Vector2 vecB = _objectData.transforms[b2].Velocity;

			float radA = _objectData.colliders[b1].Radius;
			float radB = _objectData.colliders[b2].Radius;

			float distance = sqrtf((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y));
			Vector2 normal = (posB - posA) / distance;
			Vector2 tangent = Vector2(-normal.y, normal.x);

			float dotTanA = vecA.x * tangent.x + vecA.y * tangent.y;
			float dotTanB = vecB.x * tangent.x + vecB.y * tangent.y;

			float dotNormA = vecA.x * normal.x + vecA.y * normal.y;
			float dotNormB = vecB.x * normal.x + vecB.y * normal.y;

			// Conservation of momentum
			float m1 = (dotNormA * (1.5f * radA - 1.2f * radB) + 2.0f * (1.2f * radB) * dotNormB) / (1.5f * radA + 1.2f * radB);
			float m2 = (dotNormB * (1.2f * radA - 1.5f * radB) + 2.0f * (1.5f * radB) * dotNormA) / (1.5f * radA + 1.2f * radB);

			// Update velocities
			Vector2 newAVel;
			newAVel.x = tangent.x * dotTanA + normal.x * m1;
			newAVel.y = tangent.y * dotTanA + normal.y * m1;

			Vector2 newBVel;
			newBVel.x = tangent.x * dotTanB + normal.x * m2;
			newBVel.y = tangent.y * dotTanB + normal.y * m2;

			newAVel.x = Clamp(newAVel.x, -100, 100);
			newAVel.y = Clamp(newAVel.y, -10, 100);

			newBVel.x = Clamp(newBVel.x, -100, 100);
			newBVel.y = Clamp(newBVel.y, -10, 100);

			_objectData.transforms[b1].Velocity = newAVel;
			_objectData.transforms[b2].Velocity = newBVel;
		}

		_objectData.collidingPairs.clear();
	}

	/// <summary>
	/// Calculate deltatime.
	/// </summary>
	void DeltaTimer()
	{
		_deltaTime = (_timer.ElapsedSeconds() - _timeElapsed) * _timeScale;
		_timeElapsed += _deltaTime;
	}
#pragma endregion Updates
	/// <summary>
	/// Returns if circles overlap.
	/// </summary>
	/// <param name="p1"></param>
	/// <param name="r1"></param>
	/// <param name="p2"></param>
	/// <param name="r2"></param>
	/// <returns></returns>
	bool DoCirclesOverlap(Vector2 p1, float r1, Vector2 p2, float r2)
	{
		return fabs((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)) <= (r1 + r2) * (r1 + r2);
	}

	/// <summary>
	/// Clamps value in range.
	/// </summary>
	/// <param name="n"></param>
	/// <param name="lower"></param>
	/// <param name="upper"></param>
	/// <returns></returns>
	float Clamp(float n, float lower, float upper)
	{
		return std::max(lower, std::min(n, upper));
	}
};