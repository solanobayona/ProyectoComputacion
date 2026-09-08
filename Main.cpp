#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>

// Tamaño de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Shaders básicos
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 ourColor;\n"
"uniform mat4 uMVP;\n"
"void main() {\n"
"    gl_Position = uMVP * vec4(aPos, 1.0);\n"
"    ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 ourColor;\n"
"void main() {\n"
"    FragColor = ourColor;\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Imprimir nombre del proyecto en consola
    std::cout << " Proyecto: Triangulos_ NICOLAS SOLANO_6000809" << std::endl;

    // Inicialización de GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangulos_ NICOLAS SOLANO_6000809", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error al inicializar GLAD" << std::endl;
        return -1;
    }

    // Activar Depth Test y Blending para transparencia Alpha
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // VÉRTICES DE LOS 3 TRIÁNGULOS Alineados de la base en Y = -0.3, organizados uno detrás de otro en Z y con tamaños diferentes
    float verticesTriangulos[] = {
        // --- TRIÁNGULO 1: VERDE (Al frente, Z = 0.4, Pequeño) ---
        // Posición (X, Y, Z)      Color (R, G, B, Alpha)
        -0.2f, -0.3f,  0.4f,     0.0f, 1.0f, 0.0f, 0.6f,
         0.2f, -0.3f,  0.4f,     0.0f, 1.0f, 0.0f, 0.6f,
         0.0f,  0.1f,  0.4f,     0.0f, 1.0f, 0.0f, 0.6f,

         // --- TRIÁNGULO 2: ROJO (En el centro, Z = 0.0, Mediano) ---
         -0.4f, -0.3f,  0.0f,     1.0f, 0.0f, 0.0f, 0.6f,
          0.4f, -0.3f,  0.0f,     1.0f, 0.0f, 0.0f, 0.6f,
          0.0f,  0.3f,  0.0f,     1.0f, 0.0f, 0.0f, 0.6f,

          // --- TRIÁNGULO 3: AZUL (Atrás, Z = -0.4, Grande) ---
          -0.6f, -0.3f, -0.4f,     0.0f, 0.0f, 1.0f, 0.6f,
           0.6f, -0.3f, -0.4f,     0.0f, 0.0f, 1.0f, 0.6f,
           0.0f,  0.5f, -0.4f,     0.0f, 0.0f, 1.0f, 0.6f
    };

    // VÉRTICES DE LOS EJES GUIA en Z = -0.6 detrás del triángulo azul
    float verticesEjes[] = {
        // Eje X (Rojo)
        -0.5f, -0.3f, -0.6f,     0.8f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.3f, -0.6f,     0.8f, 0.0f, 0.0f, 1.0f,

         // Eje Y (Verde)
          0.0f, -0.3f, -0.6f,     0.0f, 0.8f, 0.0f, 1.0f,
          0.0f,  0.6f, -0.6f,     0.0f, 0.8f, 0.0f, 1.0f,

          // Eje Z (Azul)
           0.0f, -0.3f, -0.8f,     0.5f, 0.5f, 0.8f, 1.0f,
           0.0f, -0.3f, -0.4f,     0.5f, 0.5f, 0.8f, 1.0f
    };

    // VAO/VBO para los Triángulos
    unsigned int vaoTriangulos, vboTriangulos;
    glGenVertexArrays(1, &vaoTriangulos);
    glGenBuffers(1, &vboTriangulos);

    glBindVertexArray(vaoTriangulos);
    glBindBuffer(GL_ARRAY_BUFFER, vboTriangulos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangulos), verticesTriangulos, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO/VBO para los Ejes Guía
    unsigned int vaoEjes, vboEjes;
    glGenVertexArrays(1, &vaoEjes);
    glGenBuffers(1, &vboEjes);

    glBindVertexArray(vaoEjes);
    glBindBuffer(GL_ARRAY_BUFFER, vboEjes);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEjes), verticesEjes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int mvpLoc = glGetUniformLocation(shaderProgram, "uMVP");

    // RENDER LOOP
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Matriz de Proyección
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = width > 0 ? (float)width / (float)height : 1.0f;
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);

        // Matriz de Vista
        glm::mat4 view = glm::lookAt(
            glm::vec3(1.5f, 0.8f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        // Matriz de Modelo (Rotación automática en Y)
        float t = (float)glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, t * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 mvp = projection * view * model;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        // Dibujar Ejes Guía
        glLineWidth(2.0f);
        glBindVertexArray(vaoEjes);
        glDrawArrays(GL_LINES, 0, 6);

        // Dibujar Triángulos
        glBindVertexArray(vaoTriangulos);
        glDrawArrays(GL_TRIANGLES, 0, 9);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberar recursos
    glDeleteVertexArrays(1, &vaoTriangulos);
    glDeleteBuffers(1, &vboTriangulos);
    glDeleteVertexArrays(1, &vaoEjes);
    glDeleteBuffers(1, &vboEjes);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}