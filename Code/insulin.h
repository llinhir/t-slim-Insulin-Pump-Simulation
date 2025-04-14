#ifndef INSULIN_H
#define INSULIN_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "machine.h"
#include "profile.h"

class Insulin : public QObject
{
public:
    Insulin(Ui::MainWindow *ui, machine *machine);

    void setBasalRates(); // grabs the basal rates from profiles -> sets them as options

    void startInsulinDelivery(float insulinAmt);
    void pauseInsulin();

    // stoping insulin
    void autoStopInsulin();
    void manualStopInsulin();


private:
    Ui::MainWindow *_ui;
    machine* _machine;
    int currGlucose; // i think i grab this from machine
    double currBasalRate;
    bool isDeliveringInsulin;
};

#endif // INSULIN_H
