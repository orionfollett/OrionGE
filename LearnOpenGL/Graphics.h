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
#include <vector>

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
    - Physics
    - Better performance

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

static float cubeFaces[6][30] = 
{ 
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    }, 
{
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        },
    {
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        },
    {
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         },
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        },
    {
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    }
};

//Mouse Global Variables
static float mousePosX;
static float mousePosY;
static bool isMouseMoved = false;

class Graphics {

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
    std::shared_ptr<Shader> instancedShader;

    unsigned int VBO, rectVAO, boxVAO, lineVAO;
    unsigned int facesVAO[6];
    std::unordered_map<unsigned int, unsigned int> fbVAOs; //map of {VAO, VBO}
    

    //texture map, maps user created enum to texture id
    std::unordered_map<int, unsigned int> textureMap;

    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;

    bool beginRenderCalled = false;
    float lastTime = 0.0f;

public:

    // camera
    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

    Graphics(unsigned int windowWidth = 400, unsigned int windowHeight = 400)
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
        instancedShader.reset(new Shader("vertexInstanced.txt", "fragment.txt"));

        //rect buffers
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


        //faces
        for (int i = 0; i < 6; i++) {
            glGenBuffers(1, &VBO);
            glGenVertexArrays(1, &facesVAO[i]);
            glBindVertexArray(facesVAO[i]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeFaces[i]), cubeFaces[i], GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);//unbind
        }
        

        //box buffers
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

        //line buffers
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &VBO);
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
        glDeleteVertexArrays(1, &lineVAO);

        glDeleteVertexArrays(6, &facesVAO[0]);

        for (auto & i : fbVAOs) {
            glDeleteBuffers(1, &i.second);
            glDeleteVertexArrays(1, &i.first);
        }
        
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

    ////returns the center of the screen in world coordinates
    //glm::vec2 getMouseCoord() 
    //{
    //    return glm::vec2(mousePosX, mousePosY);
    //}

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

        //global variable activated by static mouse callback
        if (isMouseMoved) {
            camera.ProcessMouseMovement(mousePosX, mousePosY);
            isMouseMoved = false;
        }

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        instancedShader->use();
        instancedShader->setMat4("projection", projection);
        instancedShader->setMat4("view", view);
        
        // activate shader
        ourShader->use();
        ourShader->setMat4("projection", projection);
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

            //rotate would go here
            if (!(axisOfRotation.x == 0 && axisOfRotation.y == 0 && axisOfRotation.z == 0)) {
                model = glm::rotate(model, glm::radians(angle), axisOfRotation);
            }
            
            
            model = glm::translate(model, pos);

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

            

            model = glm::translate(model, pos);
            
            model = glm::scale(model, dimensions);
            //model[0][0] = dimensions.x;
            //model[1][1] = dimensions.y;
            //model[2][2] = dimensions.z;


            //rotate would go here
            if (!(axisOfRotation.x == 0 && axisOfRotation.y == 0 && axisOfRotation.z == 0)) {
                model = glm::rotate(model, glm::radians(angle), axisOfRotation);
            }

            ourShader->setMat4("model", model);

            glBindVertexArray(boxVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
        else {
            std::cout << "Texture could not be found!" << std::endl;
        }
    }

    //draw line
    void drawLine(glm::vec3 start, glm::vec3 end) {
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        float lineVertices[] = {start.x, start.y, start.z, end.x, end.y, end.z};
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

        //model = glm::translate(model, start);

        //model = glm::scale(model, {1, 1, 1});
        ourShader->setMat4("model", model);
        glBindVertexArray(lineVAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }


    //------------------------------------------------Optimized Draw Box ---------------------------------------------
    //*pos -> array of positions for the boxes
    //numBoxes -> length of pos array
    //called once before program runs
    void InitChunkDraw(std::vector<glm::vec3> pos, glm::vec3 blockDimensions, unsigned int * chunkId) {
        if (pos.size() <= 0) {
            return;
        }
       
        //generate fastBox VAO
        unsigned int fastBoxVAO;
        glGenVertexArrays(1, &fastBoxVAO);

        glBindVertexArray(fastBoxVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        //transform positions into model transformation matrices
        glm::mat4* modelMatrices = new glm::mat4[pos.size()];
        for (unsigned int i = 0; i < pos.size(); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos[i]);
            modelMatrices[i] = glm::scale(model, blockDimensions);
        }

        //buffer the data
        //glGenBuffers(1, &fastBoxBuffer);
        //glBindBuffer(GL_ARRAY_BUFFER, fastBoxBuffer);
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        *chunkId = fastBoxVAO;
        fbVAOs.insert({ fastBoxVAO, buffer});

        float start = glfwGetTime();
        glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
        float end = glfwGetTime();

        // set attribute pointers for matrix (4 times vec4)
        glBindVertexArray(fastBoxVAO);

        std::size_t vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
        

        std::cout << "Buffer Time on INIT" << end - start << std::endl;
    }


    void ModifyChunkBuffer(std::vector<glm::vec3> pos, glm::vec3 dimensions, unsigned int chunkId) {
        glm::mat4 * modelMatrices = new glm::mat4[pos.size()];

        for (unsigned int i = 0; i < pos.size(); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos[i]);
            modelMatrices[i] = glm::scale(model, dimensions);
        }

        //glBindBuffer(GL_ARRAY_BUFFER, fastBoxBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, fbVAOs.at(chunkId));
        glBufferSubData(GL_ARRAY_BUFFER, 0, pos.size()  * sizeof(glm::mat4), &modelMatrices[0]);
        
        delete modelMatrices;
    }

    void DrawChunk(unsigned int chunkId, int numBoxes, int textureEnum) {
        instancedShader->use();
        if (textureMap.find(textureEnum) != textureMap.end()) {
            instancedShader->use();
            unsigned int texId = textureMap[textureEnum];

            instancedShader->setInt("texture1", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texId);

            glBindVertexArray(chunkId);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, numBoxes);
            //glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100);
            glBindVertexArray(0);
        }
        else {
            std::cout << "Texture could not be found!" << std::endl;
        }

        ourShader->use();
    }

    //Faces guide
    //0 -> front
    //1 -> back
    //2 -> left
    //3 -> right
    //4 -> bottom
    //5 -> top

    //draw Partial Box
    void drawPartialBox(glm::vec3 pos, glm::vec3 dimensions, int textureEnum, bool faces[6]) {
        if (textureMap.find(textureEnum) != textureMap.end()) {
            unsigned int texId = textureMap[textureEnum];

            ourShader->setInt("texture1", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texId);

            
            //calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

            model = glm::translate(model, pos);
            model = glm::scale(model, dimensions);
            ourShader->setMat4("model", model);


            //add only vertices that the faces says too
            for(int i = 0; i < 6; i++)
            {
                if (faces[i]) {
                    glBindVertexArray(facesVAO[i]);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                    glBindVertexArray(0);
                }
            }
        }
        else {
            std::cout << "Texture could not be found!" << std::endl;
        }
    }

    //deltaTime is the time it took this frame to render
    void showFPS(float deltaTime)
    {
        float currentTime = glfwGetTime();
        if (currentTime > lastTime + 0.5f) {
            lastTime = currentTime;
            double fps = int(1 / deltaTime);
            std::stringstream ss;
            ss << "OrionGE" << " " << " [" << fps << " FPS]";

            glfwSetWindowTitle(window, ss.str().c_str());
        }
    }
};


#endif
