#include"Projectile.hpp"

static Uint32 kill(Projectile* projectileToKill, Uint32 interval, void* param)
{
	projectileToKill->isAlive = false;
	return 0;
}

Projectile::Projectile(float radius, float x, float y, Vector2D<float> direction)
{
	isAlive = true;
	iAm = WhoAmI::PROJECTILE;
	collider.radius = radius;
	speed = 200;
	this->position.x = x;
	this->position.y = y;
	this->collider.x = x;
	this->collider.y = y;

	velocity.x = speed * direction.x;
	velocity.y = speed * direction.y;
	color = YELLOW;
	generateRegularPolygonVertices(radius, 4);
	startTime = timePicker.getMiliseconds();
}

Projectile::~Projectile()
{
	delete[] worldVertices;
	delete[] localVertices;
}

void Projectile::move(float deltaTime)
{
    if(timePicker.getMiliseconds() - startTime >= 2000)
    {
        this->isAlive = false;
    }

	position.x = position.x + deltaTime * velocity.x;
	position.y = position.y + deltaTime * velocity.y;
	moveCollider();
	regeneratePolygon();
}

void Projectile::generate()
{
	for (int i = 0; i < numberOfVertices; i++)
	{
		worldVertices[i].x = position.x + localVertices[i].x;
		worldVertices[i].y = position.y + localVertices[i].y;
	}
}

void Projectile::generateRegularPolygonVertices(float circumscribedCircleRadius, int numberOfVertices)
{
	this->circumscribedCircleRadius = circumscribedCircleRadius;
	this->numberOfVertices = numberOfVertices;
	this->worldVertices = new Vector2D<float>[numberOfVertices];
	this->localVertices = new Vector2D<float>[numberOfVertices];

	angleForGenerateInIsoscelesPolygons = startAngle;
	stepAngle = 360.0F / numberOfVertices;
	for (int i = 0; i < numberOfVertices; i++)
	{
		localVertices[i].x = 0.0F + static_cast<int>(circumscribedCircleRadius * cos(angleForGenerateInIsoscelesPolygons * (PI / 180.0F)));
		localVertices[i].y = 0.0F + static_cast<int>(circumscribedCircleRadius * sin(angleForGenerateInIsoscelesPolygons * (PI / 180.0F)));
		angleForGenerateInIsoscelesPolygons += stepAngle;
	}
	generate();
}

void Projectile::regeneratePolygon()
{
	generate();
}
