QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../model/modeldata.cc \
    ../model/preparer.cc \
    main.cpp \
    mainwindow.cc \
    viewwidget.cc \
    ../controller/controller.cc \
    ../model/parser.cc

HEADERS += \
    ../model/modeldata.h \
    ../model/observable.h \
    ../model/observer.h \
    ../model/preparer.h \
    mainwindow.h \
    viewwidget.h \
    ../controller/controller.h \
    ../model/parser.h
    #../libs/QtGifImage-master/src/gifimage/qgifimage.h

FORMS += \
    mainwindow.ui

#LIBS += -lglut32 -lOpenGL #-lopengl32 -lGLU32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourses.qrc
