#ifndef INSULIN_H
#define INSULIN_H

#include "ui_mainwindow.h"

class Insulin
{
public:
    Insulin(Ui::MainWindow *ui);
    void autoStopInsulin();

private:
    Ui::MainWindow *_ui;
    int insulinOnBoard;
};

#endif // INSULIN_H
