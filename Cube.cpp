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
		newVertices.push_back(this->vertices[i].z / screenWidth);

		// Color with stride of 3
		newVertices.push_back(color.red + i / 10.0f);
		newVertices.push_back(color.green + i / 10.0f);
		newVertices.push_back(color.blue + i / 10.0f);

	}

	// Sets index of indices for cube (Total of 6 indices)
	this->indexStart = indices.size();
	this->indexEnd = indexStart + 12 * 3;

	GLuint indexNum = vertexStart / 6;

	// Sets vertex indices for the indices vector to be appended
	std::vector<GLuint> newIndices = {
		// Top Face
		indexNum + 0, indexNum + 1, indexNum + 2,
		indexNum + 0, indexNum + 2, indexNum + 3,

		// Side Face
		indexNum + 0, indexNum + 4, indexNum + 5,
		indexNum + 0, indexNum + 1, indexNum + 5,

		indexNum + 0, indexNum + 4, indexNum + 7,
		indexNum + 0, indexNum + 3, indexNum + 7,

		indexNum + 2, indexNum + 1, indexNum + 5,
		indexNum + 2, indexNum + 6, indexNum + 5,

		indexNum + 2, indexNum + 6, indexNum + 7,
		indexNum + 2, indexNum + 3, indexNum + 7,

		// Bottom Face
		indexNum + 4, indexNum + 5, indexNum + 6,
		indexNum + 4, indexNum + 6, indexNum + 7
	};

	// Merges new vertices and indices to current vertices and indicies vector
	vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
	indices.insert(indices.end(), newIndices.begin(), newIndices.end());

}

Cube::Cube(Type _blockType, int id) : center({ 0,0,0 }), direction({ 0,0 }), ObjectRender({ 0, 0, 0 }, id) {
	updatePlanes();

	if (_blockType == BLOCK)
		this->size = 300;
	else if (_blockType == ITEM)
		this->size = 100;
	else
		throw("Unknown block type");
}

Cube::Cube(Point _center, Type _blockType, Orientation _direction, Color _color, int id) : center(_center), direction(_direction), ObjectRender(_color, id) {
	updatePlanes();

	if (_blockType == BLOCK)
		this->size = 300;
	else if (_blockType == ITEM)
		this->size = 100;
	else
		throw("Unknown block type");
}

Coordinate Cube::getCubeCoordinate() {
	return { (int)(center.x / 173.2f), (int)(center.y / 173.2f), (int)(center.z / 173.2f) };
}

Point Cube::getCubeCenter() {
	return center;
}

void Cube::setCubeCoordinate(Coordinate loc) {
	center = {
				loc.x * 173.2,
				loc.y * 173.2,
				loc.z * 173.2
			}; // 173.2f is the cube's side length

	updatePlanes();
}

void Cube::setCubeCoordinate(int x, int y, int z) {
	center.x = x * 173.2;
	center.y = y * 173.2;
	center.z = z * 173.2;
	updatePlanes();
}

void Cube::setCubeCenter(Point loc) {
	center = loc;
	updatePlanes();
}

void Cube::setCubeCenter(double x, double y, double z) {
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
		//std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
	}

	//std::cout << "----------------" << direction.theta << std::endl;
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