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
    QStageData.cpp \
    QCampaignData.cpp \
    SudokuUtil.cpp \
    QSudokuBoxModel.cpp

HEADERS  += mainwindow.h \
    QStageData.h \
    QCampaignData.h \
    portable.h \
    SudokuUtil.h \
    QSudokuBoxModel.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
