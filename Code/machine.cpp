#include "machine.h"

machine::machine(Ui::MainWindow *ui)
{
    // PAGE NUMBER:
    currentPage = 0;

    // PASSWORD:
    password = "666";

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

void machine::updateBatteryLevel()
{
    int updatedBatteryLevel = 1;
    currentBatteryLevel = updatedBatteryLevel;
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
