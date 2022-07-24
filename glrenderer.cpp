#include "glrenderer.h"

#include <QCoreApplication>
#include "CS1230Lib/resourceloader.h"

GLRenderer::GLRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

GLRenderer::~GLRenderer()
{
    makeCurrent();
    doneCurrent();
}

void GLRenderer::initializeGL()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));

    //TASK 2: Set the clear color here
    glClearColor(0.0, 0.0, 0.0, 1.0);

    m_shader = ResourceLoader::createShaderProgram("Resources/Shaders/default.vert", "Resources/Shaders/default.frag"); //Shader setup (DO NOT EDIT)

    // Vertex Buffer Objects //

    //TASK 3: Generate a VBO here and store it in m_vbo
    glGenBuffers(1, &m_vbo);

    //TASK 4: Bind the VBO you created here
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    //TASK 6: Construct your std::vector of triangle data here | TASK 8: Add colors to your triangle here
    std::vector<GLfloat> triangle =
    {   //    POSITIONS    //    COLORS    //
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    //TASK 7: Pass the triangle vector into your VBO here
    glBufferData(GL_ARRAY_BUFFER, triangle.size()*sizeof(GLfloat), triangle.data(), GL_STATIC_DRAW);


    // Vertex Array Objects //

    //TASK 9: Generate a VAO here and store it in m_vao
    glGenVertexArrays(1, &m_vao);

    //TASK 10: Bind the VAO you created here
    glBindVertexArray(m_vao);

    //TASK 11: Add position and color attributes to your VAO here
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));


    // Returning to Default State //

    //TASK 12: Unbind your VBO and VAO here
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLRenderer::paintGL()
{
    //TASK 13: Clear the screen here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shader);

    //TASK 14: Bind your VAO here
    glBindVertexArray(m_vao);

    //TASK 15: Draw your VAO here
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //TASK 16: Unbind your VAO here
    glBindVertexArray(0);

    glUseProgram(0);
}

void GLRenderer::resizeGL(int w, int h)
{

}
