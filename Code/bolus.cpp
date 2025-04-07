#include "bolus.h"

#include <iostream>
#include <iomanip>
using namespace std;

Bolus::Bolus(Ui::MainWindow *ui, machine *machine)
{
    thisMachine = machine;
    _ui = ui;
    currProfile = thisMachine->getCurrentProfile();
    connect(ui->viewCalculationButton, &QPushButton::clicked, this, [this]()
            { viewCalculation(); });
    connect(ui->viewCalculationButton_2, &QPushButton::clicked, this, [this]()
            { cgmCalculation(); });
}

Bolus::~Bolus()
{
}

void Bolus::viewCalculation()
{
    cout << "Calculating bolus" << endl;
    carbohydrates = _ui->addCarbsButton->text().toFloat();
    currGlucose = _ui->addGlucoseButton->text().toFloat();
    cout << "Carbs: " << carbohydrates << "\nGlucose: " << currGlucose << endl;
    int units = 0; // CHANGE WHEN INSULIN ON BOARD IS IMPLEMENTED!!!!!!!!!!!!!
    bolusCalculation(carbohydrates, currGlucose, units);
}

// INSULIN ON BOARD FROM THE MACHINE, not yet implemented
void Bolus::bolusCalculation(int carbs, float glucose, int insulinOnBoard)
{ // get these from ui vs CGM
    cout << "Starting calculation..." << endl;
    float foodBolus = carbs / currProfile->getCarbohydrateRatio();
    float correctionBolus = (glucose - currProfile->getTargetGlucoseLevel()) / currProfile->getCorrectionFactor();

    cout << "Continuing calculation..." << endl; // breaks before this
    float totalBolus = foodBolus + correctionBolus;
    float finalBolus = totalBolus - insulinOnBoard;

    cout << "View Calculation: " << fixed << setprecision(5) << finalBolus << endl;
    QString floatString = QString::number(finalBolus, 'f', 5);
    _ui->editManualBolus->setText(floatString); // it is not changing :(
}

void Bolus::cgmCalculation()
{
    // pls make the cgm stuff so I can take it :)
}

void Bolus::stepBolus()
{
}

// make sure it can only be numbers
