 
#include <iostream>
#include "helpers.h"
#include "shader.h"


float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
    0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    // top 
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f // top left 
};    



int main(){
    // Initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Create the window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Tells openGL "This window's context is the one all the gl 
    // commands should apply to from now on."
    glfwMakeContextCurrent(window);
    // The moment a user resizes the window the viewport should be adjusted as well. 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    // Initialize GLAD
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell OpenGL the size of the rendering window before we start rendering. 
    glViewport(0, 0, 800, 600);
    
    

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Load, compile and link the shaders from disk (paths are relative to the working directory)
    Shader ourShader("shader.vs", "shader.fs");
    ourShader.use();
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // create the model matrix 
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 

    // the view matrix
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

    // the projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    

    // keep drawing images and handling user input until the program has been explicitly told to stop. 
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"),      1,
        GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"),       1,
        GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1,
        GL_FALSE, glm::value_ptr(projection));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();    

    }

    // Clean all resources allocated
    glfwTerminate();
    return 0;
}