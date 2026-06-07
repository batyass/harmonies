QT += core gui widgets
CONFIG += c++17
CONFIG -= app_bundle
TEMPLATE = app
TARGET = harmonies
INCLUDEPATH += include
SOURCES += src/main.cpp \
    $$files(src/core/*.cpp) \
    $$files(src/model/*.cpp) \
    $$files(src/rules/*.cpp) \
    $$files(src/scoring/*.cpp) \
    $$files(src/ui/*.cpp) \
    $$files(src/utils/*.cpp)
HEADERS += \
    $$files(include/core/*.h) \
    $$files(include/model/*.h) \
    $$files(include/rules/*.h) \
    $$files(include/scoring/*.h) \
    $$files(include/ui/*.h) \
    $$files(include/utils/*.h)
