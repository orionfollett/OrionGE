#pragma once

#include <vector>;
#include "Graphics.h";

template <class T> class Array3D {
private:
	std::vector<T> data;
	std::size_t xDim;
	std::size_t yDim;
	std::size_t zDim;

public:
	Array3D(std::size_t xDim, std::size_t yDim, std::size_t zDim, T defaultVal) {
		this->xDim = xDim;
		this->yDim = yDim;
		this->zDim = zDim;
		this->data.reserve(xDim * yDim * zDim);

		for (int i = 0; i < xDim * yDim * zDim; i++) {
			this->data.push_back(defaultVal);
		}
	}

	T at(int x, int y, int z) {
		//no checking bounds makes it faster, make sure your indices are correct though!
		return this->data[x + y * xDim + z * xDim * yDim];
	}

	void put(int x, int y, int z, T val) {
		this->data[x + y * xDim + z * xDim * yDim] = val;
	}

	void put(int i, T val) {
		this->data[i] = val;
	}

	int length() {
		return this->data.size();
	}

	//get vector from flat index
	glm::vec3 index(int idx) {	
		int z = idx / (xDim * yDim);
		idx -= (z * xDim * yDim);
		int y = idx / xDim;
		int x = idx % xDim;
		return { x, y, z };
	}
};
