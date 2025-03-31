#ifndef SIMULATION_H
#define SIMULATION_H


class Simulation
{
public:
    Simulation();
    void startSimulation();
    bool enterPassword();

private:
    bool isCharging;
    //Machine* machine;
};

#endif // SIMULATION_H
