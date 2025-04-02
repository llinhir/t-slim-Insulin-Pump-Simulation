#ifndef SIMULATION_H
#define SIMULATION_H

#include <QCoreApplication>
#include <QTextStream>
#include <iostream>
#include "ui_mainwindow.h"

#include <string>

#include "machine.h"
#include "profile.h"

using namespace std;

class Simulation
{
public:
    Simulation(Ui::MainWindow *ui);
    void startSimulation();
    bool enterPassword(string input);

private:
    machine *m;
    Ui::MainWindow *ui;
};

#endif // SIMULATION_H
