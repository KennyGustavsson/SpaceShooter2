#pragma once
#include "SDL.h"
#include "Vector2.h"
#include <vector>
#include "iostream"
#include <fstream>
#include <strstream>
#include <string>  

using namespace std;

/// <summary>
/// Triangle contains 3 points and a color.
/// </summary>
struct Triangle
{
	Triangle() {}

	Triangle(Vector2 p1, Vector2 p2, Vector2 p3)
	{
		point[0] = p1;
		point[1] = p2;
		point[2] = p3;
	}

	Vector2 point[3];
	SDL_Color color = { 255, 255, 255, 255 };
};

/// <summary>
/// Container of triangles.
/// </summary>
struct Mesh
{
	std::vector<Triangle> t = std::vector<Triangle>();

	/// <summary>
	/// Loads an obj file from path.
	/// </summary>
	/// <param name="sFilename"></param>
	/// <returns></returns>
	bool LoadFromObjFile(string sFilename)
	{
		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Clear the triangles
		t.clear();

		vector<Vector2> verts;

		// Read the file
		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			// Check the initial char
			if (line[0] == 'v')
			{
				if (line[1] == 't')
				{

				}
				else
				{
					Vector2 v;
					s >> junk >> v.x >> v.y;
					verts.push_back(v);
				}
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				t.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}
		return true;
	}

	/// <summary>
	/// Scales the mesh triangles.
	/// </summary>
	/// <param name="f"></param>
	void Scale(float f)
	{
		for (int i = 0; i < t.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				t[i].point[j].x *= f;
				t[i].point[j].y *= f;
			}
		}
	}

	/// <summary>
	/// Draws the mesh triangles to renderer.
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="rot"></param>
	/// <param name="pos"></param>
	void DrawModel(SDL_Renderer* renderer, float rot, Vector2 pos)
	{
		Mesh m = *this;
		Mesh transformed;

		// Rotate
		for (int i = 0; i < m.t.size(); i++)
		{
			transformed.t.push_back(Triangle());

			//triangle
			for (int j = 0; j < 3; j++)
			{
				transformed.t[i].point[j].x = m.t[i].point[j].x * cos(rot) - m.t[i].point[j].y * sin(rot);
				transformed.t[i].point[j].y = m.t[i].point[j].x * sin(rot) + m.t[i].point[j].y * cos(rot);
			}
		}

		// Translate
		for (int i = 0; i < transformed.t.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				transformed.t[i].point[j] += pos;
			}
		}

		for (int i = 0; i < transformed.t.size(); i++)
		{
			DrawFilledTriangle(transformed.t[i], renderer);
		}
	}

private:
	/// <summary>
	/// Method to renderer a filled triangle to renderer.
	/// </summary>
	/// <param name="t"></param>
	/// <param name="renderer"></param>
	static void DrawFilledTriangle(Triangle const t, SDL_Renderer* renderer)
	{
		int maxX = std::max(t.point[0].x, std::max(t.point[1].x, t.point[2].x));
		int minX = std::min(t.point[0].x, std::min(t.point[1].x, t.point[2].x));

		int maxY = std::max(t.point[0].y, std::max(t.point[1].y, t.point[2].y));
		int minY = std::min(t.point[0].y, std::min(t.point[1].y, t.point[2].y));

		for (int y = minY; y < maxY + 1; y++)
		{
			for (int x = minX; x < maxX + 1; x++)
			{
				Vector2 point = Vector2(x, y);

				if (PointInTriangle(point, t))
				{
					SDL_SetRenderDrawColor(renderer, t.color.r, t.color.g, t.color.b, t.color.a);
					SDL_RenderDrawPoint(renderer, point.x, point.y);
				}
			}
		}
	}

	/// <summary>
	/// Returns the signed value for a triangle.
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	static float Sign(Vector2 const a, Vector2 const b, Vector2 const c)
	{
		return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
	}

	/// <summary>
	/// Returns if point is in triangle.
	/// </summary>
	/// <param name="point"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static bool PointInTriangle(Vector2 const point, Triangle const t)
	{
		float d1, d2, d3;
		bool negative, positive;

		d1 = Sign(point, t.point[0], t.point[1]);
		d2 = Sign(point, t.point[1], t.point[2]);
		d3 = Sign(point, t.point[2], t.point[0]);

		negative = (d1 < 0) || (d2 < 0) || (d3 < 0);
		positive = (d1 > 0) || (d2 > 0) || (d3 > 0);

		return !(negative && positive);
	}
};