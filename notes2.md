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