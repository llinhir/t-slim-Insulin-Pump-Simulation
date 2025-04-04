#include "simulation.h"

Simulation::Simulation(Ui::MainWindow *ui)
{

    m = new machine(ui);
    this->ui = ui;
    bolus = new Bolus(ui, m);
}

////void Simulation::startSimulation(){
//////    bool correctPass = false;
//////    string
//////    if(isCharging){
//////        while(!correctPass){

//////            correctPass = enterPassword(profile, input);
//////        }
//////    }
////}

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

////    QString password = ui->passwordInput->text(); // Assuming "passwordInput" is your QLineEdit
////    if(password.toStdString() == profile->getPassword()){
////        qInfo("Correct Password");
////        return true;
////    }else{
////        qInfo("Incorrect Password");
////        return false;
////    }
//}
