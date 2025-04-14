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
    // Update current profile based on selected -> need a popup for current profile if none is selected
    currProfile = thisMachine->getCurrentProfile();

    // Taking input from the user
    carbohydrates = _ui->addCarbsButton->text().toFloat();
    currGlucose = _ui->addGlucoseButton->text().toFloat();
    cout << "Carbs: " << carbohydrates << "\nGlucose: " << currGlucose << endl;
    int units = 5; // CHANGE WHEN INSULIN ON BOARD IS IMPLEMENTED!!!!!!!!!!!!! HARDCODED
    bolusCalculation(carbohydrates, currGlucose, units);
}

// Calculates final bolus based on user input
void Bolus::bolusCalculation(int carbs, float glucose, int insulinOnBoard)
{
    cout << "Starting calculation..." << endl;
    float foodBolus = carbs / currProfile->getCarbohydrateRatio();
    float correctionBolus = (glucose - currProfile->getTargetGlucoseLevel()) / currProfile->getCorrectionFactor();

    float totalBolus = foodBolus + correctionBolus;
    currBolus = totalBolus - insulinOnBoard; // currBolus vs finalBolus, will fix

    cout << "View Calculation: " << fixed << setprecision(2) << currBolus << endl;
    QString floatString = QString::number(currBolus, 'f', 2);
    _ui->editManualBolus->setText(floatString);
}

void Bolus::cgmCalculation()
{
    // pls make the cgm stuff so I can take it :)
}

void Bolus::stepBolus()
{
}

// IN PROGRESS // + need to add to logger and probably in Insulin add to history vector

void Bolus::manualBolus()
{
    float finalBolus = _ui->editManualBolus->text().toFloat();
    _insulin->startInsulinDelivery(finalBolus); // give float to function as param
    // take parameter(finalBolus, how many hours) and take the insulin out of device and deliver to user
    // change values accordingly
}

void Bolus::immediateBolus()
{ // FIGURE OUT order of stuff!! and give IOB to someone
    float finalBolus = _ui->editManualBolus->text().toFloat(); // or have an in-house var to take from
    float immediateBolus = finalBolus * 0.6;
    cout << "Immediate Bolus (60%): " << immediateBolus << endl;
    manualBolus();
}

void Bolus::extendedBolus()
{
    float finalBolus = _ui->editManualBolus->text().toFloat();
    float extendedBolus = finalBolus * 0.4;
    cout << "Extended Bolus (40%) over 3 hours: " << extendedBolus << endl;
}

void Bolus::stopOngoingBolus()
{
    _insulin->pauseInsulin();
    cout << "Pausing bolus delivery..." << endl;
}

void Bolus::cancelBolus() // instead of just pause, get rid of the ongoing bolus
{
    _insulin->manualStopInsulin();
    cout << "Bolus delivery cancelled." << endl;
}
