#pragma once
#include "Graphics.h";
#include "Block.h";
#include "Array3D.h";
#include <deque>;

class Chunk {
public:
	static const int chunkSize = 16;
	static enum FaceT { FRONT, BACK, LEFT, RIGHT, BOTTOM, TOP }; //ORDER OF THIS ENUM MATTERS!!! RELATED TO THE INDEX OF FACES IN DRAW BOX FAST
	//Faces guide
	//0 -> front
	//1 -> back
	//2 -> left
	//3 -> right
	//4 -> bottom
	//5 -> top

private: Array3D<bool> chunk = Array3D<bool>(chunkSize, chunkSize, chunkSize, true);

//vector of vec3s, only 
private: std::vector<glm::vec3> blockPos;
private: std::deque<glm::vec3> deletions;//blocks marked for deletion

public:
	unsigned int chunkId;
	glm::vec3 pos;

	Chunk(Graphics * context, glm::vec3 pos) {
		std::cout << "Number of blocks: " << chunkSize * chunkSize * chunkSize << std::endl;
		this->pos = pos;
		for (int i = 0; i < this->chunk.length(); i++) {
			blockPos.push_back(this->chunk.index(i) + pos);
		}

		context->InitChunkDraw(blockPos, { 1, 1, 1 }, &chunkId);
		std::cout << "CHUNKID: " << chunkId << std::endl;
	}

	void Draw(Graphics* context, Block model) {
		context->DrawChunk(chunkId, Chunk::chunkSize * Chunk::chunkSize * Chunk::chunkSize, CONTAINER);
		return;

		/*
		for (int i = 0; i < chunkSize; i++) {
			for (int j = 0; j < chunkSize; j++) {
				for (int k = 0; k < chunkSize; k++) {
					if (chunk[i][j][k]) {
						bool faces[6] = { false, false, false, false, false, false };

						//LEFT AND RIGHT
						if (i == 0) {
							faces[LEFT] = true;
							faces[RIGHT] = !chunk[i + 1][j][k];
						}
						else if (i == chunkSize - 1) {
							faces[RIGHT] = true;
							faces[LEFT] = !chunk[i - 1][j][k];
						}
						else {
							faces[LEFT] = !chunk[i - 1][j][k];
							faces[RIGHT] = !chunk[i + 1][j][k];
						}

						//TOP AND BOTTOM
						if (j == 0) {
							faces[TOP] = !chunk[i][j + 1][k];
							faces[BOTTOM] = true;
						}
						else if (j == chunkSize - 1) {
							faces[TOP] = true;
							faces[BOTTOM] = !chunk[i][j - 1][k];
						}
						else {
							faces[TOP] = !chunk[i][j + 1][k];
							faces[BOTTOM] = !chunk[i][j - 1][k];
						}


						//FRONT AND BACK
						if (k == 0) {
							faces[FRONT] = true;
							faces[BACK] = !chunk[i][j][k + 1];
						}
						else if (k == chunkSize - 1) {
							faces[FRONT] = !chunk[i][j][k - 1];
							faces[BACK] = true;
						}
						else {
							faces[FRONT] = !chunk[i][j][k - 1];
							faces[BACK] = !chunk[i][j][k + 1];
						}

						model.collisionCube.pos = glm::vec3(i, j, k) + this->pos;
						model.DrawPartialBlock(context, faces);
					}
				}
			}
		}*/
	}

	//if you insert a block thats already inserted, then it will take two deletes to delete it
	void Edit(int x, int y, int z, bool isSet) {
		this->chunk.put(x, y, z, isSet);
		glm::vec3 v = pos + glm::vec3(x, y, z);
		if (isSet) {
			this->blockPos.push_back(v);
		}
		else {
			deletions.push_back(v);
		}
	}

	void Edit(int i, bool isSet) {
		glm::vec3 v = this->chunk.index(i);
		Edit(v.x, v.y, v.z, isSet);
	}

	//must be called for changes to the chunk to be applied, should be called rarely, not every frame
	//because the modify call is slow as it needs to send a chunk to the GPU
	void ApplyEdits(Graphics* context) {
		while (!deletions.empty()) {
			glm::vec3 d = deletions.front();
			std::remove(blockPos.begin(), blockPos.end(), d);
		    deletions.pop_front();
		}
		context->ModifyChunkBuffer(blockPos, { 1, 1, 1 }, chunkId);
	}

	void Delete() {
		//delete this->chunk;
	}

	//must call apply edits after doing this
	void GenerateCave() {
		for (int i = 0; i < chunkSize * chunkSize * chunkSize; i++) {
			bool isSet;
			if (std::rand() % 2 == 0) {
				this->Edit(i, false);
			}
		}
	}
};