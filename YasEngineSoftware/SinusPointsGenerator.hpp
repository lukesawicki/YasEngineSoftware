#ifndef SINUSPOINTSGENERATOR_HPP
#define SINUSPOINTSGENERATOR_HPP
#include"PointsGenerator.hpp"
#include"PointsSet.hpp"
#include"Vector2D.hpp"
#include "YasGraphicsLibrary.hpp"

class SinusPointsGenerator: public PointsGenerator
{
	public:
		virtual PointsSet* generatePoints() override;

};

#endif