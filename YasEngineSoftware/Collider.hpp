#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include "Vector2D.hpp"
//#include"YasGraphicsLibrary.hpp"


class Collider
{

	public:
		float radius = 0.0F;
		float x = 0.0F;
		float y = 0.0F;
		Collider();
		Collider(float radius, float x, float y);

		static bool isInCollision(const Collider& object0, const Collider& object1);
		static bool isCollidingWithWall(const Collider& object0, const Vector2D<int>& worldSizes);
	private:
};

#endif