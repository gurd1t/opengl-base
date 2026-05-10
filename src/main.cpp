#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "include/math_3d.hpp"
#include <cmath>
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int VAO, VBO;
unsigned int gWorldLocation;

const char *pVSFileName = "vs.glsl";
const char *pFSFileName = "fs.glsl";

// ============================================
// Vertex Shader
// ============================================

const char *vertexShaderSource = "#version 460 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "\n"
                                 "uniform mat4 gWorld;\n"
                                 "\n"
                                 "void main()\n"
                                 "{\n"
                                 "    gl_Position = gWorld * vec4(aPos, 1.0);\n"
                                 "}\0";

// ============================================
// Fragment Shader
// ============================================

const char *fragmentShaderSource = "#version 460 core\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
                                   "}\0";

// ============================================
// Matrix Struct
// ============================================

// struct Matrix4f {
//   float m[4][4];
// };

// ============================================
// Framebuffer Resize Callback
// ============================================

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// ============================================
// Input
// ============================================

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// ============================================
// Shader Compilation
// ============================================

unsigned int createShader(unsigned int type, const char *source) {
  unsigned int shader = glCreateShader(type);

  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cout << "Shader compilation failed:\n" << infoLog << std::endl;
  }

  return shader;
}

// ============================================
// Shader Program
// ============================================

unsigned int createProgram(unsigned int vertexShader,
                           unsigned int fragmentShader) {
  unsigned int program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  int success;
  char infoLog[512];

  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cout << "Program linking failed:\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

// ============================================
// Geometry Setup
// ============================================

void createTriangle() {
  float vertices[] = {// positions
                      0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, // layout location
                        3, // vec3
                        GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// ============================================
// Main
// ============================================

int main() {
  // GLFW Init
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "glfw", nullptr, nullptr);

  if (!window) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD Init
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }

  // Viewport
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  // Build Shaders
  unsigned int vertexShader =
      createShader(GL_VERTEX_SHADER, vertexShaderSource);

  unsigned int fragmentShader =
      createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  unsigned int shaderProgram = createProgram(vertexShader, fragmentShader);

  glUseProgram(shaderProgram);

  // Uniform Location
  gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");

  // Create Triangle
  createTriangle();

  // Background Color
  glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

  // ============================================
  // Render Loop
  // ============================================

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    static float angle = 0.0f;
    angle += 0.01f;

    Matrix4f world = {};

    // Rotation around Z axis

    world.m[0][0] = cosf(angle);
    world.m[0][1] = -sinf(angle);
    world.m[0][2] = 0.0f;
    world.m[0][3] = 0.0f;

    world.m[1][0] = sinf(angle);
    world.m[1][1] = cosf(angle);
    world.m[1][2] = 0.0f;
    world.m[1][3] = 0.0f;

    world.m[2][0] = 0.0f;
    world.m[2][1] = 0.0f;
    world.m[2][2] = 1.0f;
    world.m[2][3] = 0.0f;

    world.m[3][0] = 0.0f;
    world.m[3][1] = 0.0f;
    world.m[3][2] = 0.0f;
    world.m[3][3] = 1.0f;

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &world.m[0][0]);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();

  return 0;
}
