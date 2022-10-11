#include "Graphics.h";
#include "Physics.h";
#include "Ray.h";
#include "Block.h";
#include "Chunk.h";
#include "Array3D.h";


void processInput(Graphics* context, float deltaTime);
void moveCube(Graphics* context, float deltaTime, Cube * c);

//int mainFunction()
int main() 
{
	Graphics context = Graphics(800, 800);
	context.camera.Position = {0, 40, 0};
	
	context.loadTexture(CONTAINER, "./assets/container.jpg", false);
	context.loadTexture(DIRT, "./assets/dirt.png", false);
	context.loadTexture(DEFAULT, "./assets/default.png", false);
	context.loadTexture(KATE, "./assets/kate.png", true);
	context.loadTexture(SPACE, "./assets/space.png", true);
	
	std::srand(1);
	//init chunk for testing
	Cube c = Cube();
	Block containerBlock = Block(CONTAINER, c);
	Chunk testChunk2 = Chunk(&context, { 0, 16, 0 });

	Chunk testChunk = Chunk(&context, {0, 0, 0});
	testChunk.GenerateCave();
	testChunk.ApplyEdits(&context);
	
	//testChunk.GenerateCave();
	//testChunk.ApplyEdits(&context);

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	while (!context.windowShouldClose()) {
		//calculate frame time and such
		float currentFrame = context.getTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//handle input
		processInput(&context, deltaTime);

		//render things into memory
		context.BeginRender();
		context.drawBackground(0.1f, 0.3f, 0.2f, 1.0f);

		//pick what things to draw
		testChunk.Draw(&context, containerBlock);
		testChunk2.Draw(&context, containerBlock);
		
		//get input and draw buffers to the screen
		context.GetInput();
		context.Display();
		context.showFPS(deltaTime);
	}

	context.cleanUp();
	testChunk.Delete();
	return 0;
}

void moveCube(Graphics* context, float deltaTime, Cube* c) {
	if (context->isKeyPressed(GLFW_KEY_I)) {
		c->vel.z = -2.5f;
	}
	else if (context->isKeyPressed(GLFW_KEY_K)) {
		c->vel.z = 2.5f;
	}
	else {
		c->vel.z = 0;
	}
	if (context->isKeyPressed(GLFW_KEY_J)) {
		c->vel.x = -2.5f;
	}
	else if (context->isKeyPressed(GLFW_KEY_L)) {
		c->vel.x = 2.5f;
	}
	else {
		c->vel.x = 0;
	}

	if (context->isKeyPressed(GLFW_KEY_U)) {
		c->vel.y = -2.5f;
	}
	else if (context->isKeyPressed(GLFW_KEY_O)) {
		c->vel.y = 2.5f;
	}
	else {
		c->vel.y = 0;
	}
}


void processInput(Graphics* context, float deltaTime) {
	if (context->isKeyPressed(GLFW_KEY_ESCAPE)) {
		context->closeWindow();
	}
	if (context->isKeyPressed(GLFW_KEY_W)) {
		context->camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (context->isKeyPressed(GLFW_KEY_S)) {
		context->camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (context->isKeyPressed(GLFW_KEY_A)) {
		context->camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (context->isKeyPressed(GLFW_KEY_D)) {
		context->camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (context->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		context->camera.MovementSpeed = 10.0f;
	}
	else {
		context->camera.MovementSpeed = 2.5f;
	}
}


/*
//init physics objects

c.pos = { 1.7, 4, 2.2 };
Block testBlock = Block(DEFAULT, c);
Ray r = Ray({ 0, 0, 0 }, { 1, 1, 1 });
*/

/*
Cube c2 = Cube({ 2, 2, 2 }, {0, 0, 0}, 1);
		Block b2 = Block(DIRT, c2);
		b2.DrawBlock(&context);

		r.start = testBlock.collisionCube.pos;
		r.end = {2.5, 0, 2};
		r.DrawRay(&context);

		float t = 0.0f;

		glm::vec3 contact_point;
		glm::vec3 contact_normal;

		if (Physics::RayVsCube(r.start, r.end, b2.collisionCube, testBlock.collisionCube.vel, t, contact_point, contact_normal)) {
			//std::cout << std::to_string(contact_normal.x) << " " << std::to_string(contact_normal.y) << " " << std::to_string(contact_normal.z) << std::endl;
		}

		if (!Grid::CubeVsGrid(testBlock.collisionCube, world)) {
			testBlock.texture = DEFAULT;
			testBlock.DrawBlock(&context);
		}
		else {
			testBlock.texture = DIRT;
			testBlock.DrawBlock(&context);
			//Physics::ResolveCubeVsCube(testBlock.collisionCube,, { 0, 0, 0 }, {0, 0, 0}, deltaTime, 0.0f);
		}

		/*
		for (int i = 0; i < worldSize; i++) {
			for (int j = 0; j < worldSize; j++) {
				for (int k = 0; k < worldSize; k++) {
					if (world[i][j][k]) {
						containerBlock.collisionCube.pos = { i, j , k };
						containerBlock.DrawBlock(&context);
					}
				}
			}
		}*/

