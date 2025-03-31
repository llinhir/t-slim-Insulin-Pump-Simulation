#include "simulation.h"

Simulation::Simulation()
{
    // initialize values and classes
    isCharging = true;
//    machine = new Machine();
//    user = new User();

    startSimulation();
}

void Simulation::startSimulation(){
    bool correctPass = false;
    if(isCharging){
        while(!correctPass){
            correctPass = enterPassword();
        }
    }

}
bool Simulation::enterPassword(){
    // checks the users password
}
