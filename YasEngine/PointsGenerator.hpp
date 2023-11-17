#ifndef POINTSGENERATOR_HPP
#define POINTSGENERATOR_HPP
#include "PointsSet.hpp"

class PointsGenerator
{
	public:
		int pointsNumber = 0;
		virtual PointsSet* generatePoints() = 0;
		
};

#endif
