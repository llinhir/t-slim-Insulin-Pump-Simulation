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

    // initliaze variables and classes
    currentBatteryLevel = 100;
    currentInsulinAmount = 300; // in mL, will be out of 300 ml

    isLoggedIn = false;
    isTurnedOn = true;
    isCharging = false;

    ui->batteryBar->setValue(currentBatteryLevel);
    ui->insulinBar->setValue(currentInsulinAmount);

    getCurrentTime(); // Get current and apply time and date

    // print for testing
    cout << "Current time: " << currentHour << ":" << currentMinute << " " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
    cout << "Current Date: " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
    options = new Options(ui);

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

machine::~machine()
{
    // Destructor
    // cout << "Destructor called" << endl;
}

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

void machine::addToHistory(string event)
{
    // Add event to history
    history.push_back(event);
    cout << "Event added to history: " << event << endl;
}

void machine::updateBatteryLevel() // this will be called every step, update ui
{
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
        }
        // check if battery is low
        if (currentBatteryLevel == 20)
        {
            ui->logger->append("Battery Low: Please Charge");
            cout << "Battery low" << endl; // add this to history and ui
        }
    }

    // ui update
    ui->batteryBar->setValue(currentBatteryLevel);
}

void machine::updateProfileInfo()
{
    cout << "Updating profile info" << endl;

    if (!profiles.empty())
    {
        ui->profile1Button->setText(QString::fromStdString(profiles.at(0)->getProfileName()));
        ui->selectProfile1->setText(QString::fromStdString(profiles.at(0)->getProfileName()));
        if (profiles.size() > 1)
        {
            ui->profile2Button->setText(QString::fromStdString(profiles.at(1)->getProfileName()));
            ui->selectProfile2->setText(QString::fromStdString(profiles.at(1)->getProfileName()));
        }
        else
        {
            ui->profile2Button->setText("No Profile");
            ui->selectProfile2->setText("No Profile");
        }
        if (profiles.size() > 2)
        {
            ui->profile3Button->setText(QString::fromStdString(profiles.at(2)->getProfileName()));
            ui->selectProfile3->setText(QString::fromStdString(profiles.at(2)->getProfileName()));
        }
        else
        {
            ui->profile3Button->setText("No Profile");
            ui->selectProfile3->setText("No Profile");
        }
        if (profiles.size() > 3)
        {
            ui->profile4Button->setText(QString::fromStdString(profiles.at(3)->getProfileName()));
            ui->selectProfile4->setText(QString::fromStdString(profiles.at(3)->getProfileName()));
        }
        else
        {
            ui->profile4Button->setText("No Profile");
            ui->selectProfile4->setText("No Profile");
        }
        if (profiles.size() > 4)
        {
            ui->profile5Button->setText(QString::fromStdString(profiles.at(4)->getProfileName()));
            ui->selectProfile5->setText(QString::fromStdString(profiles.at(4)->getProfileName()));
        }
        else
        {
            ui->profile5Button->setText("No Profile");
            ui->selectProfile5->setText("No Profile");
        }
    }
    else
    {
        cout << "No profiles available" << endl;
        ui->profile1Button->setText("No Profile");
        ui->selectProfile1->setText("No Profile");
    }
}

bool machine::loginAttempt(string passwordGuess)
{
    // Check if the password is correct
    if (passwordGuess == password)
    {
        ui->logger->append("Login Successful");
        isLoggedIn = true;
        return true;
    }
    else
    {
        ui->logger->append("Incorrect Password");
        return false;
    }
}

void machine::stepTime() // this is wrong
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
}

void machine::stepMachine()
{
    stepTime();
    updateBatteryLevel();
}

void machine::createProfile()
{
    profiles.push_back(options->createProfile());

    // Switch to the edit profile page
    updateProfileInfo();
    ui->stackedWidget->setCurrentIndex(PROFILES_PAGE);
}

void machine::editProfile(int index)
{
    try{
        profiles.at(index);
    }
    catch(const std::out_of_range& e) {
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
    currentProfile->setBasalRate(ui->newBasalRate->text().toInt());
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
    try{
        profiles.at(index);
    }
    catch(const std::out_of_range& e) {
        return;
    }
    currentProfile = profiles.at(index);
    QString profileMessage = "Active Profile: " + QString::fromStdString(currentProfile->getProfileName());
    ui->activeProfileLabel->setText(profileMessage);
}
