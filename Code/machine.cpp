#include "machine.h"
#include <algorithm> // for std::find_if
#include <vector>    // for std::vector
#include <string>

machine::machine(Ui::MainWindow *ui)
{
    // PAGE NUMBER:
    currentPage = 0;

    // PASSWORD:
    password = "999";

    // Initialize the UI
    this->ui = ui;

    // Initialize variables and classes
    currentBatteryLevel = 100;
    currentInsulinAmount = 200; // in units, will be out of 300 u
    currentBasalRate = 0;
    previousBasalRate = 0;
    currentIOB = 0;
    isLoggedIn = false;
    isTurnedOn = true;
    isCharging = false;
    hourStepCounter = 0;
    glucoseStepCounter = 0;

    ui->batteryBar->setValue(currentBatteryLevel);
    ui->insulinBar->setValue(currentInsulinAmount);

    //    // creates a timer that updates the time by 5 minutes (in simulation) every 5 seconds (irl)
    //    getCurrentTime(); // Get current and apply time and date
    //    QTimer *timer = new QTimer(this);
    //     connect(timer, &QTimer::timeout, this, &machine::stepMachine);
    //    timer->start(5000);

    // initalize the other classes
    options = new Options(ui);

    // hard coding a test profile as the initial active profile, uses values from the List of Features
    currentProfile = new Profile(0.5, 1, 1, 5, "Bean Senjamin"); // was 0.8, 2, 3, 4
    profiles.push_back(currentProfile);
    setActiveProfile(0);
    updateProfileInfo();

    // function holds all the slot connections, if you create more connects, pls add them to the function
    connectSlots();
}

machine::~machine()
{
    // Destructor
    // cout << "Destructor called" << endl;
}

void machine::connectSlots()
{
    connect(ui->createProfileSaveButton, &QPushButton::clicked, this, [this]()
            { createProfile(); });

    connect(ui->editProfileButton, &QPushButton::clicked, this, [this]()
            { updateProfileInfo(); });

    connect(ui->profile1Button, &QPushButton::clicked, this, [this]()
            { editProfile(0); });
    connect(ui->profile2Button, &QPushButton::clicked, this, [this]()
            { editProfile(1); });
    connect(ui->profile3Button, &QPushButton::clicked, this, [this]()
            { editProfile(2); });
    connect(ui->profile4Button, &QPushButton::clicked, this, [this]()
            { editProfile(3); });
    connect(ui->profile5Button, &QPushButton::clicked, this, [this]()
            { editProfile(4); });

    connect(ui->selectProfile1, &QPushButton::clicked, this, [this]()
            { setActiveProfile(0); });
    connect(ui->selectProfile2, &QPushButton::clicked, this, [this]()
            { setActiveProfile(1); });
    connect(ui->selectProfile3, &QPushButton::clicked, this, [this]()
            { setActiveProfile(2); });
    connect(ui->selectProfile4, &QPushButton::clicked, this, [this]()
            { setActiveProfile(3); });
    connect(ui->selectProfile5, &QPushButton::clicked, this, [this]()
            { setActiveProfile(4); });

    connect(ui->saveProfileButton, &QPushButton::clicked, this, [this]()
            { saveProfile(); });
    connect(ui->deleteProfileButton, &QPushButton::clicked, this, [this]()
            { deleteProfile(); });
}

// Break  down time into usable data for the ui and program
tm *machine::getCurrentTime() // as a note, the current time should not be compute heavy, if it is just stick to updating the time
{
    // Get current time
    time(&rawTime);
    currentTime = localtime(&rawTime);

    currentHour = currentTime->tm_hour;
    currentMinute = currentTime->tm_min;
    currentDay = currentTime->tm_mday;
    currentMonth = currentTime->tm_mon + 1;    // tm_mon is 0-11, so add 1
    currentYear = currentTime->tm_year + 1900; // tm_year is years since 1900, so add 1900

    return currentTime;
}

