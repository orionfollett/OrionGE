#include "OrionGE.h";

enum TextureT {CONTAINER, SMILEY, DEFAULT};

void processInput(OrionGE* context, float deltaTime) {
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
}

//int mainFunction()
int main() 
{
	OrionGE context = OrionGE(800, 800);

	context.loadTexture(CONTAINER, "./container.jpg", false);
	context.loadTexture(SMILEY, "./awesomeface.png", true);
	context.loadTexture(DEFAULT, "./default.png", false);
	
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
		context.drawBackground(0.2f, 0.2f, 0.2f, 1.0f);

		context.drawRectangle(glm::vec3(1.0f, 0.0f, 0.0f), 0.5f, 1.0f,45.0f, glm::vec3(1.0f, 0.0f, 0.0f), CONTAINER );
		context.drawRectangle(glm::vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.5f, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f), DEFAULT);
		context.drawRectangle(glm::vec3(-1.0f, 0.0f, 0.0f), 0.1f, 2.0f, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), SMILEY);

		//get input and draw buffers to the screen
		context.GetInput();
		context.Display();
	}

	context.cleanUp();
	return 0;
}