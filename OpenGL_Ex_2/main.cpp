///This program is a simple rectangle in screen using ModernOpenGL
///Author: Jorge Andres Gonzalez Borboa
///Following: https://learnopengl.com/#!Getting-started/Shaders

///You need GLFW||GLUT||freeGLUT and GLEW||GLAD to compile this example.

#define GLEW_STATIC

/// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <cassert>
#include <stdexcept>

#include <iostream>

using namespace std;

///Data for the square
///we are drawing
GLfloat vertices[] =
{
     //Position          //Color
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,      // top right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,      // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,      // bottom left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f       // top left
};

///Corresponding indices
///for the triangles
///this info is for the
///EBO
unsigned int indices[] =
{
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

///This is the callback function for when the window gets resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///Just adjust the viewport to the new window size
    glViewport(0, 0, width, height);
}

///This is the callback function for input data, keyboard, mouse etc
void processInput(GLFWwindow *window)
{
    ///If the 'esc' key was pressed
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        ///Close the window
        glfwSetWindowShouldClose(window, true);
    }

}

int main ()
{

///\///////////////////INITIALIZE ALL THE FRAMEWORKS////////////////////////////

    ///Initialize GLFW
    if(glfwInit() == GLFW_TRUE)
    {
        cout << "GLFW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLFW" << endl;
    }

    ///Set GLFW version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    ///Uncomment this function for MAC-OS
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    ///Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "ModernOpenGL", NULL, NULL);
    if (window == NULL)
    {
        ///If it fails, end the program
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    ///Set the current context, the window we are currently working in.
    glfwMakeContextCurrent(window);

    ///Register 'framebuffer_size_callback' as the resize window callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ///Set the initial viewport
    glViewport(0, 0, 800, 600);

    ///Initialize GLEW
    if(glewInit() == GLEW_OK)
    {
        cout << "GLEW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLEW" << endl;
    }

///\////////////////////////////////////////////////////////////////////////////

///\///////////////////SHADER SOURCE CODE///////////////////////////////////////

    ///This is the vertex shader code written in GLSL ver 3.3
    static const char *vertexShaderCode[] =
    {
        "#version 330 core                                  \n"
        "layout (location = 0) in vec3 aPos;                \n"
        "layout (location = 1) in vec3 aColor;              \n"
        "out vec3 myColor;                                  \n"
        "void main()                                        \n"
        "{                                                  \n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
        "  myColor = aColor;                                \n"
        "}                                                  \n"

    };

    ///This is the fragment shader code written in GLSL ver 3.3
    static const char *fragmentShaderCode [] =
    {
        "#version 330 core                                  \n"
        "in vec3 myColor;                                   \n"
        "out vec4 FragColor;                                \n"
        "void main()                                        \n"
        "{                                                  \n"
            "FragColor = vec4(myColor,1.0);                 \n"
        "}                                                  \n"
    };

///\////////////////////////////////////////////////////////////////////////////

///\/////////////////////////COMPILE SHADERS///////////////////////////////////

    ///Create a Vertex Shader Object, *An object is just an index.
    unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);

    ///Link our Vertex Shader Code with our Vertex Shader Object
    glShaderSource(iVertexShader, 1,vertexShaderCode, NULL);

    ///Compile the VertexShader
    glCompileShader(iVertexShader);

    ///Check if the VertexShader compiled correctly
    int  success;
    char infoLog[512];
    glGetShaderiv(iVertexShader, GL_COMPILE_STATUS, &success);

    ///Self explanatory
    if(success)
    {
        cout << "Vertex Shader compiled successfully" << endl;
    }
    else
    {
        glGetShaderInfoLog(iVertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
        << endl;
    }

    ///\///SAME PROCESS FOR FRAGMENT SHADER/////

    unsigned int iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(iFragmentShader, 1, fragmentShaderCode, NULL);
    glCompileShader(iFragmentShader);

    glGetShaderiv(iFragmentShader, GL_COMPILE_STATUS, &success);

    if(success)
    {
        cout << "Fragment Shader compiled successfully" << endl;
    }
    else
    {
        glGetShaderInfoLog(iVertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
        << endl;
    }

///\////////////////////////////////////////////////////////////////////////////


///\///////////////LINK THE SHADERS INTO A SHADER PROGRAM///////////////////////

    ///Create a shader program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    ///Attach your shaders to the program and link them
    glAttachShader(shaderProgram, iVertexShader);
    glAttachShader(shaderProgram, iFragmentShader);
    glLinkProgram(shaderProgram);

    ///Check for linking errors.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Shader Linking Error" << endl;
        return -1;
    }
    else
    {
        cout << "Shaders linked sucessfully" << endl;

        ///If the program linked correctly you can delete your shader
        ///objects, we don't need them anymore
        glDeleteShader(iVertexShader);
        glDeleteShader(iFragmentShader);
    }

    ///Set the program we are gonna currently use
    glUseProgram(shaderProgram);

///\////////////////////////////////////////////////////////////////////////////


///\////////////////CREATION OF VAOs VBOs and EBOs//////////////////////////////

//VAO-Vertex Array Object
//VBO-Vertex Buffer Object
//EBO-Element Buffer Object

    ///Generate VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    ///Generate VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    ///Generate EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    ///First Bind the VAO, so that all the configuration is saved in this VAO
    glBindVertexArray(VAO);

    ///Bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    ///Populate VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    ///Populate EBO with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    ///Set the info of how the VBO must be read
    /// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    /// color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);



///\////////////////////////////////////////////////////////////////////////////


    ///Draw in Wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ///Draw in Fillmode, this is default.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ///This is the render loop *While the window is open*
    while(!glfwWindowShouldClose(window))
    {
        ///Set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ///Process user input, in this case if the user presses the 'esc' key
        processInput(window);

        ///THIS IS THE ACTUAL DRAWING FUNCTION
        ///Set the shader program
        glUseProgram(shaderProgram);

        ///Draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ///Swap the Front and Back buffer.
        glfwSwapBuffers(window);

        ///Poll CallBack Events
        glfwPollEvents();
    }

    ///Free resources when application ends.
    glfwTerminate();
    return 0;
}
