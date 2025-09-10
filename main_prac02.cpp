//Practica 2
//Velazquez Balandra Diego
//Fecha de entrega: 24 de agosto 2025
//Número de cuenta: 421089824

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLuint VBO, VAO, EBO;
GLuint shaderProgram;

// Vertex Shader
static const char* myVertexShaderColor = R"( 
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
})";

// Fragment Shader
static const char* myFragmentShaderColor = R"( 
#version 330 core
in vec3 ourColor;
out vec4 FragColor;
void main()
{
    FragColor = vec4(ourColor, 1.0);
})";

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void my_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void addSquare(float x, float y, float s, float r, float g, float b,
               std::vector<float>& vertices,
               std::vector<unsigned int>& indices,
               unsigned int& offset)           // <-- unsigned int&
{
    float hs = s / 2.0f;

    float verts[] = {
        x - hs, y - hs, 0.0f,  r, g, b,
        x + hs, y - hs, 0.0f,  r, g, b,
        x + hs, y + hs, 0.0f,  r, g, b,
        x - hs, y + hs, 0.0f,  r, g, b
    };
    vertices.insert(vertices.end(), verts, verts + 24);

    unsigned int inds[] = {
        offset, offset+1, offset+2,
        offset, offset+2, offset+3
    };
    indices.insert(indices.end(), inds, inds + 6);

    offset += 4;
}


// 0 = vacío, 1=negro, 2=azul, 3=piel, 4=blanco, 5=rojo
int sonic[32][32] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 2, 2, 2, 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 2, 2, 1, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 2, 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 1, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 4, 4, 4, 4, 1, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 1, 4, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 1, 2, 3, 3, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 1, 3, 3, 3, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 3, 3, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 3, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 5, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 5, 5, 1, 1, 5, 5, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 5, 4, 5, 1, 1, 5, 5, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 4, 4, 5, 5, 5, 5, 1, 5, 5, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    //{ 0, 0, 0, 0, 0, 0, 0, 4, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};


void getColor(int code, float& r, float& g, float& b) {
    switch(code) {
        case 1: r=0.0f; g=0.0f; b=0.0f; break;          // negro
        case 2: r=0.0f; g=0.4f; b=1.0f; break;          // azul
        case 3: r=1.0f; g=0.8f; b=0.6f; break;          // piel
        case 4: r=1.0f; g=1.0f; b=1.0f; break;          // blanco
        case 5: r=1.0f; g=0.0f; b=0.0f; break;          // rojo
        default: r=g=b=0.0f; break;
    }
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

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 2 Velazquez Balandra Diego", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
    glewInit();

    // Data
std::vector<float> vertices;
std::vector<unsigned int> indices;

unsigned int offset = 0;          // <-- UNA sola vez
float pixelSize = 0.05f;          // ajusta al gusto

for (int row = 0; row < 32; ++row) {
    for (int col = 0; col < 32; ++col) {
        int code = sonic[row][col];
        if (code != 0) {
            float r,g,b; getColor(code, r,g,b);
            float x = (col - 16) * pixelSize;   // centra en X
            float y = (16 - row) * pixelSize;   // centra en Y
            addSquare(x, y, pixelSize, r, g, b, vertices, indices, offset);
        }
    }
}


    // Buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Atributos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Shaders
    setupShaders();

    // Loop
    while (!glfwWindowShouldClose(window)) {
        my_input(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

