
#ifndef ORION_PHYSICS_H
#define ORION_PHYSICS_H

#include "Graphics.h";

//pos is bottom back left
class Cube {
public:
	glm::vec3 pos;
	glm::vec3 vel;
	float width;

	Cube(glm::vec3 position, glm::vec3 velocity, float cubeWidth) {
		pos = position;
		vel = velocity;
		width = cubeWidth;
	}

	Cube() {
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		vel = glm::vec3(0.0f, 0.0f, 0.0f);
		width = 1.0f;
	}

	//call once per frame 
	void applyVelocity(float deltaTime) {
		pos.x += vel.x * deltaTime;
		pos.y += vel.y * deltaTime;
		pos.z += vel.z * deltaTime;
	}
};


//world coord -> arbitrary values that openGL draws to
//grid coord -> correspond to indices in array, scaling factor applied to transform to world coords, based on what looks good
//grid coords make world coords cubes into unit length
//ex: cube is 0.5 width in world coord, but 1.0 length in grid coords
class Grid {
public:
	//world coordinates * scaleFactor = gridCoordinates
	float scaleFactor;

	Grid(float scaleFactor = 2.0f) {
		this->scaleFactor = scaleFactor;
	}

	//return three indices (x, y, z)
	glm::vec3 worldToGrid(glm::vec3 worldCoords) {
		return glm::vec3((int)(worldCoords.x * scaleFactor), (int)(worldCoords.y * scaleFactor), (int)(worldCoords.z * scaleFactor));
	}

	glm::vec3 gridToWorld(glm::vec3 gridCoords) {
		return glm::vec3(gridCoords.x / scaleFactor, gridCoords.y / scaleFactor, gridCoords.z / scaleFactor);
	}

	//fix this to not use a magic numbe rof 0.5f
	//takes a cube, the world (3d array)
	template<std::size_t worldSize>
	static bool CubeVsGrid(Cube c, bool(*world)[worldSize][worldSize]) {
		//get 8 corners of the cube in tile coordinates

		/*
		glm::vec3 upFrontLeft = glm::vec3((int)(c.pos.x - 0.5f), (int)(c.pos.y + 0.5f), (int)(c.pos.z + 0.5f));
		glm::vec3 upFrontRight = glm::vec3((int)(c.pos.x + 0.5f), (int)(c.pos.y + 0.5f), (int)(c.pos.z + 0.5f));
		glm::vec3 upBackLeft = glm::vec3((int)(c.pos.x - 0.5f), (int)(c.pos.y + 0.5f), (int)(c.pos.z - 0.5f));
		glm::vec3 upBackRight = glm::vec3((int)(c.pos.x + 0.5f), (int)(c.pos.y + 0.5f), (int)(c.pos.z - 0.5f));

		glm::vec3 bottomFrontLeft = glm::vec3((int)(c.pos.x - 0.5f), (int)(c.pos.y - 0.5f), (int)(c.pos.z + 0.5f));
		glm::vec3 bottomFrontRight = glm::vec3((int)(c.pos.x + 0.5f), (int)(c.pos.y - 0.5f), (int)(c.pos.z + 0.5f));
		glm::vec3 bottomBackLeft = glm::vec3((int)(c.pos.x - 0.5f), (int)(c.pos.y - 0.5f), (int)(c.pos.z - 0.5f));
		glm::vec3 bottomBackRight = glm::vec3((int)(c.pos.x + 0.5f), (int)(c.pos.y - 0.5f), (int)(c.pos.z - 0.5f));
		*/

		glm::vec3 bottomBackLeft = c.pos;
		glm::vec3 bottomBackRight = c.pos + glm::vec3(c.width, 0, 0);
		glm::vec3 bottomFrontLeft = c.pos + glm::vec3(0, 0, c.width);
		glm::vec3 bottomFrontRight = c.pos + glm::vec3(c.width, 0, c.width);

		glm::vec3 topBackLeft = c.pos + glm::vec3(0, c.width, 0);
		glm::vec3 topBackRight = c.pos + glm::vec3(c.width, c.width, 0);
		glm::vec3 topFrontLeft = c.pos + glm::vec3(0, c.width, c.width);
		glm::vec3 topFrontRight = c.pos + glm::vec3(c.width, c.width, c.width);
		
		//std::cout << std::to_string(topFrontLeft.x) << " " << std::to_string(topFrontLeft.y) << " " << std::to_string(topFrontLeft.z) << std::endl;

		
		
		//if a corner is in a tile that is occupied, follow protocol to correct cube position
		//std::cout << std::to_string(worldSize) << std::endl;
		//
		
		return (Grid::PointVsGrid(topFrontLeft, world) ||
			Grid::PointVsGrid(topFrontRight, world) ||
			Grid::PointVsGrid(topBackLeft, world) ||
			Grid::PointVsGrid(topBackRight, world) ||
			Grid::PointVsGrid(bottomFrontLeft, world) ||
			Grid::PointVsGrid(bottomFrontRight, world) ||
			Grid::PointVsGrid(bottomBackLeft, world) ||
			Grid::PointVsGrid(bottomBackRight, world)
			);

		//return true;
	}

