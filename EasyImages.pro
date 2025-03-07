QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colorslider.cpp \
    colorsliderform.cpp \
    colorwheel.cpp \
    cropbox.cpp \
    customitemclass.cpp \
    customitemlayer.cpp \
    customlabelcolor.cpp \
    custompatharrow.cpp \
    custompropertyform.cpp \
    customtreewidgetlayer.cpp \
    customundo.cpp \
    cutimage.cpp \
    dialogaline.cpp \
    dialogbina.cpp \
    dialogcoldco.cpp \
    dialogcolorreplace.cpp \
    dialogcontract.cpp \
    dialoggetcolor.cpp \
    dialoghsv.cpp \
    dialoglightness.cpp \
    dialogrgb.cpp \
    dialogwarmco.cpp \
    globalclicklistener.cpp \
    graphicdrawview.cpp \
    imageshowlabel.cpp \
    main.cpp \
    themainwindow.cpp

HEADERS += \
    colorslider.h \
    colorsliderform.h \
    colorwheel.h \
    cropbox.h \
    customitemclass.h \
    customitemlayer.h \
    customlabelcolor.h \
    custompatharrow.h \
    custompropertyform.h \
    customtreewidgetlayer.h \
    customundo.h \
    cutimage.h \
    dialogaline.h \
    dialogbina.h \
    dialogcoldco.h \
    dialogcolorreplace.h \
    dialogcontract.h \
    dialoggetcolor.h \
    dialoghsv.h \
    dialoglightness.h \
    dialogrgb.h \
    dialogwarmco.h \
    globalclicklistener.h \
    graphicdrawview.h \
    imageshowlabel.h \
    themainwindow.h

FORMS += \
    colorsliderform.ui \
    custompropertyform.ui \
    cutimage.ui \
    dialogaline.ui \
    dialogbina.ui \
    dialogcoldco.ui \
    dialogcolorreplace.ui \
    dialogcontract.ui \
    dialoggetcolor.ui \
    dialoghsv.ui \
    dialoglightness.ui \
    dialogrgb.ui \
    dialogwarmco.ui \
    themainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# DISTFILES +=

# RC_FILE = IconSet.rc
