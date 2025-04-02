#ifndef MACHINE_H
#define MACHINE_H

#include <ctime>
#include <iostream>
#include <vector>

#include <string>
#include "ui_mainwindow.h"
#include "profile.h"

using namespace std;

class machine
{
public:
    // machine(Ui::MainWindow *ui); // test
    machine(Ui::MainWindow *ui); // this is temp
    ~machine();
    void getCurrentTime();           // Get current time
    void addToHistory(string event); // Add event to history
    void stepMachine();              // TODO: Implement machine step function
    void updateBatteryLevel();
    bool loginAttempt(string passwordGuess);

    // Getters
    struct tm *getCurrentTimeStruct() { return currentTime; }
    vector<string> getHistory() { return history; }
    int getCurrentPage() { return currentPage; }
    int getCurrentBolusPrecent() { return currentBolusPrecent; }
    int getCurrentBatteryLevel() { return currentBatteryLevel; }

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
    int currentBolusPrecent;
    int currentBatteryLevel;

    bool isCharging;
    bool isTurnedOn;
    bool isLoggedIn;

    string password;            // Password for login
    vector<Profile *> profiles; // List of profiles
    Profile *currentProfile;    // Current profile
};
#endif // MACHINE_H
