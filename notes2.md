# Graphics pipeline of OpenGL 
The graphics pipeline of opengl is divided into two main parts:
- transforming 3d coordinates into 2d coordinates
- transform the 2d coordinates into colored pixels 

but overall, it has multiple parts/stages. Each stage depends on the output of the previous stage's output. 
- but each stage is parallelizable within itself, which si why graphics cards have many processing cores. 
- Shader: a small program run on the GPU for each step of the pipeline. 

What is the original input to the graphics pipeline?
- A list of three 3d coordinates that should form a triangle, in an array called Vertex Data -- a collection of vertices. Each vertex contains vertex attributes, i.e. color.


## Step 1: Vertex shader
Purpose: transforms 3d coordinates into different 3d coordinates
- Passes the output into geometry shader

## Step 2: Geometry shader (Optional)
Input: a collection of vertices that form a primitive (what is a primitive?) and generates other shapes by emitting new vertices to form other primitives. 
A primitive is the basic geometric shape OpenGL knows how to draw directly. It's not a general geometry term, it's specifically: points, lines, or triangles (and their variants).

## Step 3: Primitive assembly
Input: all the vertices from the vertex or geometry shader
Assembles all the points in the primitive shape given. i.e. a triangle

## Step 4: Rasterization
maps the resulting primitives from the previous stage to the corresponding pixels on the final screen. 
Performs clipping, discarding fragments that are outside the view. 

## Step 5: Fragment shader 
calculates the final color of a pixel. the stage where stuff like lighting happens 

## Step 6: alpha test and blending 
depth and stencil value - Asks: Is this fragment in front or behind something else? Should it be discarded? 
- also keeps track of opacity and such. 

*For many cases, you only have to work with the vertex and fragment shader.* 
- but have to define a vertex and fragment shader of your own. 


# Next
OpenGL is a 3d graphics engine, so all coordinates have to be in 3d (x,y,z). 
- OpenGL only processes normalized device coordinates (x,y,z have to be in between -1.0 and 1.0)

define vertices in a simple float array like this, 
```
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};  
```
once the vertex data is defined, first have to allocate memory on the GPU where you store the vertex data, then configure how OpenGL should interpret the memory, and then specify how to send the data to the graphics card. 
- Manage this memory using a vertex buffer object (VBO) that can store a large number of vertices in the GPU's memory. 
- advantage of using this: send large batches of data all at once to the graphics card and keep it there if there's enough memory left (without sending data one vertex at a time)
- cpu to graphics card = slow operation 

```
unsigned int VBO;
glGenBuffers(1, &VBO);  
```
^ Buffer has a unique id corresponding to that buffer

The buffer type of a vertex buffer object is `GL_ARRAY_BUFFER`. 

after designating the ID, call `glBindBuffer(GL_ARRAY_BUFFER, VBO);  `. 
This binds the buffer. 
This makes it so whenever we make calls on `GL_ARRAY_BUFFER`, it refers to that specific currently bound buffer. 

Actually put the vertices into the buffer:
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

The fourth parameter specifies how we want the graphics card to manage the given data. This can take 3 forms:

GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
GL_STATIC_DRAW: the data is set only once and used many times.
GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

At this point, the vertex data has now been stored on the graphics card's memory. 

## Vertex shader 
modern openGL requires at least a vertex and fragment shader.

First thing to do is to write the vertex shader in the shader language GLSL. Then store it in a const C string in a code file:

```
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
```

Declare all the input vertex attributes in the vertex shader with the in keyword. Since each vertex has a 3d coordinate, create a vec3 input variable with the name aPos. The location is 0 via `layout (location = 0)`. 

Set the output of the vertex shader by assigning the position data to the predefined gl_Position variable. 

In the main file, compile the shader using:

```
unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
```

## Fragment shader
Fragment shader calculates the color of your pixels. 

```
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```
Declare the output value with `out`
Alpha, R, G, B

```

unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);
```

## Shader program
Shader program object is the final linked version of multiple shaders combined. 

```
unsigned int shaderProgram;
shaderProgram = glCreateProgram(); // creates a program and returns an ID reference to the newly created program object. 
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
```

Activate using:
```
glUseProgram(shaderProgram);
```

Can delete the shader objects
```
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);  
```

By this point you have sent the input vertex data to the GPU (glbufferdata) and instructed the gpu how it should process the data within a vertex and a fragment shader (gluseprogram(shaderprogram)). 

OpenGL does not yet know how it should interpret the vertex data in memory and how it should connect the vertex data to the vertex shader's attributes. 

## Linking Vertex Attributes
We have to manually specify what part of our input data goes to which vertex attribute in the vertex shader - that is because vertex buffer is just one big buffer.

```
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);  
```

The first parameter specifies which vertex attribute we want to configure. Remember that we specified the location of the position vertex attribute in the vertex shader with layout (location = 0). This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
The next argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
The next argument specifies if we want the data to be normalized. If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not relevant for us so we'll leave this at GL_FALSE.
The fifth argument is known as the stride and tells us the space between consecutive vertex attributes. Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride. Note that since we know that the array is tightly packed (there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
The last parameter is of type void* and thus requires that weird cast. This is the offset of where the position data begins in the buffer. Since the position data is at the start of the data array this value is just 0. We will explore this parameter in more detail later on

A vertex array object fixes a problem: when you draw different objects, you have to configure the vertex attribute pointer every single time. If you make a vertex array object, you can store a configured setting and reuse it each time. 

```
// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
// 1. bind Vertex Array Object
glBindVertexArray(VAO);
// 2. copy our vertices array in a buffer for OpenGL to use
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. then set our vertex attributes pointers
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);  

  
[...]

// ..:: Drawing code (in render loop) :: ..
// 4. draw the object
glUseProgram(shaderProgram);
glBindVertexArray(VAO); // this stores the attribute pointer in the VAO
someOpenGLFunctionThatDrawsOurTriangle();   
```

## putting it together
```
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```
The glDrawArrays function takes as its first argument the OpenGL primitive type we would like to draw.

## Element Buffer Object
A way to optimize your code by sharing vertices between primitives 

```
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

unsigned int EBO;
glGenBuffers(1, &EBO);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```