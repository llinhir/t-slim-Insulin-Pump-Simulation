#include "machine.h"

machine::machine(Ui::MainWindow *ui)
{
    // PAGE NUMBER:
    currentPage = 0;

    // PASSWORD:
    password = "999";

    // initliaze variables and classes
    currentBatteryLevel = 100;
    currentBolusPrecent = 100;

    isLoggedIn = false;
    isTurnedOn = true;
    isCharging = false;

    ui->batteryBar->setValue(currentBatteryLevel);

    getCurrentTime(); // Get current and apply time and date

    // print for testing
    cout << "Current time: " << currentHour << ":" << currentMinute << " " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
    cout << "Current Date: " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
    options = new Options(ui);

    connect(ui->createProfileSaveButton, &QPushButton::clicked, this, [this]()
            { profiles.push_back(options->createProfile()); });

    connect(ui->editProfileButton, &QPushButton::clicked, this, [this]()
            { updateProfileInfo(); });
}

machine::~machine()
{
    // Destructor
    // cout << "Destructor called" << endl;
}

void machine::getCurrentTime() // as a note, the current time should not be compute heavy, if it is just stick to updating the time
{
    // Get current time
    time(&rawTime);
    currentTime = localtime(&rawTime);

    // set current time
    currentHour = currentTime->tm_hour;
    currentMinute = currentTime->tm_min;
    currentDay = currentTime->tm_mday;
    currentMonth = currentTime->tm_mon + 1;
    currentYear = currentTime->tm_year + 1900;
}

void machine::addToHistory(string event)
{
    // Add event to history
    history.push_back(event);
    cout << "Event added to history: " << event << endl;
}

void machine::updateBatteryLevel() // this will be called every step
{
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
    }
}

void machine::updateProfileInfo(){
    ui->profile1Button->setText(QString::fromStdString(profiles.at(0)->getProfileName()));
}

bool machine::loginAttempt(string passwordGuess)
{
    // Check if the password is correct
    if (passwordGuess == password)
    {
        cout << "Correct Password" << endl;
        isLoggedIn = true;
        return true;
    }
    else
    {
        cout << "Incorrect Password" << endl;
        return false;
    }
}

void machine::stepTime()
{
    // add 5 minutes to the current time
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
}

void machine::stepMachine()
{
    stepTime();
    updateBatteryLevel();
}
