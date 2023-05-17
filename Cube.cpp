#include "Cube.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#define FINGERTIP_ERROR 5

void Cube::addCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
	int screenWidth = 1920;
	int screenHeight = 1080;

	// Sets index of vertices for cube (Total of 4 vertices)
	this->vertexStart = vertices.size();
	this->vertexEnd = vertexStart + 8 * 6;

	// Sets positions of each vertex (stride of 3) and color (stride of 3)
	std::vector<GLfloat> newVertices;

	for (int i = 0; i < 8; i++) {

		// Positions with stride of 3
		newVertices.push_back(this->vertices[i].x / screenWidth);
		newVertices.push_back(this->vertices[i].y / screenWidth);
		newVertices.push_back(this->vertices[i].z / screenHeight);

		// Color with stride of 3
		newVertices.push_back(color.red + i / 10.0f);
		newVertices.push_back(color.green + i / 10.0f);
		newVertices.push_back(color.blue + i / 10.0f);

	}

	// Sets index of indices for cube (Total of 6 indices)
	this->indexStart = indices.size();
	this->indexEnd = indexStart + 12 * 3;



	// Sets vertex indices for the indices vector to be appended
	std::vector<GLuint> newIndices = {
		// Top Face
		vertexStart + 0, vertexStart + 1, vertexStart + 2,
		vertexStart + 0, vertexStart + 2, vertexStart + 3,

		// Side Face
		vertexStart + 0, vertexStart + 4, vertexStart + 5,
		vertexStart + 0, vertexStart + 1, vertexStart + 5,

		vertexStart + 0, vertexStart + 4, vertexStart + 7,
		vertexStart + 0, vertexStart + 3, vertexStart + 7,

		vertexStart + 2, vertexStart + 1, vertexStart + 5,
		vertexStart + 2, vertexStart + 6, vertexStart + 5,

		vertexStart + 2, vertexStart + 6, vertexStart + 7,
		vertexStart + 2, vertexStart + 3, vertexStart + 7,

		// Bottom Face
		vertexStart + 4, vertexStart + 5, vertexStart + 6,
		vertexStart + 4, vertexStart + 6, vertexStart + 7
	};

	// Merges new vertices and indices to current vertices and indicies vector
	vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
	indices.insert(indices.end(), newIndices.begin(), newIndices.end());

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
	side.constant = size * size / 4;
	side.center = { side.coeff_x * size / 2, side.coeff_y * size / 2, side.coeff_z * size / 2 };
	cube_sides[0] = side;
	/* flip plane to opposite side */
	side.constant *= -1;
	side.center = { -side.coeff_x * size / 2, -side.coeff_y * size / 2, -side.coeff_z * size / 2 };
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
	side.constant = size * size / 4;
	side.center = { side.coeff_x * size / 2, side.coeff_y * size / 2, side.coeff_z * size / 2 };
	cube_sides[2] = side;
	/* flip plane to opposite side */
	side.constant *= -1;
	side.center = { -side.coeff_x * size / 2, -side.coeff_y * size / 2, -side.coeff_z * size / 2 };
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
	side.constant = size * size / 4;
	side.center = { side.coeff_x * size / 2, side.coeff_y * size / 2, side.coeff_z * size / 2 };
	cube_sides[4] = side;
	/* flip plane to opposite side */
	side.constant *= -1;
	side.center = { -side.coeff_x * size / 2, -side.coeff_y * size / 2, -side.coeff_z * size / 2 };
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
		size / 2 * sin(direction.theta + M_PI + atan(sqrt(2))) * cos(direction.phi - M_PI * 3 / 4),
		size / 2 * sin(direction.theta + M_PI + atan(sqrt(2))) * sin(direction.phi - M_PI * 3 / 4),
		size / 2 * cos(direction.theta + M_PI + atan(sqrt(2)))
	};
	vertices[4] = vertex;
	cube_sides[1].vertices[0] = vertex;
	cube_sides[2].vertices[3] = vertex;
	cube_sides[4].vertices[3] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + M_PI + atan(sqrt(2))) * cos(direction.phi + M_PI * 3 / 4),
		size / 2 * sin(direction.theta + M_PI + atan(sqrt(2))) * sin(direction.phi + M_PI * 3 / 4),
		size / 2 * cos(direction.theta + M_PI + atan(sqrt(2)))
	};
	vertices[5] = vertex;
	cube_sides[1].vertices[1] = vertex;
	cube_sides[2].vertices[2] = vertex;
	cube_sides[5].vertices[3] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + M_PI + atan(sqrt(2))) * cos(direction.phi + M_PI / 4),
		size / 2 * sin(direction.theta + M_PI + atan(sqrt(2))) * sin(direction.phi + M_PI / 4),
		size / 2 * cos(direction.theta + M_PI + atan(sqrt(2)))
	};
	vertices[6] = vertex;
	cube_sides[1].vertices[2] = vertex;
	cube_sides[3].vertices[3] = vertex;
	cube_sides[5].vertices[2] = vertex;

	vertex = {
		size / 2 * sin(direction.theta + M_PI + atan(sqrt(2))) * cos(direction.phi - M_PI / 4),
		size / 2 * sin(direction.theta + M_PI + atan(sqrt(2))) * sin(direction.phi - M_PI / 4),
		size / 2 * cos(direction.theta + M_PI + atan(sqrt(2)))
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
			if (cube_side.distanceToPoint({ fingertips[i].x - center.x, fingertips[i].y - center.y, fingertips[i].z - center.z }) <= FINGERTIP_ERROR) {
				++in_contact;
				break;
			}
		}
	}

	return (in_contact >= 2);
}