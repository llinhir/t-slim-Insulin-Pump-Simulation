#include "machine.h"

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
    currentBolusPrecent = 100;

    isLoggedIn = false;
    isTurnedOn = true;
    isCharging = false;

    ui->batteryBar->setValue(currentBatteryLevel);

    getCurrentTime(); // Get current and apply time and date

    // print for testing
    // cout << "Current time: " << currentHour << ":" << currentMinute << " " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
    // cout << "Current Date: " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
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
    }
    else
    {
        cout << "No profiles available" << endl;
        ui->profile1Button->setText("No Profile");
    }
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

    // cout << "Current time: " << currentHour << ":" << currentMinute << " " << currentDay << "/" << currentMonth << "/" << currentYear << endl;

    // Update the UI
    tm *timeInfo = getCurrentTimeStruct();

    ui->dateTimeEdit->setDateTime(QDateTime::fromSecsSinceEpoch(mktime(timeInfo)));
}

void machine::stepMachine()
{
    stepTime();
    updateBatteryLevel();
}
