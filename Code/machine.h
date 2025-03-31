#ifndef MACHINE_H
#define MACHINE_H

#include <ctime>
#include <iostream>
#include <vector>

#include <string>

using namespace std;

class machine
{
public:
    machine();
    ~machine();
    void getCurrentTime();           // Get current time
    void addToHistory(string event); // Add event to history
    void stepMachine();              // TODO: Implement machine step function

    // Getters
    struct tm *getCurrentTimeStruct() { return currentTime; }
    vector<string> getHistory() { return history; }
    int getCurrentPage() { return currentPage; }
    int getCurrentBolusPrecent() { return currentBolusPrecent; }
    int getCurrentBatteryLevel() { return currentBatteryLevel; }

private:
    time_t rawTime;         // Raw time data
    struct tm *currentTime; // Current time structure
    int currentHour;        // Current hour
    int currentMinute;      // Current minute
    int currentDay;         // Current day
    int currentMonth;       // Current month
    int currentYear;        // Current year

    vector<string> history;  // History of events
    int currentPage;         // Current page number
    int currentBolusPrecent; // Current bolus percentage
    int currentBatteryLevel; // Current battery level
};

#endif // MACHINE_H