	//accepts a point in grid coordinates
	template<std::size_t worldSize>
	static bool PointVsGrid(glm::vec3 point, bool(*world)[worldSize][worldSize]) {

		int x = (int)(point.x);
		int y = (int)(point.y);
		int z = (int)(point.z);
		
		if (x < worldSize && y < worldSize && z < worldSize &&
			x >= 0 && y >= 0 && z >= 0) {
			return world[x][y][z];
		}
		return false;
	}


	static void CubeStayInGrid(Cube* c, int worldSize) {	
		Grid::clamp(&c->pos.x, 0, worldSize - 1);
		Grid::clamp(&c->pos.y, 0, worldSize - 1);
		Grid::clamp(&c->pos.z, 0, worldSize - 1);
	}
private:
	static void clamp(float* num, float min, float max) {
		if (*num < min) {
			*num = min;
		}
		else if (*num > max) {
			*num = max;
		}
	}


};

class Physics {

public:
	//raycast 
	static bool RayVsCube(glm::vec3 start, glm::vec3 dir, Cube c, float * t) {

		//left bottom back
		glm::vec3 t_near = (c.pos + glm::vec3(-1*c.width, -1*c.width, -1*c.width)) - start;
		t_near = { (t_near.x / dir.x), (t_near.y / dir.y), (t_near.z / dir.z) };

		//right top front 
		glm::vec3 t_far = (c.pos + glm::vec3(c.width, c.width, c.width)) - start;
		t_far = { (t_far.x / dir.x), (t_far.y / dir.y), (t_far.z / dir.z) };

		float t_min = std::max(std::max(std::min(t_near.x, t_far.x), std::min(t_near.y, t_far.y)), std::min(t_near.z, t_far.z));
		float t_max = std::min(std::min(std::max(t_near.x, t_far.x), std::max(t_near.y, t_far.y)), std::max(t_near.z, t_far.z));

		//ray intersects behind the start vector
		if (t_max > 0) {
			*t = t_max;
			return false;
		}

		//no intersection
		if (t_min > t_max) {
			return false;
		}

		*t = t_min;
		return true;
	}

	static bool CubeVsCube(Cube c1, Cube c2) {
		return
			((c1.pos.x >= c2.pos.x && c1.pos.x <= c2.pos.x + c2.width)
				|| (c1.pos.x + c1.width >= c2.pos.x && c1.pos.x + c1.width <= c2.pos.x + c2.width))
			&&
			((c1.pos.y >= c2.pos.y && c1.pos.y <= c2.pos.y + c2.width)
				|| (c1.pos.y + c1.width >= c2.pos.y && c1.pos.y + c1.width <= c2.pos.y + c2.width))
			&&
			((c1.pos.z >= c2.pos.z && c1.pos.z <= c2.pos.z + c2.width)
				|| (c1.pos.z + c1.width >= c2.pos.z && c1.pos.z + c1.width <= c2.pos.z + c2.width));
	}