// Add event to history
void machine::addToHistory(string event)
{
    history.push_back(event);
    cout << "Event added to history: " << event << endl;
}

void machine::addToHistory(QString event)
{
    history.push_back(event.toStdString());
    cout << "Event added to history (QString)" << endl;
}

void machine::addTimetoHistory(string error){

    string minuteStr = (currentMinute < 10) ? "0" + std::to_string(currentMinute) : std::to_string(currentMinute);
    string currentTime = std::to_string(currentHour) + ":" + minuteStr + " " + std::to_string(currentDay) + "/" + std::to_string(currentMonth) + "/" + std::to_string(currentYear);
    string event = error + currentTime; // error example : "Low insulin at "
    addToHistory(event);

}

// User can select a profile; if the selected profile is empty, it does not update current profile
void machine::updateProfileInfo()
{
    cout << "Updating profile info" << endl;

    if (!profiles.empty())
    {
        ui->profile1Button->setText(QString::fromStdString(profiles.at(0)->getProfileName()));
        ui->selectProfile1->setText(QString::fromStdString(profiles.at(0)->getProfileName()));
        ui->basalRate1->setText(returnString(profiles.at(0)));
        if (profiles.size() > 1)
        {
            ui->profile2Button->setText(QString::fromStdString(profiles.at(1)->getProfileName()));
            ui->selectProfile2->setText(QString::fromStdString(profiles.at(1)->getProfileName()));
            ui->basalRate2->setText(returnString(profiles.at(1)));
        }
        else
        {
            ui->profile2Button->setText("No Profile");
            ui->selectProfile2->setText("No Profile");
            ui->basalRate2->setText("No Profile");
        }
        if (profiles.size() > 2)
        {
            ui->profile3Button->setText(QString::fromStdString(profiles.at(2)->getProfileName()));
            ui->selectProfile3->setText(QString::fromStdString(profiles.at(2)->getProfileName()));
            ui->basalRate3->setText(returnString(profiles.at(2)));
        }
        else
        {
            ui->profile3Button->setText("No Profile");
            ui->selectProfile3->setText("No Profile");
            ui->basalRate3->setText("No Profile");
        }
        if (profiles.size() > 3)
        {
            ui->profile4Button->setText(QString::fromStdString(profiles.at(3)->getProfileName()));
            ui->selectProfile4->setText(QString::fromStdString(profiles.at(3)->getProfileName()));
            ui->basalRate4->setText(returnString(profiles.at(3)));
        }
        else
        {
            ui->profile4Button->setText("No Profile");
            ui->selectProfile4->setText("No Profile");
            ui->basalRate4->setText("No Profile");
        }
        if (profiles.size() > 4)
        {
            ui->profile5Button->setText(QString::fromStdString(profiles.at(4)->getProfileName()));
            ui->selectProfile5->setText(QString::fromStdString(profiles.at(4)->getProfileName()));
            ui->basalRate5->setText(returnString(profiles.at(4)));
        }
        else
        {
            ui->profile5Button->setText("No Profile");
            ui->selectProfile5->setText("No Profile");
            ui->basalRate5->setText("No Profile");
        }
    }
    else
    {
        cout << "No profiles available" << endl;
        ui->profile1Button->setText("No Profile");
        ui->selectProfile1->setText("No Profile");
        ui->basalRate1->setText("No Profile");
    }
}

bool machine::loginAttempt(string passwordGuess)
{
    // Check if the password is correct
    if (passwordGuess == password)
    {
        ui->logger->append("Login Successful");
        addToHistory(std::string("Login Successful"));
        isLoggedIn = true;
        return true;
    }
    else
    {
        ui->logger->append("Incorrect Password");
        return false;
    }
}

////////////////////////////////////////////////////
//         Machine functions start here           //
////////////////////////////////////////////////////

void machine::createProfile()
{
    profiles.push_back(options->createProfile());

    // Switch to the edit profile page
    updateProfileInfo();
    ui->stackedWidget->setCurrentIndex(PROFILES_PAGE);
}

