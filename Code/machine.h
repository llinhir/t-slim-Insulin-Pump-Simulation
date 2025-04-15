#ifndef MACHINE_H
#define MACHINE_H

#include <ctime>
#include <iostream>
#include <vector>
#include <QTimer>
#include <sstream>
#include <iomanip>

#include <string>
#include "defs.h"
#include "ui_mainwindow.h"
#include "profile.h"
#include "options.h"


using namespace std;

class machine : public QObject
{
public:
    machine(Ui::MainWindow *ui);
    ~machine();
    void connectSlots();             // Function holding all the slot connects to declutter the machine constructor
    tm *getCurrentTime();            // Get current time
    void addToHistory(string event); // Add event to history
    void stepTime();                 // Update time every second
    void updateBatteryLevel();
    void updateProfileInfo();
    bool loginAttempt(string passwordGuess);
    void powerOff() { isTurnedOn = false; };
    void powerOn() { isTurnedOn = true; };

    // Getters
    struct tm *getCurrentTimeStruct() { return currentTime; }
    vector<string> getHistory() { return history; }
    int getCurrentPage() { return currentPage; }
    double getCurrentInsulinAmount() { return currentInsulinAmount; }
    int getCurrentBatteryLevel() { return currentBatteryLevel; }
    bool getIsCharging() { return isCharging; }
    bool getIsTurnedOn() { return isTurnedOn; }
    bool getIsLoggedIn() { return isLoggedIn; }
    bool hasBattery() { return currentBatteryLevel > 0; };
    Profile *getCurrentProfile() { return currentProfile; }
    Profile *getProfile(size_t index); // implemented in .cpp to include out of bound checking

    // Setters
    void setCurrentBatteryLevel(int level) { currentBatteryLevel = level; }
    void setIsLoggedIn(bool loggedIn) { isLoggedIn = loggedIn; }
    void setIsTurnedOn(bool turnedOn) { isTurnedOn = turnedOn; }
    void setIsCharging(bool charging) { isCharging = charging; }
    void setBasalRate(double rate) { currentBasalRate = rate; }

    // Machine Functinos
    void createProfile();
    void editProfile(int index);
    void saveProfile(); // Helper function for editProfile()
    void deleteProfile();
    void setActiveProfile(int index);
    void refillInsulin();
    void stepInsulin();

    // QoL functions to save space
    QString returnString(Profile* profile);

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
    double currentInsulinAmount; // This is the amount of insulin in the machine in u, will be out of 300 u
    int currentBatteryLevel;
    int userGlucoseLevel;       // Current Blood Glucose of User, in mmol/L
    int userInsulinOnBoard;     // The amount of insulin still inside the body after a bolus injection

    bool isCharging;
    bool isTurnedOn;
    bool isLoggedIn;
    int insulinStepCounter;     // used to make sure insulin is delivered every 60 minutes (in simulation)

    string password;            // Password for login
    vector<Profile *> profiles; // List of profiles
    Profile *currentProfile;    // Current profile
    Options *options;           // System Options
    double currentBasalRate;       // The basal rate that the system drains from the Insulin gauge


private slots:
    void stepMachine(); // TODO: Implement machine step function
};
#endif // MACHINE_H
