#ifndef SIMULATION_H
#define SIMULATION_H

#include <QCoreApplication>
#include <QTextStream>
#include <iostream>
#include "ui_mainwindow.h"

#include <string>
#include <vector>
#include <QTimer>

#include "machine.h"
#include "profile.h"
#include "bolus.h"
#include "insulin.h"

using namespace std;

class Simulation : public QObject // if this is breaking stuff, can remove from qobject
{
public:
    Simulation(Ui::MainWindow *ui);
    void startSimulation();
    bool enterPassword(string input);

    machine *getMachine() { return m; }

private:
    machine *m;
    Bolus *bolus;
    int IOB; // Insulin on board
    Ui::MainWindow *ui;
    Insulin *insulin;
    vector<float> GlucoseVector; // This is for the glucose value at each time step, almost like the readings that a machine would get

private slots:
    void stepMachine();
};

#endif // SIMULATION_H
