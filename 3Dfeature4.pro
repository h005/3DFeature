#-------------------------------------------------
#
# Project created by QtCreator 2015-08-03T22:46:24
#
#-------------------------------------------------

QT       += widgets
DIRECTORIES = .

QT       -= gui

TARGET = 3Dfeature4

CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cc \
    mainwindow.cc \
    glwidget.cc \
    shader.cc \
    trackball.cc \
    feature.cc \
    images.cc \
    fea.cpp

OTHER_FILES += shader/*.vert shader/*.frag

DEFINES += _USE_MATH_DEFINES

win32 {
    DP_TOOLS_DIR = $$(DP_TOOLS_DIR)

    # OpenMesh
    INCLUDEPATH += $$DP_TOOLS_DIR/openmesh/include
    LIBS += -L$$DP_TOOLS_DIR/openmesh/lib/vs2013 -lOpenMeshCored

    # assimp
    LIBS += -L$$DP_TOOLS_DIR/assimp-3.1.1-win-binaries/build/code/Release -lassimp
    INCLUDEPATH += $$DP_TOOLS_DIR/assimp-3.1.1-win-binaries/include

    # glew
    INCLUDEPATH += $$DP_TOOLS_DIR/glew/include
    LIBS += -L$$DP_TOOLS_DIR/glew/lib/Release/Win32 -lglew32
}

# glm
INCLUDEPATH += D:/tools/glm

defineTest(copyToDestdir) {
    files = $$1

    for(FILE, files) {
        DDIR = $$OUT_PWD

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}
copyToDestdir($$_PRO_FILE_PWD_/shader)

HEADERS += \
    common.hh \
    mainwindow.hh \
    externalimporter.hh \
    glwidget.hh \
    shader.hh \
    trackball.hh \
    meshglhelper.hh \
    feature.hh \
    images.hh \
    fea.h \
    gausscurvature.hh \
    meancurvature.hh \
    abstractfeature.hh \
    curvature.hh \
    colormap.hh

#opencv
INCLUDEPATH += D:\tools\opencv\build\include\opencv\
                D:\tools\opencv\build\include\opencv2\
                D:\tools\opencv\build\include

LIBS += D:/tools/opencv/build/x86/vc12/lib/*.lib

