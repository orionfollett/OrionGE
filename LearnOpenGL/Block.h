#pragma once
#include "Graphics.h";
#include "Physics.h";
enum TextureT { CONTAINER, DIRT, DEFAULT, KATE, SPACE };
//pos is bottom back left
class Block {

public:
	TextureT texture;
	Cube collisionCube; //in grid coords
	int width; //in grid coordinates

	Block(TextureT tex, Cube colCube, int std_width = 1) {
		texture = tex;
		collisionCube = colCube;
		width = std_width;
	}

	Block() {
		texture = DEFAULT;
		collisionCube = Cube();
		width = 1;
	}

	//scale factor * world coord = grid coord
	void DrawBlock(Graphics* context, float scaleFactor = 2.0f) {
		float w = collisionCube.width / scaleFactor;

		//slower one, not really slower at the moment

		context->drawBox(
			glm::vec3(collisionCube.pos.x / scaleFactor, collisionCube.pos.y / scaleFactor, collisionCube.pos.z / scaleFactor),
			glm::vec3(w, w, w),
			0.0f,
			glm::vec3(1.0f, 0.0f, 0.0f),
			texture);
	}

	void DrawPartialBlock(Graphics* context, bool faces[6], float scaleFactor = 2.0f) {
		float w = collisionCube.width / scaleFactor;
		context->drawPartialBox(
			glm::vec3(collisionCube.pos.x / scaleFactor, collisionCube.pos.y / scaleFactor, collisionCube.pos.z / scaleFactor),
			glm::vec3(w, w, w),
			texture,
			faces);
	}
};