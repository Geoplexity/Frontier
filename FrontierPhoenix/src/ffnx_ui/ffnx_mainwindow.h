#ifndef FFNX_UI_FFNX_MAINWINDOW_H
#define FFNX_UI_FFNX_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>

class FFNXMainWindow : public QMainWindow {

public:
    FFNXMainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("FFNX UI");
    }

};

#endif