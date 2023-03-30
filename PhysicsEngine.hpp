#ifndef PHYSICS_H
#define PHYSICS_H

struct Point {
	double x;
	double y;
	double z;
};

struct Orientation {	// in spherical angular format <Θ,φ>
	double theta;
	double phi;
};

class Plane {
	public:
		double coeff_x, coeff_y, coeff_z, constant;	// coeff_x * x + coeff_y * y + coeff_z * z = constant
		Point center;	// center of plane from cube center
		Point vertices[4];	// four corners of square

		Plane();
		Plane& operator*= (double multiplier);
		Plane& operator= (const Plane& rhs);

		double distanceToPoint(Point loc);
};

#endif