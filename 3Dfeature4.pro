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


DEFINES += _USE_MATH_DEFINES GLM_FORCE_RADIANS

# OpenMesh
INCLUDEPATH += /usr/local/include
LIBS += -lOpenMeshCore

# assimp
INCLUDEPATH += /usr/include/assimp
LIBS += -lassimp

# glew
# INCLUDEPATH +=
LIBS += -lGLEW -lGLU -lGL

# glm
INCLUDEPATH += usr/include/glm

#opencv
INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2 \

LIBS += /usr/local/lib/libopencv_aruco.so.3.1 \
        /usr/local/lib/libopencv_bgsegm.so.3.1 \
        /usr/local/lib/libopencv_bioinspired.so.3.1 \
        /usr/local/lib/libopencv_calib3d.so.3.1 \
        /usr/local/lib/libopencv_ccalib.so.3.1 \
        /usr/local/lib/libopencv_core.so.3.1 \
        /usr/local/lib/libopencv_datasets.so.3.1 \
        /usr/local/lib/libopencv_dnn.so.3.1 \
        /usr/local/lib/libopencv_dpm.so.3.1 \
        /usr/local/lib/libopencv_face.so.3.1 \
        /usr/local/lib/libopencv_features2d.so.3.1 \
        /usr/local/lib/libopencv_flann.so.3.1 \
        /usr/local/lib/libopencv_fuzzy.so.3.1 \
        /usr/local/lib/libopencv_hdf.so.3.1 \
        /usr/local/lib/libopencv_highgui.so.3.1 \
        /usr/local/lib/libopencv_imgcodecs.so.3.1 \
        /usr/local/lib/libopencv_imgproc.so.3.1 \
        /usr/local/lib/libopencv_line_descriptor.so.3.1 \
        /usr/local/lib/libopencv_ml.so.3.1 \
        /usr/local/lib/libopencv_objdetect.so.3.1 \
        /usr/local/lib/libopencv_optflow.so.3.1 \
        /usr/local/lib/libopencv_photo.so.3.1 \
        /usr/local/lib/libopencv_plot.so.3.1 \
        /usr/local/lib/libopencv_reg.so.3.1 \
        /usr/local/lib/libopencv_rgbd.so.3.1 \
        /usr/local/lib/libopencv_saliency.so.3.1 \
        /usr/local/lib/libopencv_sfm.so.3.1 \
        /usr/local/lib/libopencv_shape.so.3.1 \
        /usr/local/lib/libopencv_stereo.so.3.1 \
        /usr/local/lib/libopencv_stitching.so.3.1 \
        /usr/local/lib/libopencv_structured_light.so.3.1 \
        /usr/local/lib/libopencv_superres.so.3.1 \
        /usr/local/lib/libopencv_surface_matching.so.3.1 \
        /usr/local/lib/libopencv_text.so.3.1 \
        /usr/local/lib/libopencv_tracking.so.3.1 \
        /usr/local/lib/libopencv_videoio.so.3.1 \
        /usr/local/lib/libopencv_video.so.3.1 \
        /usr/local/lib/libopencv_videostab.so.3.1 \
        /usr/local/lib/libopencv_viz.so.3.1 \
        /usr/local/lib/libopencv_xfeatures2d.so.3.1 \
        /usr/local/lib/libopencv_ximgproc.so.3.1 \
        /usr/local/lib/libopencv_xobjdetect.so.3.1 \
        /usr/local/lib/libopencv_xphoto.so.3.1


