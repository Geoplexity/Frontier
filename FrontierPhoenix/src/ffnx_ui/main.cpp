#include "ffnx_mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    auto window = new ffnx::ui::FFNXMainWindow();
    window->show();

    return QApplication::exec();
}