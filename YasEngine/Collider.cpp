#include"Collider.hpp"

Collider::Collider()
{
	this->radius = 0.0F;
	this->x = 0.0F;
	this->y = 0.0F;
}

Collider::Collider(float radius, float x, float y)
{
	this->radius = radius;
	this->x = x;
	this->y = y;
}
bool Collider::isCollision(const Collider& object0, const Collider& object1)
{
	return (sqrt(pow(object1.x - object0.x, 2.0) + pow(object1.y - object0.y, 2.0))) <= (static_cast<double>(object0.radius) + static_cast<double>(object1.radius));
}

bool Collider::isCollidingWithWall(const Collider& object0, const Vector2D<int>& worldSizes)
{
	return object0.x - object0.radius < -static_cast<float>(worldSizes.x) * 0.5F ||	// LEFT
		object0.x + object0.radius > static_cast<float>(worldSizes.x) * 0.5F ||		// RIGHT
		object0.y + object0.radius > static_cast<float>(worldSizes.y) * 0.5F ||		// TOP
		object0.y - object0.radius < -static_cast<float>(worldSizes.y) * 0.5F;		// BOTTOM
}

bool Collider::isCollidingWithCustomWalls(const Collider& object0, const Vector2D<int>& worldSizes)
{
    return object0.x - object0.radius < -static_cast<float>(worldSizes.x) * 0.5F ||		// LEFT
           object0.x + object0.radius > static_cast<float>(worldSizes.x) * 0.0F ||		// RIGHT
           object0.y + object0.radius > static_cast<float>(worldSizes.y) * 0.5F ||		// TOP
           object0.y - object0.radius < -static_cast<float>(worldSizes.y) * 0.5F ;		// BOTTOM
}
