#include "bolus.h"

#include <iostream>
#include <iomanip>
using namespace std;

Bolus::Bolus()
{

}

float Bolus::bolusCalculation(){ // get from CGM / profile
    float bolusCalculated = 0;

    //bolusCalculated = Profile::getGlucose() / Profile::getCarbs(); // NOT REAL CALCULATION!!!!!!!!!
     cout << "View Calculation: " << fixed << setprecision(5) << bolusCalculated << endl;
     return bolusCalculated;
}

float Bolus::bolusCalculation(float glucose, int carbs){ // get these from ui
    float bolusCalculated = 0;

    bolusCalculated = glucose / carbs; // NOT REAL CALCULATION!!!!!!
    cout << "View Calculation: " << fixed << setprecision(5) << bolusCalculated << endl;
    return bolusCalculated;
}

// (m->loginAttempt(input)
// make sure it can only be numbers
