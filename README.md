# Lab 9: VBOs & VAOs

## 1. Intro

In the last lab, you learned how to construct vertex data for various shapes and put them into a container as continuous floats. What you didn’t know is that this representation is actually how OpenGL stores its vertex data as well! Currently, our data is in C++ structures on the CPU, but in this lab you will create OpenGL specific objects to both store and describe the data that can be used for drawing on the GPU. These objects are Vertex Buffer Objects (VBO) and Vertex Array Objects (VAO). At the moment, you can imagine a VBO to be a dumb container, the same as a C++ vector that doesn’t have a clue about the meaning behind the data that is in it. The VAO on the other hand acts as a translation manual for the GPU to understand what data and what format of data is inside a Vertex Buffer Object.
By the end of this lab, you should be able to 
1. Understand the difference between a Vertex Buffer Object and a Vertex Array Object and their individual uses,
2. Learn about per vertex attributes and layouts,
3. Draw triangle meshes (which you have been creating in the last lab!)

### 1.1 Using the Khronos Documentation

Most APIs come with some form of documentation on how you can use them. OpenGL is no exception, as a matter of fact, OpenGL is made by the Khronos group (who now is also developing the Vulkan API) and has extensive documentation on any functions across any version of OpenGL you may use. The home page is located [here](https://www.khronos.org/registry/OpenGL-Refpages/). The version we will be using is 3.1 with GLSL version 3.10.

Let’s take a look at a function to see what the docs tell us about it!

|**_Task 1:_**|
|:---|
|Find the function `glGetError` and open its documentation|
|Notice the different sections.<ul><li>The first section, **Name**, says the function name along with a short description of what the function is. This is useful if you want to quickly browse for a reminder, but often if you are looking at new functions, you will have to dig deeper.</li><li>The second section, **C Specification** says the particular syntax of the function in question.</li><li>In cases where a function has input parameters, there will be an intermediate section titled **Parameters** which gives short descriptions of each input parameter and occasionally what values they can take on.</li><li>The third section, **Description**, gives you much more information about how the function is used and sometimes information about the parameters in question such as OpenGL error enums as we see here!</li><li>The fourth section, **API Version Support**, tells us in which OpenGL versions this function can be called. Be aware of this if you are trying to use modern OpenGL functions but any we cover here will be supported by the stencil’s OpenGL version.</li></ul>Through this lab and your work in OpenGL, we strongly recommend having the docs open in another tab to reference, and each function signature will serve as a link to their relevant page in the docs.|

### 1.2 OpenGL as a State Machine

Instead of defining a bunch of instances of objects and editing them directly, OpenGL functions as a state machine. This means that you keep track of an OpenGL context with different settings and conditions which affects every operation you do. In this way, you will have to change your programming mindset when looking at OpenGL functions. In a moment, you will encounter the glBindBuffer operation which will be used to set a specific VBO to be active in the current OpenGL context or state. Then in order to edit it, we make calls on the state rather than the object as you will see.

Because of this unique behavior, it is important to be careful about managing OpenGL’s state during the execution of your programs. There are two strategies for this:
Assume nothing When writing a function that uses OpenGL, consider that the state could be anything. Explicitly set the values of the state parameters you depend on.
Default configuration. Decide on a set of reasonable default state values, and set the state to these values at the start of your application. Anywhere you change one of these state parameters, restore the parameter to its previous value after you’re done rendering a component of the scene.
 
|**_Task 2:_**|
|:---|
|Search the docs for the `glSetClearColor` function and call at the start of initializeGL to alter our state and change the background color to anything you like!|

## 2. Vertex Buffer Objects

Typically, shapes in OpenGL are broken down into a collection of triangles, each of which is defined by three vertices. So far, we have been processing all of our scene data on the CPU, but in order to draw using OpenGL, we need to send a list of vertices to the GPU.

To send this data, we need some sort of container or buffer to store it first. In OpenGL, this generic container is called a vertex buffer object, or VBO for short. The key to VBOs is to remember that they are just mindless containers that store numbers and nothing more. They have no idea of what they contain or what it will be used for. In this lab we will learn about how to create one of these VBOs and use it to later draw shapes of our own!
 
### 2.1 Creating a Vertex Buffer Object

Let’s take a look at how you would actually create a VBO. The OpenGL function is as follows:

`void glGenBuffers(GLsizei n, GLuint *buffers)`

<details>
<summary>void</summary>
<br>
Indicates the function does not return
</details>
<details>
<summary>GLsizei n</summary>
<br>
Indicates the number of vertex buffer objects to create. The type GLsizei is an OpenGL alias indicating it is an integer representing some size
</details>
<details>
<summary>GLuint *buffers</summary>
<br>
This is the pointer this function will fill in with an id for the generated buffer. This way you can refer to it later by the same stored id. Multiple buffers can be filled in using a pointer to the first element of an array. GLuint is an OpenGL alias for an unsigned int.
</details>

|**_Task 3:_**|
|:---|
|Generate a single VBO and store its id in `m_vbo`|

### 2.2 Adding Data

#### 2.2.1 Binding a VBO

Congratulations, you have made a VBO! But…it currently holds no data! Let’s fix that. As said before, OpenGL is a state machine, so in order to interact with our created buffer, we need to bind it to the current state. The OpenGL function to do this is:

`void glBindBuffer(GLenum target, GLuint buffer)`

<details>
<summary>void</summary>
<br>
Indicates the function does not return
</details>
<details>
<summary>GLenum target</summary>
<br>
This is an enumerable indicating which type of buffer you are binding. For our purposes, the target we need is GL_ARRAY_BUFFER which indicates it stores vertex attributes.
</details>
<details>
<summary>GLuint buffer</summary>
<br>
This indicates the specific buffer id that should be bound (hint: remember the GLuint we created in the last step?)
</details>

|**_Task 4:_**|
|:---|
|Bind your previously created VBO|

Now our state is all set up to shove our data into our buffer container! But before we do this, we actually need some data to put inside. Let’s construct an array that represents a triangle.

#### 2.2.2 Defining a Triangle

A triangle as said before is defined by 3 vertices. For now, let’s assume that these vertices are just positions, although in in the last lab you worked with vertex normals and in part 2 you will learn about more attributes vertices can hold!

As a reminder in OpenGL, the screen is defined as in the image below:

[image]

Take note of the x and y ranges of -1 to 1. As a reminder: in OpenGL, triangles are defined in a counter clockwise direction. 

|**_Task 5:_**|
|:---|
|Pick 3 points that would make a nice triangle|

#### 2.2.3 Putting Data Into a VBO

Now how will we actually put this information into our VBO? Let's take a look at the function to do so first:

`void glBufferData(GLenum target, GLsizeiptr size, const void * data, GLenum usage)`

<details>
<summary>void</summary>
<br>
Indicates the function does not return
</details>
<details>
<summary>GLenum target</summary>
<br>
This is the same argument seen in glBindBuffer. Refer to this binding process for a hint as to what this should be!
</details>
<details>
<summary>GLsizeiptr size</summary>
<br>
This indicates the size in bytes of the input data
</details>
<details>
<summary>const void * data</summary>
<br>
This is a pointer to the first element of the input data array
</details>
<details>
<summary>GLenum usage</summary>
<br>
This is an OpenGL enum representing how the data will be used. For our purposes, the usage parameter should be GL_STATIC_DRAW which indicates we don’t expect to change the data, so OpenGL can make optimizations based on that.
</details>

Next let’s look at data. The function expects a pointer to an input data array, but we don’t have that array yet! Let’s use an std::vector to construct our data container. In OpenGL, rather than separating out values for each individual vertex, it expects all of them mashed together into one contiguous collection as you did in the previous lab, and the VAO you will learn about soon keeps track of where the attributes are for us!

|**_Task 6:_**|
|:---|
|Construct an std::vector containing your three triangle points in a contiguous segment|

Now to pass this into the function, we can use the `data()` method of the std::vector object. This will return a pointer to the first element as needed.

For the size, while we can manually input the size of our data, it is much easier to let the program calculate that for us. The size of our data is going to be equal to:
(length of data) * (size of type of data stored)
To get the first part is simple using std::vector methods. For the second part, we can take advantage of the `sizeof()` function and pass in the data type stored in our vector.

|**_Task 7:_**|
|:---|
|Pass a triangle’s worth of data into your VBO|

## 3. Vertex Array Objects

Next, we will tell OpenGL the structure of our data using something called a Vertex Array Object or VAO for short. 

### 3.1 Vertex Attributes

At the moment, the only data we have cared about is the position of each vertex. But as a matter of fact, each vertex can have many more attributes than just its position. Consider colors, texture coordinates, or, as you saw in the previous lab, normals. These can also be unique to each vertex and it is our job to tell OpenGL what attributes we care about. 

When adding multiple attributes, they should appear in the same order and be repeated per vertex in our data. Consider the following image which shows a data layout containing positions, colors, and texture coordinates:

[image here]

|**_Task 8:_**|
|:---|
|Add colors of your choosing as RGB values to each vertex and integrate them into your previous triangle data vector|

### 3.2 Creating a VAO

Now let’s tell OpenGL that we have this extra attribute by making a VAO! The function to do this is:

`void glGenVertexArrays(GLsizei n, GLuint *arrays)`

<details>
<summary>void</summary>
<br>
Indicates the function does not return
</details>
<details>
<summary>GLsizei n</summary>
<br>
Indicates the number of vertex array objects to create
</details>
<details>
<summary>GLuint *arrays</summary>
<br>
This is the pointer this function will fill in with an id for the generated array
</details>

|**_Task 9:_**|
|:---|
|Generate a single VAO and store its id in `m_vao`|

### 3.3 Binding a VAO

Next to actually use this VAO, we need to bind it to the state. The function to do this is:

`void glBindVertexArray(GLuint array)`

<details>
<summary>void</summary>
<br>
Indicates the function does not return
</details>
<details>
<summary>GLuint array</summary>
<br>
This is the id of the VAO we wish to bind
</details>

|**_Task 10:_**|
|:---|
|Bind your VAO|

### 3.4 Utilizing a VAO

Now that the VAO is bound, we can actually use it to let OpenGL know the structure of our VBO. For each attribute, we need to do two things: 
1. Let OpenGL know we are identifying an attribute
2. Specify the location of the attribute in the VBO data.

For the first part, we call on the following function:

`void glEnableVertexAttribArray(GLuint index)`

<details>
<summary>void</summary>
<br>
Indicates the function does not return
</details>
<details>
<summary>GLuint index</summary>
<br>
Indicates the index of the attribute we are identifying. Indexing should start from 0 for the first attribute, so would be 1 for the second, 2 for the third and so forth
</details>

For the second part, we use a more complex function:

`void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)`

<details>
<summary>void:</summary>
<br>
Indicates the function does not return
</details>
<details>
<summary>GLuint index:</summary> 
This is the same index as used in glEnableVertexAttribArray above
</details>
<details>
<summary>GLint size:</summary>
This indicates the length of one vertex’s instance of this attribute, not the size in bytes. For a position of 3 floats, this would simply be 3. For an attribute that has 5 ints, this would be 5.
</details>
<details>
<summary>GLenum type:</summary>
This indicates the type of variable used for each component in the attribute. For floats, this would be GL_FLOAT
</details>
<details>
<summary>GLboolean normalized:</summary>
This is a bit complicated to do with types, but for now use GL_FALSE
</details>
<details>
<summary>GLsizei stride:</summary> 
This indicates the size in bytes between two vertices. That is, the distance between the x coordinate of the position of the first vertex and the x coordinate of the second vertex in bytes would be this value
</details>
<details>
<summary>const void* pointer:</summary>
This is a pointer to the first vertex’s instance of the attribute in the array of data. However, this more so refers to the offset into the array in which you will find this attribute. That is in the figure 1 (figure showing position,color,uv), the offset of the color attribute is 3*sizeof(GLfloat). But to cast this into a void*, we need to use reinterperet_cast<GLvoid*>(offset value)
</details>

>Note: These two functions will assume they apply to whichever VBO is currently bound, so be careful of your state when using them.

|**_Task 11:_**|
|:---|
|Add the two attributes of position and color to your VAO.|

<details>
<summary>Does your std::vector look familiar?</summary>
It should! This is the same format of data we worked with in the last lab! Without knowing it, you were actually creating VBO/VAO ready data with position and normal attributes! Keep that in mind when working on the project!
</details>

## 3: Unbinding Objects

In section 1.2, we discussed common approaches to handling the OpenGL state machine. Here, we will implement these ideas by keeping a common default state set. When we started, VBO and VAO ids 0 were bound, which means no VBO or VAO was bound. Let’s return to that starting condition before we exit the `initializeGL` function.

|**_Task 12:_**|
|:---|
|Unbind your VBO and VAO|

## 4: Drawing in OpenGL

### 4.1 Drawing Overview

Great! Now you are ready to draw the triangle you have worked so hard to create. To do this, these are the general steps you will want to follow each frame.

1. Clear the screen using `glClear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT)`
2. For each shader you want to use:
    1. Bind the shader using glUseProgram(GLuint program)
    2. For each VAO you want to draw with the current shader:
        1. Bind the VAO
        2. Draw using glDrawArrays(GLenum mode, GLint first, GLsizei count
        3. Unbind the VAO
    3. Unbind the shader using glUseProgram(0)

>Note: Notice how all operations including a specific shader and VAO are surrounded by a Bind and Unbind call!

For this, you will only be responsible for steps 1 and 2.ii.a through 2.ii.c. While these loops are best to keep in mind, oftentimes such as this one you will only work with a single shader program and in this case, a single VAO. As such, no loops will need to be present in this section of the lab.

|**_Task 13:_**|
|:---|
|In <code>PaintGL</code>, before <code>glUseProgram</code>, clear the screen|

|**_Task 14:_**|
|:---|
|In <code>PaintGL</code>, after <code>glUseProgra</code>, bind our VAO|

### 4.2 The glDrawArrays Call

As for the last function,

`void glDrawArrays(GLenum mode, GLint first, GLsizei count)`

<details>
<summary>void:</summary> 
Indicates the function does not return
</details>
<details>
<summary>GLenum mode:</summary>
Indicates the type of primitive to render, for our case, use GL_TRIANGLES
</details>
<details>
<summary>GLint first:</summary>
Indicates the starting index for OpenGL to look in our VBO, and will most usually be 0
</details>
<details>
<summary>GLsizei count:</summary>
Indicates how many vertices to draw
</details>

|**_Task 15:_**|
|:---|
|At the end of <code>PaintGL</code>, draw our triangle|

|**_Task 16:_**|
|:---|
|After drawing the triangle, return to default state by unbinding our VAO|

