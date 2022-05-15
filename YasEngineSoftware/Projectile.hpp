#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include"YasGraphicsLibrary.hpp"
#include"GameObject.hpp"


class Projectile : public GameObject
{
	private:
		bool directionSwitched = false;
	public:
		Vector2D<float> velocity;
		Projectile(float radius, float x, float y, Vector2D<float> direction);
		~Projectile();
		void move(float deltaTime) override;
		void generate() override;
		void generateRegularPolygonVertices(const Vector2D<float>& position, float circumscribedCircleRadius, int numberOfVertices) override;
		void regeneratePolygon() override;

};

#endif
