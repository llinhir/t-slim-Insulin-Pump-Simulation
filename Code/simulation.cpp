#include "simulation.h"

Simulation::Simulation(Ui::MainWindow *ui)
{

    m = new machine(ui);
    IOB = 0;
    this->ui = ui;
    bolus = new Bolus(ui, m);
    insulin = new Insulin(ui, m);
}


bool Simulation::enterPassword(string input)
{
    // checks the users password
    if (m->loginAttempt(input))
    {
        return true;
    }
    else
    {
        return false;
    }
}

