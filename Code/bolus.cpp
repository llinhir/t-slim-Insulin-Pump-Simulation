#include "bolus.h"

#include <iostream>
#include <iomanip>
using namespace std;

Bolus::Bolus(Ui::MainWindow *ui, machine *machine, Insulin *insulin)
{
    thisMachine = machine;
    _ui = ui;
    _insulin = insulin;

    currProfile = thisMachine->getCurrentProfile();
    bolusPaused = true;

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

// Gathers user input for bolus calculation
void Bolus::viewCalculation()
{
    if(_ui->editManualBolus->text().toFloat() != 0 || _ui->addGlucoseButton->text().toFloat() == 0){

        cout << "Add values to GLUCOSE (CARBS if needed), select the CGM calculation for use of stored values, OR clear the manual bolus textbox." << endl;

    } else{

    // Update current profile based on selected
    currProfile = thisMachine->getCurrentProfile();

    // Taking input from the user
    carbohydrates = _ui->addCarbsButton->text().toFloat();
    currGlucose = _ui->addGlucoseButton->text().toFloat();
    currIOB = thisMachine->getIOB();

    cout << "Carbs: " << carbohydrates << ", Glucose: " << currGlucose << ", IOB: " << currIOB << endl;
    bolusCalculation(carbohydrates, currGlucose, currIOB);
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

    // Testing
//    cout << foodBolus << endl;
//    cout << correctionBolus << endl;
//    cout << totalBolus << endl;
//    cout << finalBolus << endl;

    cout << "View Calculation: " << fixed << setprecision(2) << finalBolus << endl;
    QString floatString = QString::number(finalBolus, 'f', 2);
    _ui->editManualBolus->setText(floatString);
}

void Bolus::cgmCalculation()
{
    cout << "Getting CGM values..." << endl;
    if(_ui->editManualBolus->text().toFloat() != 0 || _ui->addCarbsButton->text().toFloat() == 0){

        cout << "Please enter carbs and/or revert manual bolus to 0." << endl;

    } else{

        // Update current profile based on selected
        currProfile = thisMachine->getCurrentProfile();
        carbohydrates = _ui->addCarbsButton->text().toFloat();
        currGlucose = thisMachine->getCurrentGlucose();
        currIOB = thisMachine->getIOB();

        cout << "Carbs: " << carbohydrates << ", Glucose: " << currGlucose << ", IOB: " << currIOB << endl;
        bolusCalculation(carbohydrates, currGlucose, currIOB);
    }
}

void Bolus::stepBolus()
{
    if(extendedCount >= 5 && extendedFullAmt > 0){

        thisMachine->consumeInsulin(extendedPortion);
        extendedFullAmt -= extendedPortion;
        extendedCount -= 5;
        _ui->logger->append("Bolus delivery: " + QString::number(extendedPortion, 'f', 2) + " units");

        if(extendedCount >= 5){
            cout << "BOLUS: " << extendedPortion << " units; " << extendedFullAmt << " units left to administer over " << extendedCount << " minutes." << endl;
        } else{
            cout << "BOLUS: " << extendedPortion << " units" << endl;

            cout << "EXTENDED BOLUS COMPLETE" << endl;
        }

    } else{

        // do nothing
    }
}

// IN PROGRESS // + need to add to logger and probably in Insulin add to history vector

void Bolus::manualBolus() // perhaps START causes it to happen + has error handling, while imm and ex just select a setting
{
    float finalBolus = _ui->editManualBolus->text().toFloat();
    _insulin->startBolusDelivery(finalBolus); // give float to function as param
    // take parameter(finalBolus, how many hours) and take the insulin out of device and deliver to user
    // change values accordingly
}

void Bolus::immediateBolus()
{
    float finalBolus = _ui->editManualBolus->text().toFloat(); // or have an in-house var to take from
    immediateFraction = _ui->immediateFractionBox->value();
    cout << immediateFraction << endl;
    double immediateBolus = finalBolus * (immediateFraction / 100.0);

    cout << "Immediate Bolus ("<<  immediateFraction << "%): " << immediateBolus << endl;
    _ui->immediateButton->setStyleSheet("background-color: red;");
    _ui->extendedButton->setStyleSheet("background-color: white;");
    // need a var that chooses

    // mmmmmm maybe only let the start button give insulin?
    thisMachine->consumeInsulin(immediateBolus);
    _ui->logger->append("Bolus delivery: " + QString::number(immediateBolus, 'f', 1) + " units");
}

void Bolus::extendedBolus() // NO MORE CHANGE HOURS :) fix it
{
    float finalBolus = _ui->editManualBolus->text().toFloat();
    immediateFraction = _ui->immediateFractionBox->value();
    float extendedBolus = finalBolus * ((100 - immediateFraction) / 100.0);

    cout << "Extended Bolus (" <<  100 - immediateFraction << "%): " << extendedBolus << endl;
    _ui->extendedButton->setStyleSheet("background-color: red;");
    _ui->immediateButton->setStyleSheet("background-color: white;");
    // need a var that chooses

    // move to start?
    extendedFullAmt = extendedBolus / 3.0; // over 3 hours
    cout << extendedFullAmt << " units per hour over 3 hours" << endl;
    extendedPortion = extendedFullAmt / 36.0; // this much every 5 mins
    extendedCount = 180; // 3 hours = 180 mins = 5mins * 36
    cout << extendedCount << " minutes left of extended bolus" << endl;
    // now stepBolus takes it away :)
}

void Bolus::stopOngoingBolus()
{
    bolusPaused = true;
    // _insulin->pauseBolus();
    cout << "Pausing bolus delivery..." << endl;
    _ui->logger->append("Bolus paused.");
}

void Bolus::cancelBolus() // instead of just pause, get rid of the ongoing bolus
{
    bolusPaused = true;
    extendedCount = 0;
    extendedFullAmt = 0;
    //_insulin->stopBolus();
    cout << "Bolus delivery cancelled." << endl;
    _ui->logger->append("Bolus cancelled.");
}
