#include "Graphics.h";
#include "Physics.h";

void processInput(Graphics* context, float deltaTime);
void moveCube(Graphics* context, float deltaTime, Cube * c);
enum TextureT {CONTAINER, DIRT, DEFAULT, KATE, SPACE};

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

		context->drawBox(
			glm::vec3(collisionCube.pos.x / scaleFactor, collisionCube.pos.y / scaleFactor, collisionCube.pos.z / scaleFactor),
			glm::vec3(w, w, w), 
			0.0f, 
			glm::vec3(1.0f, 0.0f, 0.0f), 
			texture);
	}
};

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
	Cube c = Cube();
	Block containerBlock = Block(CONTAINER, c);
	c.pos = { 0, 3, 1 };
	Block testBlock = Block(DEFAULT, c);

	//init world
	const int worldSize = 5;
	
	//Cube world[worldSize][worldSize][worldSize];
	bool world[worldSize][worldSize][worldSize];
	for (int i = 0; i < worldSize; i++) {
		for (int j = 0; j < worldSize; j++) {
			for (int k = 0; k < worldSize; k++) {
				if (i % 2 ==0 && k % 2 == 0 && j % 3== 0) {
					world[i][j][k] = true;
				}
				else {
					world[i][j][k] = false;
				}
			}
		}
	}

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
		moveCube(&context, deltaTime, &testBlock.collisionCube);

		//do physics things
		testBlock.collisionCube.applyVelocity(deltaTime);
		Grid::CubeStayInGrid(&testBlock.collisionCube, worldSize);

		//render things into memory
		context.BeginRender();
		context.drawBackground(0.0f, 0.0f, 0.0f, 1.0f);

		//pick what things to draw

		Grid g = Grid();

		//if (!Grid::PointVsGrid(testBlock.collisionCube.pos, world, g)) {
		if (!Grid::CubeVsGrid(testBlock.collisionCube, world)) {
			testBlock.texture = DEFAULT;
			testBlock.DrawBlock(&context);
		}
		else {
			testBlock.texture = DIRT;
			testBlock.DrawBlock(&context);
		}
		
		for (int i = 0; i < worldSize; i++) {
			for (int j = 0; j < worldSize; j++) {
				for (int k = 0; k < worldSize; k++) {
					if (world[i][j][k]) {
						containerBlock.collisionCube.pos = { i, j , k };
						containerBlock.DrawBlock(&context);
						//glm::vec3 pos = { i * .5, j * .5, k * .5 };
						//context.drawBox(pos, glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), CONTAINER);
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
