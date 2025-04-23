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
    bolusOption = 0;

    connect(ui->viewCalculationButton, &QPushButton::clicked, this, [this]()
            { viewCalculation(); });
    connect(ui->viewCalculationButton_2, &QPushButton::clicked, this, [this]()
            { cgmCalculation(); });
    connect(ui->selectProfileButton, &QPushButton::clicked, this, [this]()
            { _ui->stackedWidget->setCurrentIndex(SELECT_ACTIVE_PROFILE_PAGE); });

    connect(ui->startManualBolus, &QPushButton::clicked, this, [this]()
            { startBolus(); });
    connect(ui->immediateButton, &QPushButton::clicked, this, [this]()
            { immediateBolus(); });
    connect(ui->extendedButton, &QPushButton::clicked, this, [this]()
            { extendedBolus(); });

    connect(ui->pauseBolusButton, &QPushButton::clicked, this, [this]()
            { stopOngoingBolus(); });
    connect(ui->continueBolusButton, &QPushButton::clicked, this, [this]()
            { continueBolus(); });
    connect(ui->cancelBolusButton, &QPushButton::clicked, this, [this]()
            { cancelBolus(); });
}

Bolus::~Bolus()
{
}

// Gathers user input for bolus calculation
void Bolus::viewCalculation()
{
    if (_ui->addGlucoseButton->text().toFloat() == 0)
    { // _ui->editManualBolus->text().toFloat() != 0 ||

        cout << "Add values to GLUCOSE (CARBS if needed) OR select the CGM calculation for use of stored values." << endl;
    }
    else
    {

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

    if (finalBolus > 0)
    {

        cout << "View Calculation: " << fixed << setprecision(2) << finalBolus << endl;
        QString floatString = QString::number(finalBolus, 'f', 2);
        _ui->editManualBolus->setText(floatString);
    }
    else
    {
        cout << "The calculation was negative. Submit carbs and/or glucose. Either your insulin levels are too high for a delivery or your glucose is too low / estimated to become too low." << endl;
    }
}

void Bolus::cgmCalculation()
{
    cout << "Getting CGM values..." << endl;
    //    if(_ui->addCarbsButton->text().toFloat() == 0){ // _ui->editManualBolus->text().toFloat() != 0 ||

    //        cout << "Please enter carbs." << endl;

    //    } else{

    // Update current profile based on selected
    currProfile = thisMachine->getCurrentProfile();
    carbohydrates = _ui->addCarbsButton->text().toFloat();
    currGlucose = thisMachine->getCurrentGlucose();
    currIOB = thisMachine->getIOB();
    _ui->addGlucoseButton->setText(QString::number(currGlucose, 'f', 2));

    cout << "Carbs: " << carbohydrates << ", Glucose: " << currGlucose << ", IOB: " << currIOB << endl;
    bolusCalculation(carbohydrates, currGlucose, currIOB);
    //}
}

void Bolus::stepBolus()
{
    // if (thisMachine->getCurrentGlucoseFromVector() <= 3.9) // if glucose is too low, exit
    // {
    //     cout << "Glucose is too low. Stop bolus delivery." << endl;
    //     struct tm *timeInfo = thisMachine->getCurrentTimeStruct();
    //     string minuteStr = (timeInfo->tm_min < 10) ? "0" + std::to_string(timeInfo->tm_min) : std::to_string(timeInfo->tm_min);
    //     string currentTime = std::to_string(timeInfo->tm_hour) + ":" + minuteStr + " " + std::to_string(timeInfo->tm_mday) + "/" + std::to_string(timeInfo->tm_mon + 1) + "/" + std::to_string(timeInfo->tm_year + 1900);
    //     string event = "Low glucose at " + currentTime;
    //     event += ", Extended bolus delivery has been paused";
    //     thisMachine->addToHistory(event);
    //     _ui->logger->append("[Low glucose levels]: Extended bolus delivery has been paused");
    //     stopOngoingBolus();
    //     // cancelBolus();
    //     return;
    // }

    if (extendedCount >= 5 && extendedFullAmt > 0)
    {

        thisMachine->consumeInsulin(extendedPortion);

        // this is the glucose math
        double newGlucose = thisMachine->getCurrentGlucose() - extendedPortion;
        thisMachine->setCurrentGlucose(newGlucose);

        extendedFullAmt -= extendedPortion;
        extendedCount -= 5;
        _ui->logger->append("Bolus delivery: " + QString::number(extendedPortion, 'f', 2) + " u");

        if (extendedCount >= 5)
        {
            cout << "BOLUS: " << extendedPortion << " units; " << extendedFullAmt << " units left to administer over " << extendedCount << " minutes." << endl;
            _ui->IOBtimeText->setText(QString::number(extendedCount) + " min");
            _ui->IOBunitsText->setText(QString::number(extendedFullAmt, 'f', 2) + " u");
        }
        else
        {
            cout << "BOLUS: " << extendedPortion << " units" << endl;

            cout << "EXTENDED BOLUS COMPLETE" << endl;
        }
    }
    else
    {

        // do nothing
    }

    double currentGlucose = thisMachine->getCurrentGlucose();
    double currentBasalRate = thisMachine->getCurrentBasalRate();
    std::cout << "Blood Glucose: " << currentGlucose << " mmol/L" << std::endl;
    _ui->logger->append(QString::number(currentGlucose, 'f', 1) + " mmol/L");
    _ui->glucoseStatNumber->display(currentGlucose);
    _ui->basalStatNumber->display(currentBasalRate);
}

void Bolus::startBolus() // delivers bolus (imm and ex just select a setting)
{
    if (bolusOption == 1)
    { // immediate

        thisMachine->consumeInsulin(immediateAmt);

        _ui->logger->append("Bolus delivery start: " + QString::number(immediateAmt, 'f', 2) + " u");
        thisMachine->addToHistory("Bolus delivery start: " + QString::number(immediateAmt, 'f', 2) + " u");

        double newGlucose = thisMachine->getCurrentGlucose() - immediateAmt;
        thisMachine->setCurrentGlucose(newGlucose);

        double currentGlucose = thisMachine->getCurrentGlucose();
        double currentBasalRate = thisMachine->getCurrentBasalRate();
        std::cout << "Blood Glucose: " << currentGlucose << " mmol/L" << std::endl;
        _ui->logger->append(QString::number(currentGlucose, 'f', 2) + " mmol/L");
        _ui->glucoseStatNumber->display(currentGlucose);
        _ui->basalStatNumber->display(currentBasalRate);
    }
    else if (bolusOption == 2)
    { // extended

        bolusPaused = false;
        cout << extendedFullAmt << " units over 3 hours" << endl;
        extendedPortion = extendedFullAmt / 36.0; // this much every 5 mins
        extendedCount = 180;                      // 3 hours = 180 mins = 5mins * 36
        cout << extendedCount << " minutes left of extended bolus" << endl;
        _ui->bolusStatNumber->display(QString::number(extendedFullAmt / 3, 'f', 2)); // units per hour
        // now stepBolus takes it away :)
    }
    else
    {
        // nothing
    }

    //    _ui->immediateButton->setStyleSheet("background-color: white;");
    //    _ui->extendedButton->setStyleSheet("background-color: white;");
    // bolusOption = 0;
}

void Bolus::immediateBolus()
{

    // if (thisMachine->getCurrentGlucoseFromVector() <= 3.9) // if glucose is too low, dont allow this
    // {
    //     cout << "Glucose is too low. Immediate bolus delivery has been denied." << endl;
    //     struct tm *timeInfo = thisMachine->getCurrentTimeStruct();
    //     string minuteStr = (timeInfo->tm_min < 10) ? "0" + std::to_string(timeInfo->tm_min) : std::to_string(timeInfo->tm_min);
    //     string currentTime = std::to_string(timeInfo->tm_hour) + ":" + minuteStr + " " + std::to_string(timeInfo->tm_mday) + "/" + std::to_string(timeInfo->tm_mon + 1) + "/" + std::to_string(timeInfo->tm_year + 1900);
    //     string event = "Low glucose at " + currentTime;
    //     event += ", Immediate bolus delivery has been denied at " + currentTime;
    //     thisMachine->addToHistory(event);
    //     _ui->logger->append("[Low glucose levels]: Immediate bolus delivery has been denied");
    //     return;
    // }

    float finalBolus = _ui->editManualBolus->text().toFloat();
    immediateFraction = _ui->immediateFractionBox->value();
    cout << immediateFraction << endl;
    immediateAmt = finalBolus * (immediateFraction / 100.0);

    bolusOption = 1;
    cout << "Immediate Bolus (" << immediateFraction << "%): " << immediateAmt << endl;
    _ui->immediateButton->setStyleSheet("background-color: red;");
    _ui->extendedButton->setStyleSheet("background-color: white;");
}

void Bolus::extendedBolus()
{
    float finalBolus = _ui->editManualBolus->text().toFloat();
    immediateFraction = _ui->immediateFractionBox->value();
    float extendedBolus = finalBolus * ((100 - immediateFraction) / 100.0);
    extendedFullAmt = extendedBolus / 3.0; // over 3 hours

    bolusOption = 2;
    cout << "Extended Bolus (" << 100 - immediateFraction << "%): " << extendedBolus << endl;
    _ui->extendedButton->setStyleSheet("background-color: red;");
    _ui->immediateButton->setStyleSheet("background-color: white;");
}

void Bolus::stopOngoingBolus()
{
    bolusPaused = true;
    bPausedCount = extendedCount;
    bPausedAmt = extendedFullAmt;
    extendedCount = 0;
    extendedFullAmt = 0;

    // _insulin->pauseBolus();
    cout << "Pausing bolus delivery..." << endl;
    _ui->logger->append("Bolus paused.");
    _ui->IOBtimeText->setText("paused");
    _ui->IOBunitsText->setText("paused");
    thisMachine->addToHistory(std::string("Bolus paused."));
    _ui->bolusStatNumber->display(0); // units per hour
}

void Bolus::continueBolus()
{

    if (bolusPaused == true)
    {

        bolusPaused = false;
        extendedCount = bPausedCount;
        extendedFullAmt = bPausedAmt;
        bPausedCount = 0;
        bPausedAmt = 0;

        //_insulin->stopBolus();

        cout << "Resuming bolus delivery..." << endl;
        _ui->logger->append("Bolus resumed.");
        _ui->IOBunitsText->setText(QString::number(extendedFullAmt, 'f', 2) + " u");
        _ui->IOBtimeText->setText(QString::number(extendedCount, 'f', 2) + " min");
        _ui->bolusStatNumber->display(QString::number(extendedPortion, 'f', 2)); // units per hour
        thisMachine->addToHistory(std::string("Bolus resumed."));
    }
    else
    {
        // nothing
    }
}

// instead of just pause, get rid of the ongoing bolus
void Bolus::cancelBolus()
{
    bolusPaused = true;
    extendedCount = 0;
    extendedFullAmt = 0;
    extendedPortion = 0;
    bPausedCount = 0;
    bPausedAmt = 0;

    //_insulin->stopBolus();

    cout << "Bolus delivery cancelled." << endl;
    _ui->logger->append("Bolus cancelled.");
    _ui->IOBunitsText->setText("0.00 u");
    _ui->IOBtimeText->setText("0.00 min");
    thisMachine->addToHistory(std::string("Bolus cancelled."));
    _ui->bolusStatNumber->display(0); // units per hour
}
