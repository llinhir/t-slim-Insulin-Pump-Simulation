#ifndef SIMULATION_H
#define SIMULATION_H

#include <QCoreApplication>
#include <QTextStream>
#include <iostream>

#include "machine.h"
#include "profile.h"

class Simulation
{
public:
    Simulation();
    void startSimulation();
    bool enterPassword(Profile* p, string input);

private:
    bool isCharging;
    machine* m;
    Profile* profile;
};

#endif // SIMULATION_H
