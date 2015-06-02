#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T16:56:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kidsudoku_editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/ColorCombo.cpp \
    widgets/EnumProperty.cpp \
    widgets/Property.cpp \
    widgets/QPropertyEditorWidget.cpp \
    widgets/QPropertyModel.cpp \
    widgets/QVariantDelegate.cpp \
    QStageData.cpp \
    QCampaignData.cpp

HEADERS  += mainwindow.h \
    widgets/ColorCombo.h \
    widgets/EnumProperty.h \
    widgets/Property.h \
    widgets/QPropertyEditorWidget.h \
    widgets/QPropertyModel.h \
    widgets/QVariantDelegate.h \
    QStageData.h \
    QCampaignData.h \
    portable.h

FORMS    += mainwindow.ui
