/*
Orion's Game Engine: By Orion Follett 
(with help from learnopengl)

Started: 9/3/2022
*/

#ifndef ORIONGE_H
#define ORIONGE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h";
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h";
#include "Camera.h";
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

//things this library will handle
/*
*   Basic:
	- user input
	- window creation
	- texture and image loading
	- model loading
	- camera
	- drawing rectangles, cubes, and custom obj files of variable sizes

    Advanced:
    - Lights
    - 3D Animations
    - 2D animations


    Naming convention
    functions that almost always need to be called once per frame 
    are full camel case

    functions that are optional are first word lower case camel case
*/

/*
    positive z axis is out the screen towards you
    pos x is right
    pos y is up
*/

//standard shape vertices
static float rectangleVertices[] = {
    // positions         // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
};

static float boxVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

//Mouse Global Variables
static float mousePosX;
static float mousePosY;
static bool isMouseMoved = false;

class OrionGE {

private:

    //functions
    // 
    // 
    //CALLBACKS
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }


    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        mousePosX = static_cast<float>(xposIn);
        mousePosY = static_cast<float>(yposIn);
        isMouseMoved = true;
    }

    //private variables

    GLFWwindow* window;
    std::shared_ptr<Shader> ourShader;
    unsigned int VBO, rectVAO, boxVAO;

    //texture map, maps user created enum to texture id
    std::unordered_map<int, unsigned int> textureMap;

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;

    bool beginRenderCalled = false;

public:

    // camera
    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    OrionGE(unsigned int windowWidth = 400, unsigned int windowHeight = 400)
    {
        SCR_WIDTH = windowWidth;
        SCR_HEIGHT = windowHeight;

        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        // --------------------
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OrionGE Game", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        //glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);

        // build and compile our shader zprogram
        // ------------------------------------
        ourShader.reset(new Shader("vertex.txt", "fragment.txt"));

        
        glGenBuffers(1, &VBO);
        
        glGenVertexArrays(1, &rectVAO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);//unbind

        glGenVertexArrays(1, &boxVAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(boxVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

    }

    //-----------------------------------Window Functions------------------------------------------------

    //checks if the window X button has been clicked
    bool windowShouldClose() 
    {
        return (bool)glfwWindowShouldClose(window);
    }

    //tells windowShouldClose to start returning false
    void closeWindow() 
    {
        glfwSetWindowShouldClose(window, true);
    }

    //clean up memory, actually kills the window
    void cleanUp() 
    {
        glDeleteVertexArrays(1, &rectVAO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &boxVAO);
        glfwTerminate();
    }

    float getTime() 
    {
        return static_cast<float>(glfwGetTime());
    }

    //----------------------------------User Input----------------------------------------

    //needs to be called every frame in order to capture user input
    void GetInput() 
    {
        glfwPollEvents();
    }

    //returns true if the key is being pressed, false otherwise, 
    //accepts a GLFW_KEY_X (I'm too lazy to type out all of the keys)
    bool isKeyPressed(int glfwKey) 
    {
        return (bool) glfwGetKey(window, glfwKey);
    }

    //--------------------------------Graphics---------------------------------------------

    //display - needs to be called once per frame
    //Draw everything loaded into memory on the screen
    void Display() 
    {
        if (!beginRenderCalled) 
        {
            std::cout << "BeginRender must be called in render loop before any rendering!" << std::endl;
        }
        else {
            beginRenderCalled = false;
        }

        glfwSwapBuffers(window);
    }

    //must call this before any of the rendering happens
    void BeginRender() 
    {
        beginRenderCalled = true;

        // activate shader
        ourShader->use();

        //global variable activated by static mouse callback
        if (isMouseMoved) {
            camera.ProcessMouseMovement(mousePosX, mousePosY);
            isMouseMoved = false;
        }

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader->setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader->setMat4("view", view);
    }

    //Draw Background
    void drawBackground(float red, float green, float blue, float alpha) 
    {
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //load texture
    void loadTexture(int textureEnum, const char * texturePath, bool hasAlpha) 
    {
        unsigned int texId;

        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        textureMap[textureEnum] = texId;

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            if (hasAlpha) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    //drawRectangle
    /*
        draws a rectangle in 3D space
        vec3 pos - > (x, y, z)
        float angle
        vec3 axis of rotation
    */
    void drawRectangle(glm::vec3 pos, float width, float height, float angle, glm::vec3 axisOfRotation, int textureEnum)
    {  
        if (textureMap.find(textureEnum) != textureMap.end()) {
            unsigned int texId = textureMap[textureEnum];

            ourShader->setInt("texture1", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texId);
           
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

            model[0][0] = width;
            model[1][1] = height;

            model = glm::translate(model, pos);

            //rotate would go here
            model = glm::rotate(model, glm::radians(angle), axisOfRotation);
            
            ourShader->setMat4("model", model);

            glBindVertexArray(rectVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
        else {
            std::cout << "Texture could not be found!" << std::endl; 
        }
    }

    //draw box
    /*
        Draws a box in 3D space with given coordinates (x,y,z),
        Dimensions (x (width), y (height), z (depth)),
        angle
        axis of rotation
        and Texture Enum referring to a previously loaded texture (see loadTexture())
    */
    void drawBox(glm::vec3 pos, glm::vec3 dimensions, float angle, glm::vec3 axisOfRotation, int textureEnum) {
        if (textureMap.find(textureEnum) != textureMap.end()) {
            unsigned int texId = textureMap[textureEnum];

            ourShader->setInt("texture1", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texId);
            
            //calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

            model = glm::scale(model, dimensions);

            model = glm::translate(model, pos);

            //rotate would go here
            model = glm::rotate(model, glm::radians(angle), axisOfRotation);

            ourShader->setMat4("model", model);

            glBindVertexArray(boxVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
        else {
            std::cout << "Texture could not be found!" << std::endl;
        }
    }


    //draw model
};


#endif