void machine::editProfile(int index)
{
    try
    {
        profiles.at(index);
    }
    catch (const std::out_of_range &e)
    {
        return;
    }
    currentProfile = profiles.at(index);
    ui->stackedWidget->setCurrentIndex(EDIT_SPECIFIC_PROFILE_PAGE);
    ui->newBasalRate->setText(QString::number(currentProfile->getBasalRate()));
    ui->newCarbohydrateRatio->setText(QString::number(currentProfile->getCarbohydrateRatio()));
    ui->newCorrectionFactor->setText(QString::number(currentProfile->getCorrectionFactor()));
    ui->newTargetGlucose->setText(QString::number(currentProfile->getTargetGlucoseLevel()));
    ui->newProfileName->setText(QString::fromStdString(currentProfile->getProfileName()));
}

void machine::saveProfile()
{
    currentProfile->setBasalRate(ui->newBasalRate->text().toDouble());
    currentProfile->setCarbohydrateRatio(ui->newCarbohydrateRatio->text().toInt());
    currentProfile->setCorrectionFactor(ui->newCorrectionFactor->text().toInt());
    currentProfile->setTargetGlucoseLevel(ui->newTargetGlucose->text().toInt());
    currentProfile->setProfileName(ui->newProfileName->text().toStdString());
    ui->stackedWidget->setCurrentIndex(EDIT_PROFILE_PAGE);
}

void machine::deleteProfile()
{
    // Ensure currentProfile is not null
    if (currentProfile == nullptr)
        return;

    // Find the profile by name
    auto it = std::find_if(profiles.begin(), profiles.end(), [&](Profile *p)
                           {
                               return p->getProfileName() == currentProfile->getProfileName(); // Dereference p to access Profile
                           });

    // If found, erase it
    if (it != profiles.end())
    {
        profiles.erase(it);
    }

    // Switch to the edit profile page
    updateProfileInfo();
    ui->stackedWidget->setCurrentIndex(EDIT_PROFILE_PAGE);
}

void machine::setActiveProfile(int index)
{
    try
    {
        profiles.at(index);
    }
    catch (const std::out_of_range &e)
    {
        return;
    }
    currentProfile = profiles.at(index);
    QString profileMessage = "Active Profile: " + QString::fromStdString(currentProfile->getProfileName());
    ui->activeProfileLabel->setText(profileMessage);
}

Profile *machine::getProfile(size_t index)
{
    if (index >= profiles.size())
        return nullptr;
    return profiles[index];
}

////////////////////////////////////////////////////
//         Insulin functions start here           //
////////////////////////////////////////////////////

void machine::refillInsulin()
{
    currentInsulinAmount = 300; // in mL, will be out of 300 ml
    ui->insulinBar->setValue(currentInsulinAmount);
    ui->logger->append("Insulin Refilled");
    addToHistory(std::string("Insulin Refilled"));
}

void machine::consumeInsulin(double amount)
{
    if (currentInsulinAmount >= amount)
    {
        currentInsulinAmount -= amount;
    }
    else
    {
        cout << "Not enough insulin available!!!" << endl;
        ui->logger->append("Not enough insulin available!!!");
        addToHistory(std::string("Not enough insulin available!!!"));
        currentInsulinAmount = 0;
    }
    udpateInsulinOnBoard(amount);                   // Add to the insulin on board
    ui->insulinBar->setValue(currentInsulinAmount); // Update the UI
}

void machine::udpateInsulinOnBoard(double amount)
{
    currentIOB += amount;
    if (currentIOB < 0)
    {
        currentIOB = 0;
    }
    ui->IOBunitsText->setText(QString::number(currentIOB, 'f', 2) + " u");
}


////////////////////////////////////////////////////
//          step functions start here            //
////////////////////////////////////////////////////

