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
    void viewCalculation();                                              // calls regular bolus calculation
    void cgmCalculation();                                               // calls CGM bolus calculation IN PROGRESS
    void bolusCalculation(int carbs, float glucose, int insulinOnBoard);

    // features- press button to administer/activate
    void manualBolus();
    void immediateBolus();
    void extendedBolus();
    void stopOngoingBolus();
    void cancelBolus();

    // getters
    float getGlucose() { return currGlucose; }; // IDK IF IT IS GOOD TO HAVE THESE HERE
    int getCarbs() { return carbohydrates; };   // BECAUSE THEN STUFF WOULD HAVE TO HAVE A BOLUS

    // setters
    void setGlucose(float glucose) { currGlucose = glucose; };
    void setCarbs(int carbs) { carbohydrates = carbs; };

    // Step function
    void stepBolus(); // this will be used to manage amount of insulin in machine

private:
    Ui::MainWindow *_ui;
    float currGlucose; // mmol/L
    int carbohydrates; // grams
    machine *thisMachine;
    Insulin *_insulin;
    Profile *currProfile;
    float currBolus;

}; // Bolus class

#endif
