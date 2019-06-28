#-------------------------------------------------
#
# Project created by QtCreator 2018-06-21T08:59:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zoweerobot
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH +=Code
INCLUDEPATH +=Ctrl
INCLUDEPATH +=Devs
INCLUDEPATH +=Glob
INCLUDEPATH +=Thread
INCLUDEPATH +=UI
INCLUDEPATH +=Ctrl/motioncode

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Code/ActionMGCode.cpp \
    Code/ActionMoveGCode.cpp \
    Code/ActionPunchGCode.cpp \
    Code/ActionReferenceGCode.cpp \
    Code/BaseGCode.cpp \
    Code/CoordinateGcode.cpp \
    Code/MacroGCode.cpp \
    Code/MacroRunGCode.cpp \
    Code/ParameteGcode.cpp \
    Code/ProcessCALLGCode.cpp \
    Code/ProcessPCALLGCode.cpp \
    Ctrl/DesEncryptionModel.cpp \
    Ctrl/ParseGCodeModel.cpp \
    Devs/BaseDevice.cpp \
    Devs/EmulationControllerDevice.cpp \
    Devs/MotionControllerDevice.cpp \
    Devs/NetCtrl.cpp \
    Devs/NetSocket.cpp \
    Devs/TCPSocket.cpp \
    Devs/XmlContrrollerDevice.cpp \
    Glob/BaseAction.cpp \
    Glob/DelayAction.cpp \
    Glob/IOAction.cpp \
    Glob/MGAction.cpp \
    Glob/MoveAction.cpp \
    Glob/PunchAction.cpp \
    Glob/ToolChangeAction.cpp \
    Glob/ToolInfo.cpp \
    Glob/ToolRotateAction.cpp \
    Thread/BaseModule.cpp \
    Thread/MotionControllerModule.cpp \
    Thread/XmlControllerModule.cpp \
    UI/cautomachiningstateui.cpp \
    UI/ccommmomuiwidget.cpp \
    UI/cemulationcanvs.cpp \
    UI/clogindialog.cpp \
    UI/codeeditor.cpp \
    SystemProcessInfo.cpp \
    SystemSchedule.cpp \
    UI/cmanualmachiningstateui.cpp \
    UI/csystempreferencesui.cpp \
    Ctrl/motioncode/arcmotioncode.cpp \
    Ctrl/motioncode/basemotioncode.cpp \
    Ctrl/motioncode/circlemotioncode.cpp \
    Ctrl/motioncode/delaymotioncode.cpp \
    Ctrl/motioncode/iomotioncode.cpp \
    Ctrl/motioncode/lasercraftmotioncode.cpp \
    Ctrl/motioncode/linemotioncode.cpp \
    Ctrl/motioncode/movemotioncode.cpp \
    Ctrl/motioncode/waitiomotioncode.cpp \
    Ctrl/ProcEmulationModel.cpp \
    UI/ctoolcoorsetui.cpp \
    UI/cjointparamsetui.cpp \
    UI/cusercoorsetui.cpp \
    UI/cteachpointui.cpp \
    TDialog.cpp



HEADERS += \
        mainwindow.h \
    Code/ActionMGCode.h \
    Code/ActionMoveGCode.h \
    Code/ActionPunchGCode.h \
    Code/ActionReferenceGCode.h \
    Code/BaseGCode.h \
    Code/CoordinateGcode.h \
    Code/GCodeComonType.h \
    Code/MacroGCode.h \
    Code/MacroRunGCode.h \
    Code/ParameteGcode.h \
    Code/ProcessCALLGCode.h \
    Code/ProcessPCALLGCode.h \
    Ctrl/DesEncryptionModel.h \
    Ctrl/ParseGCodeModel.h \
    Devs/BaseDevice.h \
    Devs/EmulationControllerDevice.h \
    Devs/MotionControllerDevice.h \
    Devs/NetCtrl.h \
    Devs/NetSocket.h \
    Devs/TCPSocket.h \
    Devs/XmlContrrollerDevice.h \
    Glob/BaseAction.h \
    Glob/DelayAction.h \
    Glob/GlobalVariableType.h \
    Glob/IOAction.h \
    Glob/MGAction.h \
    Glob/MoveAction.h \
    Glob/PunchAction.h \
    Glob/ToolChangeAction.h \
    Glob/ToolInfo.h \
    Glob/ToolRotateAction.h \
    Thread/BaseModule.h \
    Thread/MotionControllerModule.h \
    Thread/XmlControllerModule.h \
    UI/cautomachiningstateui.h \
    UI/ccommmomuiwidget.h \
    UI/cemulationcanvs.h \
    UI/clogindialog.h \
    UI/codeeditor.h \
    ComonTypes.h \
    SystemProcessInfo.h \
    SystemSchedule.h \
    UI/cmanualmachiningstateui.h \
    UI/csystempreferencesui.h \
    Devs/GlobalDefine.h \
    Ctrl/motioncode/arcmotioncode.h \
    Ctrl/motioncode/basemotioncode.h \
    Ctrl/motioncode/circlemotioncode.h \
    Ctrl/motioncode/delaymotioncode.h \
    Ctrl/motioncode/iomotioncode.h \
    Ctrl/motioncode/lasercraftmotioncode.h \
    Ctrl/motioncode/linemotioncode.h \
    Ctrl/motioncode/movemotioncode.h \
    Ctrl/motioncode/waitiomotioncode.h \
    Ctrl/ProcEmulationModel.h \
    UI/ctoolcoorsetui.h \
    UI/cjointparamsetui.h \
    UI/cusercoorsetui.h \
    UI/cteachpointui.h \
    TDialog.h




FORMS += \
        mainwindow.ui \
    UI/cautomachiningstateui.ui \
    UI/clogindialog.ui \
    UI/cmanualmachiningstateui.ui \
    UI/csystempreferencesui.ui \
    UI/ctoolcoorsetui.ui \
    UI/cjointparamsetui.ui \
    UI/cusercoorsetui.ui \
    UI/cteachpointui.ui \
    TDialog.ui

RESOURCES += \
    UI/images.qrc


RC_FILE += UI/icon.rc

QT += xml
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += serialport
