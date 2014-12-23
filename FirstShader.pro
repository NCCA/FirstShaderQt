TARGET=FirstShader
OBJECTS_DIR=obj
# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
cache()
DEFINES +=QT5BUILD
}

MOC_DIR=moc
CONFIG-=app_bundle
QT+=gui opengl core
SOURCES+= main.cpp \
					OpenGLWindow.cpp

HEADERS+= OpenGLWindow.h

INCLUDEPATH +=./

DESTDIR=./
OTHER_FILES+=

CONFIG += console
CONFIG -= app_bundle

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/include/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++*: {
		DEFINES += LINUX
                DEFINES +=GL_GLEXT_PROTOTYPES

}
linux-clang*: {
		DEFINES += LINUX
                DEFINES +=GL_GLEXT_PROTOTYPES

}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN
# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/

win32: {
				DEFINES+=USING_GLEW
				INCLUDEPATH+=-I c:/boost_1_44_0
				INCLUDEPATH+=-I c:/boost

				INCLUDEPATH+= -I C:/NGL/Support/glew
				LIBS+= -L C:/NGL/lib
				LIBS+= -lmingw32
				DEFINES += WIN32
				DEFINES += USING_GLEW
				DEFINES +=GLEW_STATIC
				DEFINES+=_WIN32
				SOURCES+=C:/NGL/Support/glew/glew.c
				INCLUDEPATH+=C:/NGL/Support/glew/
}


