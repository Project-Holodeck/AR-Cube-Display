﻿#ifndef OBJECTS_H
#define OBJECTS_H

#include "PhysicsEngine.hpp"
#include "ObjectRender.h"

class Cube : public ObjectRender {
private:
	Point center;	// center of cube in cartesian coordinates
	double size;	// side length of cube
	Orientation direction;	// direction cube in facing in spherical angular format <Θ,φ>
	Plane cube_sides[6];	// plane eq for all 6 sides of cube with center as <0,0,0>
	Color color;

public:
	Point vertices[8];	// eight corners of cube
	Cube();
	Cube(Point _center, double _size, Orientation _direction, Color color);

	Point getCubeCenter();	// returns center of cube in a point struct
	void changeCubeCenter(Point loc);	// change center of cube
	void changeCubeCenter(double x, double y, double z);	// change center of cube

	void updatePlanes();	// compute plane eqs for each side
	void updateVertices();	// compute vertices for the cube
	bool checkHolding(Point fingertips[5]);	// check whether fingers are touching at least two cube sides
	void addCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);
};

#endif