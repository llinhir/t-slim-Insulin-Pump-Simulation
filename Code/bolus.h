#ifndef BOLUS_H
#define BOLUS_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "profile.h"
#include "machine.h"

class Bolus : public QObject
{

public:
    // constructor
    Bolus(Ui::MainWindow *ui, machine *machine);

    // destructor
    ~Bolus();

    // calculator
    void viewCalculation();                                              // calls regular bolus calculation
    void cgmCalculation();                                               // calls CGM bolus calculation IN PROGRESS
    void bolusCalculation(int carbs, float glucose, int insulinOnBoard); // or should these just be part of the constructor????????
    // view calculation
    // administer bolus??

    // features
    // manualBolus();
    // quickBolus();
    // extendedBolus();
    // cancelBolus();
    // stopOngoingBolus();

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
    Profile *currProfile;

}; // Bolus class

#endif
