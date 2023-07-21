#ifndef OBJECTS_H
#define OBJECTS_H

#include "PhysicsEngine.hpp"
#include "ObjectRender.h"

enum Type {
	BLOCK,
	ITEM,
};

struct Coordinate {
	int x, y, z;
};

class Cube : public ObjectRender {
private:
	Point center;	// center of cube in cartesian coordinates
	double size;	// side length of cube
	Orientation direction;	// direction cube in facing in spherical angular format <Θ,φ>
	Plane cube_sides[6];	// plane eq for all 6 sides of cube with center as <0,0,0>

public:
	Point vertices[8];	// eight corners of cube
	Cube(Type _blockType, int id);
	Cube(Point _center, Type _blockType, Orientation _direction, Color color, int id);

	
	void setCubeCoordinate(Coordinate loc);	// change center of cube
	void setCubeCoordinate(int x, int y, int z);	// change center of cube
	Coordinate getCubeCoordinate();	// returns cube coordinate in a coord struct

	void setCubeCenter(Point loc);	// change center of cube
	void setCubeCenter(double x, double y, double z);	// change center of cube
	Point getCubeCenter();	// returns center of cube in a point struct

	void setSize(double size);	// sets the size of the cube
	void setDirection(Orientation direction);

	double getSize();
	Orientation getDirection();

	void updatePlanes();	// compute plane eqs for each side
	void updateCubeVertices();	// compute vertices for the cube
	bool checkHolding(Point fingertips[5]);	// check whether fingers are touching at least two cube sides
	void addCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);
};

#endif