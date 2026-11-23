steps

1. Instnatiate GLFW 
2. create the window object
3. initialize GLAD 
4. tell openGL the size of the rendering window using `glViewport` 
- Also.want to register a callback function on the window that gets called each time the window is resized
```
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  
``` 

Basically this function will tell the GLFWindow that the width and height have changed each time the window gets resized.
register this with `glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  `. 


Use the render loop:

```
while(!glfwWindowShouldClose(window))
{
    glfwSwapBuffers(window); //  swaps the color buffer, shows it as output to the screen. 
    glfwPollEvents();    // Checks if any events are triggered, adn then calls the relevant callback methods 
}
```

cleanup
```
glfwTerminate();
return 0;
```

How does input work?

Best to define a `processInput` function that houses the input logic, and then call that `processInput` function in the render loop. 