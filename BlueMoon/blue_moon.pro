TEMPLATE = app
TARGET = blue_moon

QT = core bluetooth widgets

SOURCES = \
    main.cpp \
    remoteselector.cpp \
    progress.cpp \
    pindialog.cpp

HEADERS = \
    remoteselector.h \
    progress.h \
    pindialog.h

FORMS = \
    remoteselector.ui \
    progress.ui \
    pindialog.ui

OTHER_FILES += \
    busy.gif \
    pairing.gif

RESOURCES += \
    blue_moon.qrc

