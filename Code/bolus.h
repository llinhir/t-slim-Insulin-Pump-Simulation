#ifndef BOLUS_H
#define BOLUS_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "profile.h"

class Bolus{

    public:
        // constructor
        Bolus();

        // destructor
        ~Bolus();

        // calculator
        float bolusCalculation(); // values pulled automatically from the CGM
        float bolusCalculation(float glucose, int carbs); // or should these just be part of the constructor????????
        // view calculation
        // administer bolus??

        // features
        // manualBolus();
        // quickBolus();
        // extendedBolus();
        // cancelBolus();
        // stopOngoingBolus();

        // getters
        float getGlucose(){return currGlucose;};
        int getCarbs(){return carbohydrates;};

        // setters
        void setGlucose(float glucose){currGlucose = glucose;};
        void setCarbs(int carbs){carbohydrates = carbs;};

    private:
        float currGlucose; // mmol/L
        int carbohydrates; // grams

}; // Bolus class

#endif
