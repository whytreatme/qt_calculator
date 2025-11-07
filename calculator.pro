QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = calculator

# 头文件、源文件、UI、资源文件都放在 src 和 resources 下
SOURCES += \
    src/main.cpp \
    src/widget.cpp

HEADERS += \
    src/widget.h

FORMS += \
    src/widget.ui

RESOURCES += resources/resources.qrc

# 如果你要添加 include 目录，也可以这样写
# INCLUDEPATH += src
