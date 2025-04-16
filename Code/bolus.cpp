#include "bolus.h"

#include <iostream>
#include <iomanip>
using namespace std;

Bolus::Bolus(Ui::MainWindow *ui, machine *machine, Insulin *insulin)
{
    thisMachine = machine;
    _ui = ui;
    _insulin = insulin;
    extendedCount = 1;

    currProfile = thisMachine->getCurrentProfile();
    connect(ui->viewCalculationButton, &QPushButton::clicked, this, [this]()
            { viewCalculation(); });
    connect(ui->viewCalculationButton_2, &QPushButton::clicked, this, [this]()
            { cgmCalculation(); });
    connect(ui->selectProfileButton, &QPushButton::clicked, this, [this]()
            { _ui->stackedWidget->setCurrentIndex(SELECT_ACTIVE_PROFILE_PAGE); });

    connect(ui->startManualBolus, &QPushButton::clicked, this, [this]()
            { manualBolus(); });
    connect(ui->immediateButton, &QPushButton::clicked, this, [this]()
            { immediateBolus(); });
    connect(ui->extendedButton, &QPushButton::clicked, this, [this]()
            { extendedBolus(); });

    connect(ui->pauseBolusButton, &QPushButton::clicked, this, [this]()
            { stopOngoingBolus(); });
    connect(ui->cancelBolusButton, &QPushButton::clicked, this, [this]()
            { cancelBolus(); });
}

Bolus::~Bolus()
{
}

void Bolus::viewCalculation()
{
    if(_ui->editManualBolus->text().toFloat() != 0 || _ui->addGlucoseButton->text().toFloat() == 0){
        // do nothing // improve
        cout << "Please add values to GLUCOSE (and CARBS if necessary) before calculating, select the CGM calculation for use of stored values, or clear the manual bolus textbox." << endl;
    } else{
    // Update current profile based on selected
    currProfile = thisMachine->getCurrentProfile();

    // Taking input from the user
    carbohydrates = _ui->addCarbsButton->text().toFloat();
    currGlucose = _ui->addGlucoseButton->text().toFloat();
    cout << "Carbs: " << carbohydrates << ", Glucose: " << currGlucose << ", IOB: " << thisMachine->getIOB() << endl;
    bolusCalculation(carbohydrates, currGlucose, thisMachine->getIOB());
    }
}

// Calculates final bolus based on user input
void Bolus::bolusCalculation(int carbs, float glucose, int insulinOnBoard)
{
    cout << "Starting calculation..." << endl;
    float foodBolus = carbs / currProfile->getCarbohydrateRatio();
    float correctionBolus = (glucose - currProfile->getTargetGlucoseLevel()) / currProfile->getCorrectionFactor();

    float totalBolus = foodBolus + correctionBolus;
    float finalBolus = totalBolus - insulinOnBoard;

    cout << "View Calculation: " << fixed << setprecision(2) << finalBolus << endl;
    QString floatString = QString::number(finalBolus, 'f', 2);
    _ui->editManualBolus->setText(floatString);
}

void Bolus::cgmCalculation() // fix to do what Kathy said last night pls :)
{ // ALSO MAY HHAVE TO REWORK CALCULATIONS A BIT- DOUBLE CHECK TEST EVERYTHING
    cout << "Getting CGM values..." << endl;
    if(_ui->editManualBolus->text().toFloat() != 0 || _ui->addCarbsButton->text().toFloat() == 0){
        // do nothing
        cout << "Please enter carbs and/or revert manual bolus to 0." << endl;
    } else{
        // Update current profile based on selected
        currProfile = thisMachine->getCurrentProfile();
        carbohydrates = _ui->addCarbsButton->text().toFloat();
        cout << "Carbs: " << carbohydrates << ", Glucose: " << thisMachine->getCurrentGlucose() << ", IOB: " << thisMachine->getIOB() << endl;
        bolusCalculation(carbohydrates, thisMachine->getCurrentGlucose(), thisMachine->getIOB());
    }
}

void Bolus::stepBolus(float exBol) // FIX- needs to be connected to the timer or stepper
{
    // I am using this wrong for now potentially- for extended bolus. can be a diff function if needed
    if(extendedCount > 1 && thisMachine->getHourStepCounter() >= 12){
        thisMachine->consumeInsulin(exBol);
        extendedCount--;
        cout << extendedCount - 1 << "hours left of extended bolus." << endl;
    }
}

// IN PROGRESS // + need to add to logger and probably in Insulin add to history vector

void Bolus::manualBolus()
{
    float finalBolus = _ui->editManualBolus->text().toFloat();
    _insulin->startBolusDelivery(finalBolus); // give float to function as param
    // take parameter(finalBolus, how many hours) and take the insulin out of device and deliver to user
    // change values accordingly
}

void Bolus::immediateBolus()
{                                                              // FIGURE OUT order of stuff!! and give IOB to someone
    float finalBolus = _ui->editManualBolus->text().toFloat(); // or have an in-house var to take from
    double immediateBolus = finalBolus * 0.6;
    cout << "Immediate Bolus (60%): " << immediateBolus << endl;
    thisMachine->consumeInsulin(immediateBolus); // replaced the insulin function call cause it's just 1 more unecessary level
}

void Bolus::extendedBolus()
{
    float finalBolus = _ui->editManualBolus->text().toFloat();
    float extendedBolus = finalBolus * 0.4;

    float hours = _ui->timeEdit->text().toFloat();
    if(hours != 0){

    } else{
        hours = 3;
    }

    cout << "Extended Bolus (40%) over " << int(hours) << " hours: " << extendedBolus << endl;
    float div = extendedBolus / hours;
    thisMachine->consumeInsulin(div);
    extendedCount = hours; // num of hours total, counter will stop at 1 cause 1/hours is already done
    cout << extendedCount - 1 << " hours left... 1 done" << endl;
    stepBolus(div); // make a getter for div and extendedCount when stepBolus is connected to timer
}

void Bolus::stopOngoingBolus()
{
    _insulin->pauseBolus();
    cout << "Pausing bolus delivery..." << endl;
}

void Bolus::cancelBolus() // instead of just pause, get rid of the ongoing bolus
{
    _insulin->stopBolus();
    cout << "Bolus delivery cancelled." << endl;
}
