QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    Matrix4x4/matrix4x4.cpp \
    Matrix4x4/vertex.cpp \
    beziercurve.cpp \
    billboard.cpp \
    boundingshape.cpp \
    camera.cpp \
    cube.cpp \
    disc.cpp \
    door.cpp \
    door1.cpp \
    door2.cpp \
    enemy.cpp \
    equidistance.cpp \
    examscene.cpp \
    heightmap.cpp\
    house.cpp \
    interactiveobject.cpp \
    lazsurface.cpp \
    light.cpp \
    logger.cpp \
    mainwindow.cpp \
    npc.cpp \
    obj.cpp \
    octahedronball.cpp \
    physicsscene.cpp \
    pointcloud.cpp \
    projectile.cpp \
    quad.cpp \
    renderwindow.cpp \
    rollingball.cpp \
    route.cpp \
    scene.cpp \
    scene1.cpp \
    scene2.cpp \
    scene3.cpp \
    shader.cpp \
    shaderhandler.cpp \
    skybox.cpp \
    sortcoords.cpp \
    square.cpp \
    tetrahedron.cpp \
    texture.cpp \
    token.cpp \
    triangle.cpp \
    trianglesurface.cpp \
    visualobject.cpp \
    visualpoint.cpp \
    xyz.cpp

HEADERS += \
    Matrix4x4/matrix4x4.h \
    Matrix4x4/vector3d.h \
    Matrix4x4/vertex.h \
    beziercurve.h \
    billboard.h \
    boundingshape.h \
    camera.h \
    cube.h \
    disc.h \
    door.h \
    door1.h \
    door2.h \
    enemy.h \
    equidistance.h \
    examscene.h \
    heightmap.h\
    house.h \
    interactiveobject.h \
    lazsurface.h \
    light.h \
    logger.h \
    mainwindow.h \
    npc.h \
    obj.h \
    octahedronball.h \
    physicsscene.h \
    pointcloud.h \
    projectile.h \
    quad.h \
    renderwindow.h \
    rollingball.h \
    route.h \
    scene.h \
    scene1.h \
    scene2.h \
    scene3.h \
    shader.h \
    shaderhandler.h \
    skybox.h \
    sortcoords.h \
    square.h \
    tetrahedron.h \
    texture.h \
    token.h \
    tquadtre.h \
    triangle.h \
    trianglesurface.h \
    visualobject.h \
    visualpoint.h \
    xyz.h

    # external libs
#    stb_image/stb_image.h \
    glm/glm.hpp

FORMS += \
    mainwindow.ui

DISTFILES += \
    phongshader.frag \
    phongshader.vert \
    plainshader.frag \
    plainshader.vert \
    skyboxshader.frag \
    skyboxshader.vert \
    textureshader.frag \
    textureshader.vert 
