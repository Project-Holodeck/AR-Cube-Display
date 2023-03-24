#pragma once

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

		Plane();
		Plane& operator*= (double multiplier);
		Plane& operator= (const Plane& rhs);

		double distanceToPoint(Point loc);
};

class Cube {
	private:
		Point center;	// center of cube in cartesian coordinates
		double size;	// side length of cube
		Orientation direction;	// direction cube in facing in spherical angular format <Θ,φ>
		Plane cube_sides[6];	// plane eq for all 6 sides of cube with center as <0,0,0>

	public:
		Cube();
		Cube(Point _center, double _size, Orientation _direction);

		Point getCubeCenter();	// returns center of cube in a point struct
		void changeCubeCenter(Point loc);	// change center of cube
		void changeCubeCenter(double x, double y, double z);	// change center of cube

		void updatePlanes();	// compute plane eqs for each side
		bool checkHolding(Point fingertips[5]);	// check whether fingers are touching at least two cube sides
};