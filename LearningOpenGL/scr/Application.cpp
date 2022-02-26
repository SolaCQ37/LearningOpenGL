#include <GL/glew.h>        // gl.h should been included after glew.h
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

void APIENTRY debugMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // Ignore some not important error or warning
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Specified OpenGL version and use core profile */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create debug context before create OpenGL context */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

    /* If create debug context successfully, we can bind debug message callback */
    GLint flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
        glDebugMessageCallback(debugMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    /* Create VertexBuffer and add texture coordinates */
    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,  // 0, left  down
             0.5f, -0.5f, 1.0f, 0.0f,  // 1, right down
             0.5f,  0.5f, 1.0f, 1.0f,  // 2, right top
            -0.5f,  0.5f, 0.0f, 1.0f   // 3, left  top
        };  // Vertex data

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };  // Index data

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Create Vertex Array */
        VertexArray va;
        /* Create Vertex Buffer */
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        /* Vreate Vertex Buffer Layout */
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //glEnableVertexAttribArray(0);
        /* Parameters:
         * index: Specifies the index of the generic vertex attribute to be modified
         * size: Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
         * type: Specifies the data type of each component in the array.
         * normalized: specifies whether fixed-point data values should be normalized
         * stride: offset of vertex
         * pointer: offset of attribute of vertex, position is first, so pointer == 0.
         */
        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

        IndexBuffer ib(indices, 6);

        /* Now we get shader code from file */
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.7f, 1.0f);

        Texture texture("res/textures/AndroscogginRiver.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);    // We bind our texture to slot 0

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            /* Parameters
             * mode:  Specifies what kind of primitives to render
             * first: Specifies the starting index in the enabled arrays
             * count: Specifies the number of indices to be rendered
             */
             //glDrawArrays(GL_TRIANGLES, 0, 3);

            shader.Bind();
            //shader.SetUniform4f("u_Color", r, 0.3f, 0.7f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (r >= 1.0f)
            {
                increment = -0.05f;
            }
            else if (r <= 0.0f)
            {
                increment = 0.05f;
            }
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    /* Deconstruction vb and ib before glfwTerminate otherwise it will cause error */

    glfwTerminate();
    return 0;
}