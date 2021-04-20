TEMPLATE = app
CONFIG += console c++2a
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lSDL2 -lassimp

SOURCES += \
        AppSystem.cpp \
        BasicPipeline.cpp \
        Camera.cpp \
        EdgeFunction.cpp \
        LoadImage.cpp \
        ModelPipeline.cpp \
        SoftwareRendererSystem.cpp \
        Texture.cpp \
        TexturedPipeline.cpp \
        ZBuffer.cpp \
        main.cpp

HEADERS += \
	AppSystem.hpp \
	BasicPipeline.hpp \
	Camera.hpp \
	Dither.hpp \
	EdgeFunction.hpp \
	FixedStack.hpp \
	LoadImage.hpp \
	ModelPipeline.hpp \
	Normalize.hpp \
	RenderingPipeline.hpp \
	SoftwareRendererSystem.hpp \
	StandardPixelType.hpp \
	StandardTexture.hpp \
	Texture.hpp \
	TexturedPipeline.hpp \
	VertexClipper.hpp \
	ZBuffer.hpp \
	stb_image.h

DISTFILES += \
	brickwall.png \
	cube.dae \
	vines.png
