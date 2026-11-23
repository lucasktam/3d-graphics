# Extensions

Whenever a graphics company comes up with a new technique or something for rendering, it will release an extension in the drivers.
If the hardware supports such an extension, the developer may use the functionality. 
Use a simple conditional to check if the hardware supports this extension. 

```
if(GL_ARB_extension_name)
{
    // Do cool new and modern stuff supported by hardware
}
else
{
    // Extension not supported: do it the old way
}
```

# State Machine
OpenGL is a state machine. A collection of variables define how OpenGL should currently operate. The state of opengl is referred to as the OpenGL *context*. 
- Change state by setting some options, manipulating some buffers, and then render *using the current context*. 
- example: Want to draw lines instead of triangles? Change the context variable that controls this. 
- OpenGL uses state-changing functions and state-using functions. 

# Objects in OpenGL
An object in OpenGL is a collection of objects that represent a subset of OpenGL's state. For example, there may be an object that represents the settings of the drawing window. 

```
struct object_name {
    float  option1;
    int    option2;
    char[] name;
};
```

When we need to use objects, we can use something like this:

```
// The State of OpenGL
struct OpenGL_Context {
  	...
  	object_name* object_Window_Target;
  	...  	
};

// create object
unsigned int objectId = 0;
glGenObject(1, &objectId);
// bind/assign object to context
glBindObject(GL_WINDOW_TARGET, objectId);
// set options of object currently bound to GL_WINDOW_TARGET
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH,  800);
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
// set context target back to default
glBindObject(GL_WINDOW_TARGET, 0);hello
```

^ basically the workflow is that you 
1. create an object and store a reference to it as an id.
2. Bind the object to the target location of the context. 
3. Set the window options (do whatever you want to that object)
4. Unbind the object. 
