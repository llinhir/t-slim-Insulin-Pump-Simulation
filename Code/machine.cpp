#include "machine.h"

machine::machine()
{
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