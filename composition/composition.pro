SOURCES += main.cpp composition.cpp \
    TargetCircleWidget.cpp \
    Skin.cpp
HEADERS += composition.h \
    TargetCircleWidget.h \
    Skin.h

SHARED_FOLDER = ../shared

include($$SHARED_FOLDER/shared.pri)

RESOURCES += composition.qrc
qtHaveModule(opengl): !contains(QT_CONFIG,dynamicgl) {
    DEFINES += USE_OPENGL
    QT += opengl svg
}
QT += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/painting/composition
INSTALLS += target


win32-msvc* {
    QMAKE_CXXFLAGS += /Zm500
    QMAKE_CFLAGS += /Zm500
}

wince* {
    DEPLOYMENT_PLUGIN += qjpeg
}
