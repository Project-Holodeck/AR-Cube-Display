

#include <iostream>
#include <cmath>
#include "PhysicsEngine.hpp"
#include "Objects.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#define FINGERTIP_ERROR 5

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

Cube::Cube(std::vector<ObjectRender>& objectVector) : center({ 0,0,0 }), size(0), direction({ 0,0 }), ObjectRender({ 0, 0, 0 }, objectVector.size()) {
	updatePlanes();
	objectVector.push_back(*this);
}

Cube::Cube(std::vector<ObjectRender>& objectVector, Point _center, double _size, Orientation _direction, Color _color, int id) : center(_center), size(_size), direction(_direction), ObjectRender(_color, id) {
	updatePlanes();
	objectVector.push_back(*this);
}

Point Cube::getCubeCenter() {
	return center;
}

void Cube::changeCubeCenter(Point loc) {
	center = loc;
	updatePlanes();
}

void Cube::changeCubeCenter(double x, double y, double z) {
	center.x = x;
	center.y = y;
	center.z = z;
	updatePlanes();
}

void Cube::setSize(double size) {
	this->size = size;
}

void Cube::setDirection(Orientation direction) {
	this->direction = direction;
}

double Cube::getSize() {
	return this->size;
}

Orientation Cube::getDirection() {
	return this->direction;
}



void Cube::updatePlanes() {
	Plane side;
	double scale;

	Orientation current_dir = direction;

	/* calculates vector to plane, which happens to be in ratio of coefficients of plane eq */
	side.coeff_x = sin(current_dir.theta) * cos(current_dir.phi);
	side.coeff_y = sin(current_dir.theta) * sin(current_dir.phi);
	side.coeff_z = cos(current_dir.theta);
	/* scale coefficients based on cube size */
	side *= sqrt((size / 2));
	/* change rhs of plane eq */
	side.constant = size*size / 4;
	side.center = {side.coeff_x*size/2, side.coeff_y*size/2, side.coeff_z*size/2};
	cube_sides[0] = side;
	/* flip plane to opposite side */
	side.constant *= -1;
	side.center = {-side.coeff_x*size/2, -side.coeff_y*size/2, -side.coeff_z*size/2};
	cube_sides[1] = side;

	/* rotate theta by π/2 */
	current_dir.theta += M_PI / 2;
	/* calculates vector to plane, which happens to be in ratio of coefficients of plane eq */
	side.coeff_x = sin(current_dir.theta) * cos(current_dir.phi);
	side.coeff_y = sin(current_dir.theta) * sin(current_dir.phi);
	side.coeff_z = cos(current_dir.theta);
	/* scale coefficients based on cube size */
	side *= sqrt((size / 2));
	/* change rhs of plane eq */
	side.constant = size*size / 4;
	side.center = {side.coeff_x*size/2, side.coeff_y*size/2, side.coeff_z*size/2};
	cube_sides[2] = side;
	/* flip plane to opposite side */
	side.constant *= -1;
	side.center = {-side.coeff_x*size/2, -side.coeff_y*size/2, -side.coeff_z*size/2};
	cube_sides[3] = side;

	/* rotate phi by π/2 */
	current_dir.phi += M_PI / 2;
	/* calculates vector to plane, which happens to be in ratio of coefficients of plane eq */
	side.coeff_x = sin(current_dir.theta) * cos(current_dir.phi);
	side.coeff_y = sin(current_dir.theta) * sin(current_dir.phi);
	side.coeff_z = cos(current_dir.theta);
	/* scale coefficients based on cube size */
	side *= sqrt((size / 2));
	/* change rhs of plane eq */
	side.constant = size*size / 4;
	side.center = {side.coeff_x*size/2, side.coeff_y*size/2, side.coeff_z*size/2};
	cube_sides[4] = side;
	/* flip plane to opposite side */
	side.constant *= -1;
	side.center = {-side.coeff_x*size/2, -side.coeff_y*size/2, -side.coeff_z*size/2};
	cube_sides[5] = side;
}