		/*
	bool RayVsRect(const glm::vec3 & ray_origin, const glm::vec3 & ray_dir, const Cube * target, glm::vec3 & contact_point, glm::vec3 & contact_normal, float& t_hit_near)
	{
		contact_normal = { 0,0, 0 };
		contact_point = { 0,0, 0 };

		// Cache division
		glm::vec3 invdir = 1.0f / ray_dir;

		// Calculate intersections with rectangle bounding axes
		glm::vec3 t_near = (target->pos - ray_origin) * invdir;
		glm::vec3 t_far = (target->pos + target->width - ray_origin) * invdir;

		if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
		if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

		// Sort distances
		if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
		if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

		// Early rejection		
		if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

		// Closest 'time' will be the first contact
		t_hit_near = std::max(t_near.x, t_near.y);

		// Furthest 'time' is contact on opposite side of target
		float t_hit_far = std::min(t_far.x, t_far.y);

		// Reject if ray direction is pointing away from object
		if (t_hit_far < 0)
			return false;

		// Contact point of collision from parametric line equation
		contact_point = ray_origin + t_hit_near * ray_dir;

		if (t_near.x > t_near.y)
			if (invdir.x < 0)
				contact_normal = { 1, 0 };
			else
				contact_normal = { -1, 0 };
		else if (t_near.x < t_near.y)
			if (invdir.y < 0)
				contact_normal = { 0, 1 };
			else
				contact_normal = { 0, -1 };

		// Note if t_near == t_far, collision is principly in a diagonal
		// so pointless to resolve. By returning a CN={0,0} even though its
		// considered a hit, the resolver wont change anything.
		return true;
	}

	bool DynamicRectVsRect(const olc::aabb::rect* r_dynamic, const float fTimeStep, const olc::aabb::rect& r_static,
		olc::vf2d& contact_point, olc::vf2d& contact_normal, float& contact_time)
	{
		// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
		if (r_dynamic->vel.x == 0 && r_dynamic->vel.y == 0)
			return false;

		// Expand target rectangle by source dimensions
		olc::aabb::rect expanded_target;
		expanded_target.pos = r_static.pos - r_dynamic->size / 2;
		expanded_target.size = r_static.size + r_dynamic->size;

		if (RayVsRect(r_dynamic->pos + r_dynamic->size / 2, r_dynamic->vel * fTimeStep, &expanded_target, contact_point, contact_normal, contact_time))
			return (contact_time >= 0.0f && contact_time < 1.0f);
		else
			return false;
	}



	bool ResolveDynamicRectVsRect(olc::aabb::rect* r_dynamic, const float fTimeStep, olc::aabb::rect* r_static)
	{
		olc::vf2d contact_point, contact_normal;
		float contact_time = 0.0f;
		if (DynamicRectVsRect(r_dynamic, fTimeStep, *r_static, contact_point, contact_normal, contact_time))
		{
			if (contact_normal.y > 0) r_dynamic->contact[0] = r_static; else nullptr;
			if (contact_normal.x < 0) r_dynamic->contact[1] = r_static; else nullptr;
			if (contact_normal.y < 0) r_dynamic->contact[2] = r_static; else nullptr;
			if (contact_normal.x > 0) r_dynamic->contact[3] = r_static; else nullptr;

			r_dynamic->vel += contact_normal * olc::vf2d(std::abs(r_dynamic->vel.x), std::abs(r_dynamic->vel.y)) * (1 - contact_time);
			return true;
		}

		return false;
	}
	/*		// Sort collisions in order of distance
		olc::vf2d cp, cn;
		float t = 0, min_t = INFINITY;
		std::vector<std::pair<int, float>> z;

		// Work out collision point, add it to vector along with rect ID
		for (size_t i = 1; i < vRects.size(); i++)
		{
			if (olc::aabb::DynamicRectVsRect(&vRects[0], fElapsedTime, vRects[i], cp, cn, t))
			{
				z.push_back({ i, t });
			}
		}

		// Do the sort
		std::sort(z.begin(), z.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
			{
				return a.second < b.second;
			});

		// Now resolve the collision in correct order
		for (auto j : z)
			olc::aabb::ResolveDynamicRectVsRect(&vRects[0], fElapsedTime, &vRects[j.first]);

*/


};

#endif
