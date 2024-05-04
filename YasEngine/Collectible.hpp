#ifndef COLLECTIBLE_HPP
#define COLLECTIBLE_HPP
#include "GameObject.hpp"
#include "Randomizer.hpp"

class Collectible: public GameObject
{
public:
	Collectible(float radius, float x, float y, int numberOfVertices);
	~Collectible();
	void generate() override;
	void generateRegularPolygonVertices(float circumscribedCircleRadius, int numberOfVertices) override;
	void regeneratePolygon() override;
	void setPosition(float x, float y) override;
	void setPosition(const Vector2D<float>& position) override;
	void move(float deltaTime) override;
	void setColor(const Vector4D<Uint8>& color) override;
	void setRandomColor();
};

#endif