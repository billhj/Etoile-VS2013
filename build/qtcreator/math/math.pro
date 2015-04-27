#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T15:58:20
#
#-------------------------------------------------

TARGET = Math
TEMPLATE = lib

DEFINES += MATH_LIBRARY

SOURCES += math.cpp

HEADERS += math.h \
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
