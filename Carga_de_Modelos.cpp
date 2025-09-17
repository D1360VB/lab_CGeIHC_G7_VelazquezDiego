// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

float movX=0.0f;
float movY=0.0f;
float movZ=-50.0f;
float rotY = 0.0f;
float rotX = 0.0f;




// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

///////
GLuint VBO, VAO, EBO;
GLuint shaderProgram;

// Vertex Shader
static const char* myVertexShaderColor = R"( 
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

// Fragment Shader
static const char* myFragmentShaderColor = R"( 
#version 330 core
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    
  vec4   texColor= texture(texture_diffuse1, TexCoords);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
})";


void resize(GLFWwindow* window, int SCR_WIDTH, int SCR_HEIGHT) {
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void my_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



GLuint compileShader(const char* src, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    return shader;
}

void setupShaders() {
    GLuint vtx = compileShader(myVertexShaderColor, GL_VERTEX_SHADER);
    GLuint frag = compileShader(myFragmentShaderColor, GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vtx);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);

    glDeleteShader(vtx);
    glDeleteShader(frag);
}



int main( )
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Previo 6 Velazquez Balandra Diego", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
    glewInit();

    glEnable(GL_DEPTH_TEST);   // <- ACTIVA el z-buffer


    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    Model dog((char*)"Models/dog/RedDog.obj");
    Model skull((char*)"Models/skull/12140_Skull_v3_L2.obj");
    Model cat((char*)"Models/cat/12221_Cat_v1_l3.obj");
    Model table((char*)"Models/table/SIMPLE ROUND TABLE.obj");
    Model grenade((char*)"Models/grenade/Grenade.obj");

    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCR_WIDTH/( float )SCR_HEIGHT, 0.1f, 100.0f );
    
  

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 modelDog1 = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog1));
        table.Draw(shader);

        // Draw dog scaled + translated
        glm::mat4 modelDog2 = glm::mat4(1.0f);
        modelDog2 = glm::translate(modelDog2, glm::vec3(1.0f, 0.0f, 0.0f));
        modelDog2 = glm::scale(modelDog2, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog2));
        dog.Draw(shader);



        

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return EXIT_SUCCESS;
}


 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 1.4f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 1.4f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 1.4f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 1.4f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 1.4f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 1.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rotY += 1.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rotY -= 1.4f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		 rotX += 1.4f;
	 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		 rotX -= 1.4f;
 }

 

