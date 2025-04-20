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
    GlucoseVector = {
        // 41
        4.2, 3.7, 3.5,                    // Early hypoglycemia
        4.0, 4.3, 5.1,                    // Early morning rising
        6.2, 7.4, 8.5, 9.8, 10.6, 11.2,   // Breakfast spike (over 10 mmol/L)
        9.3, 7.8, 6.5, 5.2,               // Late morning settling
        5.9, 6.8, 7.9, 9.2, 10.1, 10.7,   // Lunch spike
        8.3, 6.1, 4.8, 3.9, 3.6, 3.4,     // Afternoon crash/hypo
        4.0, 5.3, 7.0, 8.6, 10.5, 11.3,   // Dinner spike (larger one)
        9.7, 7.2, 6.0, 5.0, 4.3, 3.8, 3.6 // Evening / overnight dip
    };
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
