# Shaders
Shaders are little programs stored in the GPU. These programs are run for each stage of the graphics pipeline
- they transform inputs to outputs 

## GLSL
Shaders are written in the c-like language GLSL. 
Shaders always begin with a version declaration, followed by a list of input and output variables, uniforms, and the `main` function. 
The `main` function processes input variables and outputs the results in the output variables. 

```
#version version_number
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;
  
uniform type uniform_name;
  
void main()
{
  // process input(s) and do some weird graphics stuff
  ...
  // output processed stuff to output variable
  out_variable_name = weird_stuff_we_processed;
}
```

Each input variable is also known as a vertex attribute. There is a maximum number of vertex attribute allowed to declare which is limited by the hardware. 

## Types 
GLSL has the normal types like int, float, double, uint, and bool. 
Also has two container types called `vector` and `matrix`

### Vectors
- vecn: the default vector of n floats.
- bvecn: a vector of n booleans.
- ivecn: a vector of n integers.
- uvecn: a vector of n unsigned integers.
- dvecn: a vector of n double components.

just use the default vecn usually because it's sufficient

Swizzling:
```
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```

Other weird syntax
```
vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);
```

## Ins and outs
the `in` and `out` keywords designate the inputs and outputs of each shader
Wherever an output variable matches with an input variable of the next shader stage, they're passed along. (Automatically?)

Vertex shader should receive some form of input otherwise it would be pretty ineffective. It receives input straight from the vertex data. 
Specify the input variables with location metadata so you can configure the vertex attributes on the CPU. For example: `layout (location = 0)`

what is location metadata? assigns a shader variable a fixed integer slot number. When you call glVertexAttribPointer, the first argument refs to that integer slot number which connects to the shader variable. 

Fragment shader requires a `vec4` color output variable. 


## uniforms
another way to pass data from the application on the CPU to the shaders on the GPU. 

Uniforms are global though. A uniform variable is unique per shader program object and can be accessed from **any shader at any stage in the shader program**. 

easy to set up a uniform 

```
#version 330 core
out vec4 FragColor;
  
uniform vec4 ourColor; // we set this variable in the OpenGL code.

void main()
{
    FragColor = ourColor;
} 
```

In the main, you can configure the uniform 

```
float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
glUseProgram(shaderProgram);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```

