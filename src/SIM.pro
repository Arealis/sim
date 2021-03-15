QT       += core gui \
            sql \
            printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG(debug, debug|release) {
    system("mkdir" $$system_path($${OUT_PWD}/debug/db))
    system("mkdir" $$system_path($${OUT_PWD}/debug/logo))
} else {
    system("mkdir" $$system_path($${OUT_PWD}/release/db))
    system("mkdir" $$system_path($${OUT_PWD}/release/logo))
}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    createdocument.cpp \
    editcompanyinfo.cpp \
    main.cpp \
    mainwindow.cpp \
    userlogin.cpp

HEADERS += \
    createdocument.h \
    editcompanyinfo.h \
    global.h \
    mainwindow.h \
    userlogin.h

FORMS += \
    createdocument.ui \
    editcompanyinfo.ui \
    mainwindow.ui \
    userlogin.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

