QT += widgets opengl openglwidgets gui

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glrenderer.cpp \
    main.cpp \
    mainwindow.cpp \
    glew-2.2.0/src/glew.c

HEADERS += \
    CS1230Lib/resourceloader.h \
    glrenderer.h \
    mainwindow.h \
    glew-2.2.0/include/GL/glew.h


unix:!macx{
    LIBS += lGLU
}

win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

INCLUDEPATH += glm glew-2.2.0/include
DEPENDPATH += glm glew-2.2.0/include

DISTFILES += \
    Resources/Shaders/default.frag \
    Resources/Shaders/default.vert \