void machine::stepTime()
{
    // cout << "Stepping time" << endl;
    // cout << "Time before: " << currentHour << ":" << currentMinute << " " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
    //  add 5 minutes to the current time
    currentMinute += 5;
    if (currentMinute >= 60)
    {
        currentMinute -= 60;
        currentHour += 1;
    }
    if (currentHour >= 24)
    {
        currentHour -= 24;
        currentDay += 1;
    }
    currentTime->tm_hour = currentHour;
    currentTime->tm_min = currentMinute;
    currentTime->tm_mday = currentDay;
    currentTime->tm_mon = currentMonth;
    currentTime->tm_year = currentYear;

    cout << "Current time: " << currentHour << ":" << currentMinute << " " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
    qInfo("in function");
    // Update the UI
    tm *timeInfo = getCurrentTimeStruct();

    QString timeString = QString::asprintf("%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min);
    if (timeInfo->tm_hour > 12)
    {
        timeString += " PM";
        timeString.replace(0, 2, QString::number(timeInfo->tm_hour - 12));
    }
    else
    {
        timeString += " AM";
        if (timeInfo->tm_hour == 0)
        {
            timeString.replace(0, 2, "12");
        }
    }
    ui->timeLabel->setText(timeString);

    // adding to history
    string minuteStr = (currentMinute < 10) ? "0" + std::to_string(currentMinute) : std::to_string(currentMinute);
    string currentTime = std::to_string(currentHour) + ":" + minuteStr + " " + std::to_string(currentDay) + "/" + std::to_string(currentMonth) + "/" + std::to_string(currentYear);
    addToHistory(currentTime);
}

// Updates UI based on device status (on/off charging/not charging)
void machine::updateBatteryLevel() // will be called every step
{
    if (!isTurnedOn) // leave early if the machine is off
    {
        return;
    }

    cout << "Updating battery level" << endl;
    if (isCharging)
    {
        currentBatteryLevel += 1;
        if (currentBatteryLevel > 100)
        {
            currentBatteryLevel = 100;
        }
    }
    else
    {
        currentBatteryLevel -= 1;
        if (currentBatteryLevel < 0)
        {
            currentBatteryLevel = 0;
            powerOff();
            ui->simulation->setCurrentIndex(OFF);
        }
        // check if battery is low
        if (currentBatteryLevel <= 20)
        {

            cout << "Battery low" << endl;
            ui->logger->append("Warning: Low Battery");
            addTimetoHistory(std::string("Warning: Low Battery at "));
            addToHistory(std::string("Please recharge the pump by using the provided USB cable.\nLook for the charging indicator to make sure it is correctly plugged in."));
        }
    }

    // ui update
    ui->batteryBar->setValue(currentBatteryLevel);
}

// Updates UI based on how much insulin is stored in the device
void machine::stepInsulin()
{

    // cout << "curr basal rate-------->: " << currentBasalRate << endl;
    currentInsulinAmount = currentInsulinAmount - currentBasalRate;
    if (currentInsulinAmount <= 0)
    {
        currentInsulinAmount = 0;
    }
    if (currentInsulinAmount < 70)
    {
        cout << "Low insulin" << endl;
        ui->logger->append("Warning: Low Insulin");
        addTimetoHistory(std::string("Warning: Low Insulin at "));
        addToHistory(std::string("Please refill the pump with insulin.\nWARNING: Please ensure your body is not connected to the pump during refill as this could cause over delivery of insulin."));

    }

    ui->insulinBar->setValue(currentInsulinAmount);
}

void machine::stepBloodGlucose()
{
    // glucoseStepCounter++;
    // if (glucoseStepCounter >= static_cast<int>(glucoseVector->size()))
    // {
    //     glucoseStepCounter = 0;
    // }

    // Current glucose amount - (the basal rate + extended bolus rate)
    currentGlucose = glucoseVector->at(glucoseStepCounter) - currentBasalRate;

    // adding to history
    QString event = "Blood glucose: " + QString::number(currentGlucose, 'f', 2) + " mmol/L";
    addToHistory(event.toStdString());

    // std::cout << "Blood Glucose: " << currentGlucose << " mmol/L" << std::endl;
    // ui->logger->append(QString::number(currentGlucose, 'f', 1) + " mmol/L");
    // ui->glucoseStatNumber->display(currentGlucose);
    // ui->basalStatNumber->display(currentBasalRate);

    // Basal rate management
    if (currentGlucose <= 3.9 && currentBasalRate != 0)
    {
        // this event needs to be logged for future reference
        std::cout << "[Low Glucose Levels]: basal rate stopped" << std::endl;
        ui->logger->append("[Low Glucose Levels]: basal rate stopped");
        addToHistory(std::string("[Low Glucose Levels]: basal rate stopped\nSeek out nourishment to increase your blood sugar or consider calling for help if you feel unwell."));

        // add to history
        string minuteStr = (currentMinute < 10) ? "0" + std::to_string(currentMinute) : std::to_string(currentMinute);
        string currentTime = std::to_string(currentHour) + ":" + minuteStr + " " + std::to_string(currentDay) + "/" + std::to_string(currentMonth) + "/" + std::to_string(currentYear);
        string event = "Low glucose at " + currentTime;
        event += ", basal rate stopped.";
        addToHistory(event);

        previousBasalRate = currentBasalRate;
        this->setBasalRate(0);
        ui->basalStatNumber->display(0);
        return;
    }
    else if(currentGlucose > 13.9){
        stepErrorGlucose();
    }
    else if (currentGlucose > 3.9 && previousBasalRate != 0 && currentBasalRate == 0)
    {
        std::cout << "prev: " << previousBasalRate << std::endl;
        ui->logger->append("[Medium Glucose Levels]: basal rate resumed");
        addToHistory(std::string("[Medium Glucose Levels]: basal rate resumed"));

        // add to history
        string minuteStr = (currentMinute < 10) ? "0" + std::to_string(currentMinute) : std::to_string(currentMinute);
        string currentTime = std::to_string(currentHour) + ":" + minuteStr + " " + std::to_string(currentDay) + "/" + std::to_string(currentMonth) + "/" + std::to_string(currentYear);
        string event = "Medium glucose at " + currentTime;
        event += ", basal rate resumed";
        addToHistory(event);

        // to make sure I dont kill them right out of the gate
        double futureGlucose = getGlucoseFromVectorInThirtyMins();
        if (futureGlucose < currentGlucose && currentBasalRate != 0)
        {
            // decrease insulin delivery to account for drop in gluces
            currentBasalRate *= 0.75; // decrease by 25%
            cout << "Predicted glucose is lower than current glucose, decreasing basal rate to: " << currentBasalRate << endl;
        }

        this->setBasalRate(previousBasalRate);
        previousBasalRate = 0;
        // currentBasalRate = currentProfile->getBasalRate();
        ui->basalStatNumber->display(currentBasalRate);
        return;
    }
    else if(currentGlucose <= 3.9)
    {
        cout << "[Low Glucose Levels]" << endl;
        ui->logger->append("[Low Glucose Levels]");
        addTimetoHistory(std::string("Warning: Low Glucose at "));
        addToHistory(std::string("Seek out nourishment to increase your blood sugar or consider calling for help if you feel unwell."));
    }


}

void machine::stepErrorGlucose(){
    double correction = std::abs(((currentGlucose - currentProfile->getTargetGlucoseLevel()) / currentProfile->getCorrectionFactor())/2);
    cout << "Predicted blood glucose is too high, correction being applied: " << correction << endl;
    currentGlucose = glucoseVector->at(glucoseStepCounter) - correction;
    cout << "Blood Glucose: " << currentGlucose << " mmol/L" << std::endl;

    ui->logger->append("Warning: High Blood Glucose");
    addTimetoHistory(std::string("Warning: High Blood Glucose at "));
    addToHistory(std::string("Please monitor your blood glucose levels."));

    ui->logger->append("[High Glucose Levels]: correction bolus applied: " + QString::number(correction, 'f', 2) + " units");
    ui->logger->append(QString::number(currentGlucose, 'f', 2) + " mmol/L");
    addToHistory(QString::number(currentGlucose, 'f', 2) + " mmol/L");
    ui->glucoseStatNumber->display(currentGlucose);
}

void machine::stepPredictBasal(){
    // Check 30 mins ahead
    //  First check thirty mins in the future, then adjust the currentBasalrate to match the scenario
    double futureGlucose = getGlucoseFromVectorInThirtyMins();

    if (futureGlucose > 10 && currentBasalRate != 0)
    {
        // calculate for correction
        double correction = std::abs((currentGlucose - currentProfile->getTargetGlucoseLevel()) / currentProfile->getCorrectionFactor());
        cout << "Predicted blood glucose is too high, correction being applied: " << correction << endl;
        currentGlucose = glucoseVector->at(glucoseStepCounter) - correction;
        std::cout << "Blood Glucose: " << currentGlucose << " mmol/L" << std::endl;

        ui->logger->append("[High Glucose Levels]: correction bolus applied: " + QString::number(correction, 'f', 2) + " units");
        ui->logger->append(QString::number(currentGlucose, 'f', 2) + " mmol/L");
        addToHistory(QString::number(currentGlucose, 'f', 2) + " mmol/L");
        ui->glucoseStatNumber->display(currentGlucose);
        return;
    }
    else if (futureGlucose < currentGlucose && currentBasalRate != 0)
    {
        // decrease insulin delivery to account for drop in gluces
        currentBasalRate *= 0.75; // decrease by 25%
        cout << "Predicted glucose is lower than current glucose, decreasing basal rate to: " << currentBasalRate << endl;
        addToHistory("Predicted glucose is lower than current glucose, decreasing basal rate to: " + QString::number(currentBasalRate, 'f', 2) + " units/hour");
    }
    else if (futureGlucose > currentGlucose && currentBasalRate != 0)
    {
        // increase insulin delivery to account for rise in glucose
        currentBasalRate *= 1.1; // increase by 10%
        cout << "Predicted glucose is higher than current glucose, increasing basal rate to: " << currentBasalRate << endl;
        addToHistory("Predicted glucose is higher than current glucose, increasing basal rate to: " + QString::number(currentBasalRate, 'f', 2) + " units/hour");
    }
}

void machine::stepInsulinOnBoard()
{
    // Setting the current thing to -> the body absorbs 25% of IOB per hour
    double decay = currentIOB * 0.25;

    // Prevent negative IOB
    if (currentIOB - decay < 0)
    {
        currentIOB = 0;
    }
    else
    {
        udpateInsulinOnBoard(-decay); // make this negative so it subtracts from IOB
    }
}

void machine::stepHistoryBox()
{
    // Update the history box with the latest events
    ui->historyTextBox->clear();
    for (const auto &event : history)
    {
        ui->historyTextBox->append(QString::fromStdString(event));
    }
}

void machine::stepGraph() {
    glucoseSeries->append(static_cast<qreal>(glucoseStepCounter), static_cast<qreal>(glucoseVector->at(glucoseStepCounter)));

}


void machine::stepBloodGlucoseVector()
{
    glucoseStepCounter++;
    if (glucoseStepCounter >= static_cast<int>(glucoseVector->size()))
    {
        glucoseStepCounter = 0;
    }
}

// etc.

double machine::getGlucoseFromVectorInThirtyMins()
{
    // Get the glucose level from the vector in 30 minutes
    int index = (glucoseStepCounter + 6) % glucoseVector->size();
    return glucoseVector->at(index);
}

QString machine::returnString(Profile *profile)
{
    std::string name = profile->getProfileName();
    double rate = profile->getBasalRate();

    std::ostringstream stream;
    stream << name << "\n"
           << std::fixed << std::setprecision(2) << rate;

    return QString::fromStdString(stream.str());
}
