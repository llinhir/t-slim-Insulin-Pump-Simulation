#ifndef MACHINE_H
#define MACHINE_H

#include <ctime>
#include <iostream>
#include <vector>
#include <QTimer>
#include <sstream>
#include <iomanip>
#include <QtCharts/QSplineSeries>


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
    void addToHistory(QString event); // QString version because changing it to a string every time was killing me
    void addTimetoHistory(string error); // for ease of use
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
    double getCurrentBasalRate() { return currentBasalRate; }
    bool getIsCharging() { return isCharging; }
    bool getIsTurnedOn() { return isTurnedOn; }
    bool getIsLoggedIn() { return isLoggedIn; }
    bool hasBattery() { return currentBatteryLevel > 0; }
    Profile *getCurrentProfile() { return currentProfile; }
    Profile *getProfile(size_t index); // implemented in .cpp to include out of bound checking
    double getCurrentGlucose() { return currentGlucose; }
    double getIOB() { return currentIOB; }                                                 // for bolus
    int getHourStepCounter() { return hourStepCounter; }                                   // stepBolus temp?
    double getCurrentGlucoseFromVector() { return glucoseVector->at(glucoseStepCounter); } // get the last value from the vector
    double getGlucoseFromVectorInThirtyMins();
//    int getCurrentHour(){ return currentHour; };
//    int getCurrentMinute(){ return currentMinute; };
//    int getCurrentDay(){ return currentDay; };
//    int getCurrentMonth(){ return currentMonth; };
//    int getCurrentYear(){ return currentYear; };

    // Setters
    void setCurrentBatteryLevel(int level) { currentBatteryLevel = level; }
    void setcurrentInsulinAmount(double deviceInsulin){ currentInsulinAmount = deviceInsulin; };
    void setIsLoggedIn(bool loggedIn) { isLoggedIn = loggedIn; }
    void setIsTurnedOn(bool turnedOn) { isTurnedOn = turnedOn; }
    void setIsCharging(bool charging) { isCharging = charging; }
    void setBasalRate(double rate) { currentBasalRate = rate; }
    void setGlucoseVector(vector<float> *vector) { glucoseVector = vector; }
    void setCurrentGlucose(double glucose) { currentGlucose = glucose; }
    void setGlucoseSeries(QtCharts::QSplineSeries *newGlucoseSeries) {glucoseSeries = newGlucoseSeries;}

    // Machine Functinos
    void createProfile();
    void editProfile(int index);
    void saveProfile(); // Helper function for editProfile()
    void deleteProfile();
    void setActiveProfile(int index);

    // Insulin Functions
    void refillInsulin();
    void consumeInsulin(double amount);
    void udpateInsulinOnBoard(double amount);

    // Step functions
    void stepTime(); // Update the simulation every 5 seconds irl
    void updateBatteryLevel();
    void stepInsulin();
    void stepBloodGlucose();
    void stepPredictBasal();
    void stepInsulinOnBoard();
    void stepHistoryBox();
    void stepBloodGlucoseVector();
    void stepGraph();
    void stepErrorGlucose();

    // QoL functions to save space
    QString returnString(Profile *profile);

private:
    Ui::MainWindow *ui;
    time_t rawTime;         // Raw time data
    struct tm *currentTime; // Current time structure
    int currentHour;
    int currentMinute;
    int currentDay;
    int currentMonth;
    int currentYear;

    // Step variables
    int hourStepCounter;    // used to make sure certain variables (ex. insulin & glucose) is updated every 60 minutes (in simulation)
    int glucoseStepCounter; //

    // variables for the machine
    vector<string> history; // History of events
    int currentPage;
    double currentInsulinAmount; // This is the amount of insulin in the machine in u, will be out of 300 u
    int currentBatteryLevel;
    bool isCharging;
    bool isTurnedOn;
    bool isLoggedIn;
    int insulinStepCounter;   // used to make sure insulin is delivered every 60 minutes (in simulation)
    double previousBasalRate; // saves the previous basal rate for when glucose levels go below zero

    string password;  // Password for login
    Options *options; // System Options

    // User info
    vector<Profile *> profiles;   // The user can have multiple profiles
    Profile *currentProfile;      // Current profile
    double currentBasalRate;      // The basal rate that the system drains from the Insulin gauge
    vector<float> *glucoseVector; // Gets the from simulation
    QtCharts::QSplineSeries *glucoseSeries;
    double currentGlucose;        // Current Blood Glucose of User, in mmol/L
    double currentIOB;            // Insulin on board. The amount of insulin still inside the body after a bolus injection

private slots:
    // void stepMachine(); // TODO: Implement machine step function
};
#endif // MACHINE_H
