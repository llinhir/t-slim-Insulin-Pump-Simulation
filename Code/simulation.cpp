#include "simulation.h"

// Simulation constructor
Simulation::Simulation(Ui::MainWindow *ui)
{

    m = new machine(ui);
    IOB = 0;
    this->ui = ui;
    bolus = new Bolus(ui, m, insulin);
    insulin = new Insulin(ui, m);

    // Simulated daily glucose trend: [mmol/L]
    // Wake up: low, breakfast spike, stabilize, lunch, dinner, night dip
    GlucoseVector = {
        4.2,                      // early morning
        5.1,                      // just after waking up
        6.8, 7.5, 8.3, 9.7, 10.4, // post-breakfast
        6.9, 5.5,                 // late morning
        7.1, 8.2, 9.1,            // lunch spike
        6.3, 5.2,                 // afternoon
        7.6, 8.8, 10.1,           // dinner spike
        6.4, 5.1, 4.3, 3.8,       // late evening to night dip
    };
}

// Checks if user entered the correct password
bool Simulation::enterPassword(string input)
{

    if (m->loginAttempt(input))
    {
        return true;
    }
    else
    {
        return false;
    }
}
