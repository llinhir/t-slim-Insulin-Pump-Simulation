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
    timer->start(1000);

    connect(ui->CGMdisconnectButton, &QPushButton::clicked, this, [this]()
            { adminErrors(1); });
    connect(ui->lowBatteryButton, &QPushButton::clicked, this, [this]()
            { adminErrors(2); });
    connect(ui->lowInsulinButton, &QPushButton::clicked, this, [this]()
            { adminErrors(3); });
    connect(ui->occlusionButton, &QPushButton::clicked, this, [this]()
            { adminErrors(4); });
    connect(ui->criticalFailureButton, &QPushButton::clicked, this, [this]()
            { adminErrors(5); });
    connect(ui->pumpShutdownButton, &QPushButton::clicked, this, [this]()
            { adminErrors(6); });
    connect(ui->lowBGButton, &QPushButton::clicked, this, [this]()
            { adminErrors(7); });
    connect(ui->highBGButton, &QPushButton::clicked, this, [this]()
            { adminErrors(8); });
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
    m->stepPredictBasal();
    m->stepBloodGlucose();
    bolus->stepBolus();
    m->stepInsulin();
    m->stepHistoryBox();
    m->stepGraph();
    m->addToHistory(std::string("\n"));

    //    // Only step insulin once every 12 calls (i.e., 60 seconds)
    //    hourStepCounter++;
    //    if (hourStepCounter >= 12) {
    //
    //        stepInsulin();
    //        stepBloodGlucose();
    //        hourStepCounter = 0;
    //    }
}

// Admin-controlled errors and error-handling
void Simulation::adminErrors(int option){ // add to everywhere

    switch(option) {
        case 1: // CGM disconnection
            ui->logger->append("Warning: CGM Sensor Not Detected - see MY CGM");
            m->addTimetoHistory(std::string("Warning: CGM Sensor Not Detected at "));
            m->addToHistory(std::string("Please check the connection between the CGM transmitter and pump, and ensure the sensor is properly in place.\nIf this does not work, refer to your user manual for restarting or recalibrating your CGM."));
            break;
        case 2: // low battery
            m->setCurrentBatteryLevel(15);
            // machine handles this with updateBatteryLevel()
            break;
        case 3: // low insulin
            m->setcurrentInsulinAmount(50);
            // machine handles this with stepInsulin()
            break;
        case 4: // occlusion
            ui->logger->append("Warning: Occlusion Detected - see MY CGM");
            m->addTimetoHistory(std::string("Warning: Occlusion Detected at "));
            m->addToHistory(std::string("Please check the infusion site for blockages.\nIf the issue persists or you are unsure how to proceed, call Tandem Diabetes customer support at XXX-XXX-XXXX or visit our website for further instructions."));
            break;
        case 5: // critical failure
            ui->logger->append("Warning: Critical Failure - see MY CGM\nStopping insulin delivery.");
            m->addTimetoHistory(std::string("Warning: Critical Failure at "));
            m->addToHistory(std::string("Stopping insulin delivery to prevent incorrect dosing.\nPlease inspect your pump to look for any obstructions or damage.\nIf the issue persists or you are unsure how to proceed, call Tandem Diabetes customer support at XXX-XXX-XXXX or visit our website for further instructions."));
            //stopping insulin delivery
            m->setBasalRate(0);
            bolus->stopOngoingBolus();
            break;
        case 6: // pump shutdown
            ui->logger->append("Warning: Pump Shutdown - see MY CGM\nStopping insulin delivery.");
            m->addTimetoHistory(std::string("Warning: Pump Shutdown at "));
            m->addToHistory(std::string("Stopping insulin delivery to prevent incorrect dosing.\nRestart the system by pressing POWER OFF in the OPTIONS page or refer to your user manual.\nIf the issue persists or you are unsure how to proceed, call Tandem Diabetes customer support at XXX-XXX-XXXX or visit our website for further instructions."));
            //stopping insulin delivery
            m->setBasalRate(0);
            bolus->stopOngoingBolus();
            break;
        case 7: // critically low blood glucose
            m->setCurrentGlucose(3.2);
            // machine handles this with stepBloodGlucose()
            break;
        case 8: // critically high blood glucose
            m->setCurrentGlucose(23);
            m->stepErrorGlucose();
            //machine handles this with stepBloodGlucose()
            break;
      default:
        cout << "Admin error." << endl;
    }


}

//ui->logger->append("Warning: Low Insulin");
//addToHistory(std::string("Warning: Low Insulin"));
//string minuteStr = (currentMinute < 10) ? "0" + std::to_string(currentMinute) : std::to_string(currentMinute);
//string currentTime = std::to_string(currentHour) + ":" + minuteStr + " " + std::to_string(currentDay) + "/" + std::to_string(currentMonth) + "/" + std::to_string(currentYear);
//string event = "Low insulin at " + currentTime;
//addToHistory(event);
