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
    void connectSlots(); // connects the buttons to functions

    // basal functions
    void setCurrBasalRate(size_t rate); // sets currBasalRate + updates the text in ui
    void startBasalDelivery();
    void pauseBasal();
    void stopBasal();
    void autoStopBasal();

    // bolus functions
    void startBolusDelivery(double bolus);
//    void pauseBolus();
//    void stopBolus();

    // i kept these 2 functions for now cause i think jessica re
    //    void autoStopInsulin();
    //    void manualStopInsulin();

private:
    Ui::MainWindow *_ui;
    machine *_machine;

    int currGlucose; // i think i grab this from machine
    double currBasalRate;
    bool isDeliveringInsulin;
    bool isPausedBasal;
    bool isPausedBolus;
};

#endif // INSULIN_H