void Cube::updateCubeVertices() {
	Point vertex;

	vertex = {
		size / 2 * sin(direction.theta + atan(sqrt(2))) * cos(direction.phi + M_PI / 4),
		size / 2 * sin(direction.theta + atan(sqrt(2))) * sin(direction.phi + M_PI / 4),
		size / 2 * cos(direction.theta + atan(sqrt(2)))
	};
	vertices[0] = vertex;
	cube_sides[0].vertices[0] = vertex;
	cube_sides[2].vertices[0] = vertex;
	cube_sides[4].vertices[0] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + atan(sqrt(2))) * cos(direction.phi - M_PI / 4),
		size / 2 * sin(direction.theta + atan(sqrt(2))) * sin(direction.phi - M_PI / 4),
		size / 2 * cos(direction.theta + atan(sqrt(2)))
	};
	vertices[1] = vertex;
	cube_sides[0].vertices[1] = vertex;
	cube_sides[2].vertices[1] = vertex;
	cube_sides[5].vertices[0] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + atan(sqrt(2))) * cos(direction.phi - M_PI * 3 / 4),
		size / 2 * sin(direction.theta + atan(sqrt(2))) * sin(direction.phi - M_PI * 3 / 4),
		size / 2 * cos(direction.theta + atan(sqrt(2)))
	};
	vertices[2] = vertex;
	cube_sides[0].vertices[2] = vertex;
	cube_sides[3].vertices[0] = vertex;
	cube_sides[5].vertices[1] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + atan(sqrt(2))) * cos(direction.phi + M_PI * 3 / 4),
		size / 2 * sin(direction.theta + atan(sqrt(2))) * sin(direction.phi + M_PI * 3 / 4),
		size / 2 * cos(direction.theta + atan(sqrt(2)))
	};
	vertices[3] = vertex;
	cube_sides[0].vertices[3] = vertex;
	cube_sides[3].vertices[1] = vertex;
	cube_sides[4].vertices[1] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + atan(M_PI - sqrt(2))) * cos(direction.phi + M_PI / 4),
		size / 2 * sin(direction.theta + atan(M_PI - sqrt(2))) * sin(direction.phi + M_PI / 4),
		size / 2 * cos(direction.theta + atan(M_PI - sqrt(2)))
	};
	vertices[4] = vertex;
	cube_sides[1].vertices[0] = vertex;
	cube_sides[2].vertices[3] = vertex;
	cube_sides[4].vertices[3] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + atan(M_PI - sqrt(2))) * cos(direction.phi - M_PI / 4),
		size / 2 * sin(direction.theta + atan(M_PI - sqrt(2))) * sin(direction.phi - M_PI / 4),
		size / 2 * cos(direction.theta + atan(M_PI - sqrt(2)))
	};
	vertices[5] = vertex;
	cube_sides[1].vertices[1] = vertex;
	cube_sides[2].vertices[2] = vertex;
	cube_sides[5].vertices[3] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + atan(M_PI - sqrt(2))) * cos(direction.phi - M_PI * 3 / 4),
		size / 2 * sin(direction.theta + atan(M_PI - sqrt(2))) * sin(direction.phi - M_PI * 3 / 4),
		size / 2 * cos(direction.theta + atan(M_PI - sqrt(2)))
	};
	vertices[6] = vertex;
	cube_sides[1].vertices[2] = vertex;
	cube_sides[3].vertices[3] = vertex;
	cube_sides[5].vertices[2] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + atan(M_PI - sqrt(2))) * cos(direction.phi + M_PI * 3 / 4),
		size / 2 * sin(direction.theta + atan(M_PI - sqrt(2))) * sin(direction.phi + M_PI * 3 / 4),
		size / 2 * cos(direction.theta + atan(M_PI - sqrt(2)))
	};
	vertices[7] = vertex;
	cube_sides[1].vertices[3] = vertex;
	cube_sides[3].vertices[2] = vertex;
	cube_sides[4].vertices[2] = vertex;

	for (int i = 0; i < 8; i++) {
		vertices[i].x += center.x;
		vertices[i].y += center.y;
		vertices[i].z += center.z;
		std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
	}

	std::cout << "----------------" << direction.theta << std::endl;
}

bool Cube::checkHolding(Point fingertips[5]) {
	int in_contact = 0;

	for (Plane& cube_side : cube_sides) {
		for (int i = 0; i < 5; ++i) {
			if (cube_side.distanceToPoint({fingertips[i].x - center.x, fingertips[i].y - center.y, fingertips[i].z - center.z}) <= FINGERTIP_ERROR) {
				++in_contact;
				break;
			}
		}
	}

	return (in_contact >= 2);
}