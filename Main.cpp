#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// -------------------------------------------------------------
// SHADERS
// -------------------------------------------------------------

// Vertex Shader: Aplica transformación matricial y define tamaño de puntos/cuadrados
const char* miVertexShader = "#version 330 core\n"
"layout (location = 0) in vec3 posicion;\n"
"uniform mat4 matTransformacion;\n"
"uniform float tamanoPunto;\n"
"void main()\n"
"{\n"
"   gl_Position = matTransformacion * vec4(posicion, 1.0);\n"
"   gl_PointSize = tamanoPunto;\n"
"}\0";

// Fragment Shader: Asigna el color enviado vía uniform
const char* miFragmentShader = "#version 330 core\n"
"out vec4 colorSalida;\n"
"uniform vec4 colorUnif;\n"
"void main()\n"
"{\n"
"   colorSalida = colorUnif;\n"
"}\0";

// -------------------------------------------------------------
// FUNCIONES AUXILIARES
// -------------------------------------------------------------

// Función para compilar y enlazar el programa de shaders en un solo paso
GLuint crearProgramaShaders(const char* vSource, const char* fSource)
{
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vSource, NULL);
    glCompileShader(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fSource, NULL);
    glCompileShader(fShader);

    GLuint programa = glCreateProgram();
    glAttachShader(programa, vShader);
    glAttachShader(programa, fShader);
    glLinkProgram(programa);

    // Limpieza de shaders individuales ya enlazados
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return programa;
}

// -------------------------------------------------------------
// MAIN
// -------------------------------------------------------------

int main()
{
    // Configuración inicial de GLFW
    if (!glfwInit()) {
        std::cout << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* ventana = glfwCreateWindow(800, 800, "OpenGL NICOLAS SOLANO", NULL, NULL);
    if (ventana == NULL) {
        std::cout << "Fallo al crear la ventana de GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(ventana);

    // Cargar direcciones de funciones de OpenGL mediante GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error al inicializar GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 800);

    // Permite controlar el tamaño de los puntos desde el Vertex Shader (gl_PointSize)
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Compilación del Shader Program usando nuestra función auxiliar
    GLuint shaderID = crearProgramaShaders(miVertexShader, miFragmentShader);

    // Geometría: Triángulos concéntricos
    GLfloat posVertices[] = {
        -0.5f,     -0.5f * float(sqrt(3)) / 3,     0.0f, // V0
         0.5f,     -0.5f * float(sqrt(3)) / 3,     0.0f, // V1
         0.0f,      0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // V2
        -0.5f / 2,  0.5f * float(sqrt(3)) / 6,     0.0f, // V3
         0.5f / 2,  0.5f * float(sqrt(3)) / 6,     0.0f, // V4
         0.0f,     -0.5f * float(sqrt(3)) / 3,     0.0f  // V5
    };

    GLuint indicesMalla[] = {
        0, 3, 5, // Triángulo 1
        3, 2, 4, // Triángulo 2
        5, 4, 1  // Triángulo 3
    };

    // Configuración de Buffers (VAO, VBO, EBO)
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posVertices), posVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesMalla), indicesMalla, GL_STATIC_DRAW);

    // Atributo 0: Posición de vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Desvincular buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Referencias a las variables Uniforms del shader
    GLint locMatrizTransform = glGetUniformLocation(shaderID, "matTransformacion");
    GLint locColor = glGetUniformLocation(shaderID, "colorUnif");
    GLint locTamanoPunto = glGetUniformLocation(shaderID, "tamanoPunto");

    // Matriz Identidad 4x4
    GLfloat matrizIdentidad[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(ventana))
    {
        // Fondo verde oliva
        glClearColor(0.22f, 0.32f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderID);

        // Envío de la matriz de transformación
        glUniformMatrix4fv(locMatrizTransform, 1, GL_FALSE, matrizIdentidad);

        glBindVertexArray(vao);

        // Pasada 1: Renderizado del cuerpo de los triángulos (Tono Naranja)
        glUniform4f(locColor, 0.8f, 0.3f, 0.02f, 1.0f);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Pasada 2: Renderizado de cuadrados sobre cada vértice (Azul Celeste)
        glUniform4f(locColor, 0.0f, 0.75f, 1.0f, 1.0f);
        glUniform1f(locTamanoPunto, 16.0f); // Ajuste del ancho del cuadrado en px
        glDrawElements(GL_POINTS, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }

    // Liberación de recursos
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderID);

    glfwDestroyWindow(ventana);
    glfwTerminate();
    return 0;
}