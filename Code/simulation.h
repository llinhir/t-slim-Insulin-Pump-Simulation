#ifndef SIMULATION_H
#define SIMULATION_H

#include <QCoreApplication>
#include <QTextStream>
#include <iostream>
#include "ui_mainwindow.h"

#include <string>

#include "machine.h"
#include "profile.h"
#include "bolus.h"

using namespace std;

class Simulation
{
public:
    Simulation(Ui::MainWindow *ui);
    void startSimulation();
    bool enterPassword(string input);

    machine *getMachine() { return m; }

private:
    machine *m;
    Bolus *bolus;
    Ui::MainWindow *ui;
};

#endif // SIMULATION_H
