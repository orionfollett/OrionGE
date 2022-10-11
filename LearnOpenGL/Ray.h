#pragma once
#include "Graphics.h";

class Ray {

public:
	glm::vec3 start;
	glm::vec3 end;

	Ray(glm::vec3 start_pos, glm::vec3 end_pos, float scale = 2.0f) {
		start = start_pos;
		end = end_pos;
	}

	void DrawRay(Graphics* context, float scaleFactor = 2.0f) {
		context->drawLine(
			glm::vec3(start.x / scaleFactor, start.y / scaleFactor, start.z / scaleFactor),
			glm::vec3(end.x / scaleFactor, end.y / scaleFactor, end.z / scaleFactor)
		);
	}


};