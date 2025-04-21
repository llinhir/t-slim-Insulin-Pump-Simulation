#include "simulation.h"

// Simulation constructor
Simulation::Simulation(Ui::MainWindow *ui)
{

    m = new machine(ui);
    IOB = 0;
    this->ui = ui;
    insulin = new Insulin(ui, m);
    bolus = new Bolus(ui, m, insulin);

    // Simulated daily glucose trend: [mmol/L]
    // Wake up: low, breakfast spike, stabilize, lunch, dinner, night dip
    // GlucoseVector = {
    //     // 41
    //     4.2, 3.7, 3.5,                    // Early hypoglycemia
    //     4.0, 4.3, 5.1,                    // Early morning rising
    //     6.2, 7.4, 8.5, 9.8, 10.6, 11.2,   // Breakfast spike (over 10 mmol/L)
    //     9.3, 7.8, 6.5, 5.2,               // Late morning settling
    //     5.9, 6.8, 7.9, 9.2, 10.1, 10.7,   // Lunch spike
    //     8.3, 6.1, 4.8, 3.9, 3.6, 3.4,     // Afternoon crash/hypo
    //     4.0, 5.3, 7.0, 8.6, 10.5, 11.3,   // Dinner spike (larger one)
    //     9.7, 7.2, 6.0, 5.0, 4.3, 3.8, 3.6 // Evening / overnight dip
    // };

    GlucoseVector = {
        4.2, 3.93, 3.54, 3.38, 3.49, 3.91, 4.12, 4.28, 4.64, 5.17, 5.78, 6.44,
        7.13, 7.75, 8.36, 9.11, 9.81, 10.46, 10.95, 11.44, 11.07, 9.67, 8.6,
        7.85, 7.17, 6.36, 5.49, 5.2, 5.65, 6.23, 6.71, 7.26, 7.94, 8.71, 9.35,
        9.83, 10.63, 10.99, 9.97, 8.38, 6.98, 5.94, 5.18, 4.56, 4.04, 3.65, 3.52,
        3.36, 3.31, 3.56, 4.18, 4.89, 5.79, 6.77, 7.66, 8.57, 9.66, 10.88, 11.48,
        11.32, 10.37, 8.73, 7.34, 6.49, 5.95, 5.37, 4.85, 4.45, 4.13, 3.75, 3.56, 3.5};

    m->setGlucoseVector(&GlucoseVector); // Initialized the glucose

    // creates a timer that updates the time by 5 minutes (in simulation) every 5 seconds (irl)
    m->getCurrentTime(); // Get current and apply time and date
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Simulation::stepMachine);
    timer->start(5000);
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

void Simulation::stepMachine()
{
    m->stepTime();
    m->updateBatteryLevel();
    m->stepInsulinOnBoard();

    // these are here for testing, remove them and uncomment the below area once fully implementing
    m->stepBloodGlucoseVector();
    m->stepBloodGlucose();
    bolus->stepBolus();
    m->stepInsulin();
    m->stepHistoryBox();

    //    // Only step insulin once every 12 calls (i.e., 60 seconds)
    //    hourStepCounter++;
    //    if (hourStepCounter >= 12) {
    //
    //        stepInsulin();
    //        stepBloodGlucose();
    //        hourStepCounter = 0;
    //    }
}
