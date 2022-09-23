#include "Graphics.h";
#include "Physics.h";

void processInput(Graphics* context, float deltaTime);
enum TextureT {CONTAINER, DIRT, DEFAULT, KATE, SPACE};

//int mainFunction()
int main() 
{
	Graphics context = Graphics(800, 800);

	context.loadTexture(CONTAINER, "./assets/container.jpg", false);
	context.loadTexture(DIRT, "./assets/dirt.png", false);
	context.loadTexture(DEFAULT, "./assets/default.png", false);
	context.loadTexture(KATE, "./assets/kate.png", true);
	context.loadTexture(SPACE, "./assets/space.png", true);
	
	//init physics objects
	Cube b1 = Cube(glm::vec3(-5.0f, 3.0f, 4.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
	//Cube b2 = Cube(glm::vec3(4.0f, 20.0f, 4.0f), glm::vec3(0.0f, -0.75f, 0.0f), 0.5f);

	//init world
	const int worldSize = 9;
	
	//Cube world[worldSize][worldSize][worldSize];
	bool world[worldSize][worldSize][worldSize];
	for (int i = 0; i < worldSize; i++) {
		for (int j = 0; j < worldSize; j++) {
			for (int k = 0; k < worldSize; k++) {
				if (k % 2 == 0 && i % 2 == 0) {
					world[i][j][k] = true;
				}
				else {
					world[i][j][k] = j == 0;
				}
			}
		}
	}

	bool result = Physics::PointVsWorld(b1.pos, world);


	
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

		//do physics things
		b1.applyVelocity(deltaTime);
		//b2.applyVelocity(deltaTime);

		//glm::vec2 mouseCoord = context.getMouseCoord();
		//std::cout << std::to_string(mouseCoord.x) + " " + std::to_string(mouseCoord.y) << std::endl;

		//render things into memory
		context.BeginRender();
		context.drawBackground(0.0f, 0.0f, 0.0f, 1.0f);
		
		//pick what things to draw
		
		//context.drawBox(b1.pos, glm::vec3(b1.width, b1.width, b1.width), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), DEFAULT);
		//context.drawBox(b2.pos, glm::vec3(b2.width, b2.width, b2.width), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), DEFAULT);
		
		context.drawBox(b1.pos, glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), DEFAULT);
		context.drawBox(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), CONTAINER);
		if (Physics::PointVsWorld(b1.pos, world)) {
			std::cout << std::to_string(deltaTime) << std::endl;
		}
		//where are the corners of the box?

		//context.drawBox(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), DEFAULT);

		for (int i = 0; i < worldSize; i++) {
			for (int j = 0; j < worldSize; j++) {
				for (int k = 0; k < worldSize; k++) {
					if (world[i][j][k]) {
						glm::vec3 pos = { i, j, k };
						context.drawBox(pos, glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), CONTAINER);
					}
				}
			}
		}

		//get input and draw buffers to the screen
		context.GetInput();
		context.Display();
	}

	context.cleanUp();
	return 0;
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
