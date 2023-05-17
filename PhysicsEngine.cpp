

#include <iostream>
#include <cmath>
#include "PhysicsEngine.hpp"

Plane::Plane() : coeff_x(0), coeff_y(0), coeff_z(0), constant(0) {}

Plane& Plane::operator*= (double multiplier) {
	coeff_x *= multiplier;
	coeff_y *= multiplier;
	coeff_z *= multiplier;
	constant *= multiplier;
	return *this;
}

Plane& Plane::operator= (const Plane& rhs) {
	coeff_x = rhs.coeff_x;
	coeff_y = rhs.coeff_y;
	coeff_z = rhs.coeff_z;
	constant = rhs.constant;
	return *this;
}

double Plane::distanceToPoint(Point loc) {	// distance formula = |Ax+By+Cz+D|/√(A²+B²+C²) for AX+BY+CZ+D=0
	return fabs(coeff_x*loc.x + coeff_y*loc.y + coeff_z*loc.z - constant) / sqrt(coeff_x*coeff_x + coeff_y*coeff_y + coeff_z*coeff_z);
}