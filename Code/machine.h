#ifndef MACHINE_H
#define MACHINE_H

#include <ctime>
#include <iostream>
#include <vector>

#include <string>
#include "ui_mainwindow.h"
#include "profile.h"
#include "options.h"

using namespace std;

class machine : public QObject
{
public:
    machine(Ui::MainWindow *ui);
    ~machine();
    tm *getCurrentTime();            // Get current time
    void addToHistory(string event); // Add event to history
    void stepTime();                 // Update time every second
    void stepMachine();              // TODO: Implement machine step function
    void updateBatteryLevel();
    void updateProfileInfo();
    bool loginAttempt(string passwordGuess);

    // Getters
    struct tm *getCurrentTimeStruct() { return currentTime; }
    vector<string> getHistory() { return history; }
    int getCurrentPage() { return currentPage; }
    int getCurrentInsulinAmount() { return currentInsulinAmount; }
    int getCurrentBatteryLevel() { return currentBatteryLevel; }
    bool getIsCharging() { return isCharging; }
    bool getIsTurnedOn() { return isTurnedOn; }
    bool getIsLoggedIn() { return isLoggedIn; }
    Profile *getCurrentProfile() { return currentProfile; }

    // Setters
    void setCurrentBatteryLevel(int level) { currentBatteryLevel = level; }
    void setIsLoggedIn(bool loggedIn) { isLoggedIn = loggedIn; }
    void setIsTurnedOn(bool turnedOn) { isTurnedOn = turnedOn; }
    void setIsCharging(bool charging) { isCharging = charging; }

private:
    Ui::MainWindow *ui;
    time_t rawTime;         // Raw time data
    struct tm *currentTime; // Current time structure
    int currentHour;
    int currentMinute;
    int currentDay;
    int currentMonth;
    int currentYear;

    vector<string> history; // History of events
    int currentPage;
    int currentInsulinAmount; // in mL, will be out of 300 ml
    int currentBatteryLevel;

    bool isCharging;
    bool isTurnedOn;
    bool isLoggedIn;

    string password;            // Password for login
    vector<Profile *> profiles; // List of profiles
    Profile *currentProfile;    // Current profile
    Options *options;           // System Options
};
#endif // MACHINE_H
