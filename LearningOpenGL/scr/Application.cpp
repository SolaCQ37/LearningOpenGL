#include <GL/glew.h>        // gl.h should been included after glew.h
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    /* Parameters
     * shader: Specifies the handle of the shader object whose source code is to be replaced
     * count: Specifies the number of elements in the string and length arrays
     * string: Specifies an array of pointers to strings containing the source code to be loaded into the shader
     * length: Specifies an array of string lengths. If length is NULL, each string is assumed to be null terminated
     */
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader." << std::endl;
        std::cout << message << std::endl;
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    /* We can delete shaders because they were linked to program. */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* We need to create a valid OpenGL rendering context before call glewInit */
    if (GLEW_OK != glewInit())     // we should define GLEW_STATIC if we use static library 
    {
        std::cout << "Error!" << std::endl;
    }

    /* Print GL_VERSION just for check */
    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Create VertexBuffer */
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };  // Vertex data

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);    // Look for usage in document(https://docs.gl/)

    glEnableVertexAttribArray(0);
    /* Parameters:
     * index: Specifies the index of the generic vertex attribute to be modified
     * size: Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
     * type: Specifies the data type of each component in the array.
     * normalized: specifies whether fixed-point data values should be normalized
     * stride: offset of vertex
     * pointer: offset of attribute of vertex, position is first, so pointer == 0.
     */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Parameters
         * mode:  Specifies what kind of primitives to render
         * first: Specifies the starting index in the enabled arrays
         * count: Specifies the number of indices to be rendered
         */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}