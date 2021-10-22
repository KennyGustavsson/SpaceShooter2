#pragma once
#include <map>
#include "Mesh.h"
#include "Transform.h"
#include "CircleCollider.h"
#include "Bullet.h"

/// <summary>
/// Contains all bulk data of the game
/// </summary>
struct ObjectData
{
public:
	std::vector<Mesh> meshes;
	std::vector<Transform> transforms;
	std::vector<CircleCollider> colliders;
	std::vector<std::pair<int, int>> collidingPairs;
	Bullets bullets = Bullets(30);
	int size = -1;

	/// <summary>
	/// Adds components for the object, the object is just an ID.
	/// </summary>
	/// <returns></returns>
	int AddObjectComponents() 
	{
		meshes.push_back(Mesh());
		transforms.push_back(Transform());
		colliders.push_back(CircleCollider());

		size++;
		return size;
	}
};