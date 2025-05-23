#ifndef BOLUS_H
#define BOLUS_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "profile.h"
#include "machine.h"
#include "insulin.h"

class Bolus : public QObject
{

public:
    // constructor
    Bolus(Ui::MainWindow *ui, machine *machine, Insulin *insulin);

    // destructor
    ~Bolus();

    // calculator
    void viewCalculation(); // calls regular bolus calculation
    void cgmCalculation();  // calls CGM bolus calculation IN PROGRESS
    void bolusCalculation(int carbs, float glucose, int insulinOnBoard);

    // features- press button to administer/activate
    void startBolus();
    //    void manualBolus();
    void immediateBolus();
    void extendedBolus();
    void stopOngoingBolus();
    void continueBolus();
    void cancelBolus();

    // Step function
    void stepBolus(); // this will be used to manage amount of insulin in machine

private:
    Ui::MainWindow *_ui;
    machine *thisMachine;
    Insulin *_insulin;
    Profile *currProfile;

    float currGlucose; // mmol/L
    int carbohydrates; // grams
    int currIOB;

    int bolusOption;       // 1, 2 = immediate, extended
    int immediateFraction; // immediate bolus fraction
    int extendedCount;     // amount of minutes left of extended bolus- decrement by 5
    float extendedFullAmt;
    float extendedPortion; // certain amount of the extended bolus that is administered every 5 mins

    float immediateAmt;

    // holding values while bolus is paused
    bool bolusPaused;
    float bPausedCount;
    float bPausedAmt;

    // probably have something that prevents harmful amounts of insulin being used?

}; // Bolus class

#endif
