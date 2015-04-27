#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T16:04:55
#
#-------------------------------------------------

QT       += widgets opengl

QT       -= gui

TARGET = qgl
TEMPLATE = lib

DEFINES += QGL_LIBRARY

SOURCES += ../../../qgl/Camera.cpp \
    ../../../qgl/ManipulatedCameraTransformFrame.cpp \
    ../../../qgl/ManipulatedTransformFrame.cpp \
    ../../../qgl/MouseGrabber.cpp \
    ../../../qgl/QGLRenderWidget.cpp \
    ../../../qgl/TransformFrame.cpp

HEADERS +=../../../qgl/Camera.h \
    ../../../qgl/ManipulatedCameraTransformFrame.h \
    ../../../qgl/ManipulatedTransformFrame.h \
    ../../../qgl/MouseGrabber.h \
    ../../../qgl/QGLRenderWidget.h \
    ../../../qgl/TransformFrame.h \
    ../../../math/ExponentialMap.h \
    ../../../math/Function1D.h \
    ../../../math/Gaussian.h \
    ../../../math/Haar.h \
    ../../../math/Haar_classic.h \
    ../../../math/Haar_classicFreq.h \
    ../../../math/Haar2D.h \
    ../../../math/LiftBase.h \
    ../../../math/MathHead.h \
    ../../../math/Matrices.h \
    ../../../math/Quaternion.h \
    ../../../math/SphericalHarmonic.h \
    ../../../math/Vectors.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ../../../qgl/QGLRenderWidget.ui
