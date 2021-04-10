TEMPLATE = app
CONFIG += console c++2a
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lSDL2

SOURCES += \
        AppSystem.cpp \
        BasicPipeline.cpp \
        EdgeFunction.cpp \
        SoftwareRendererSystem.cpp \
        main.cpp

HEADERS += \
	AppSystem.hpp \
	BasicPipeline.hpp \
	EdgeFunction.hpp \
	RenderingPipeline.hpp \
	SoftwareRendererSystem.hpp \
	StandardPixelType.hpp \
	StandardTexture.hpp \
	Texture.hpp
