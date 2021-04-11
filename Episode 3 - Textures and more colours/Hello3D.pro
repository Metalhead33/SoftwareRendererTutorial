TEMPLATE = app
CONFIG += console c++2a
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lSDL2

SOURCES += \
        AppSystem.cpp \
        BasicPipeline.cpp \
        EdgeFunction.cpp \
        LoadImage.cpp \
        SoftwareRendererSystem.cpp \
        Texture.cpp \
        TexturedPipeline.cpp \
        main.cpp

HEADERS += \
	AppSystem.hpp \
	BasicPipeline.hpp \
	Dither.hpp \
	EdgeFunction.hpp \
	LoadImage.hpp \
	Normalize.hpp \
	RenderingPipeline.hpp \
	SoftwareRendererSystem.hpp \
	StandardPixelType.hpp \
	StandardTexture.hpp \
	Texture.hpp \
	TexturedPipeline.hpp \
	stb_image.h

DISTFILES += \
	brickwall.png
