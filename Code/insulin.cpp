#include "insulin.h"

Insulin::Insulin(Ui::MainWindow *ui, machine *machine)
{
    _machine = machine;
    _ui = ui;
    isPausedBasal = false;
    currBasalRate = 0;

    connectSlots();
}
////////////////////////////////////////////////////
//          Basal functions start here            //
////////////////////////////////////////////////////

// sets the currBasalRate of the machine
void Insulin::setCurrBasalRate(size_t index)
{
    if (!_machine->getProfile(index))
    {
        return;
    }

    currBasalRate = _machine->getProfile(index)->getBasalRate();
    _ui->yourBasalRateText->setText(QString::number(currBasalRate, 'f', 2) + " units/hour");
}

// begins delivering insulin at a rate of currBasalRate units/hour
void Insulin::startBasalDelivery()
{
    cout << "Delivering Basal: " << currBasalRate << " units" << endl;
    if (!currBasalRate)
    {
        return;
    }
    if (!_machine) {
        std::cout << "[ERROR] _machine is null!" << std::endl;
        return;
    }

    _machine->setBasalRate(currBasalRate);
    _ui->logger->append("Set basal rate: " + QString::number(currBasalRate, 'f', 2) + "u/h");
    _ui->basalStatNumber->display(currBasalRate);
}

// pauses basal insulin delivery
void Insulin::pauseBasal()
{
    if (!isPausedBasal && currBasalRate != 0)
    {
        isPausedBasal = true;
        _machine->setBasalRate(0);
        _ui->logger->append("Basal paused");
        _ui->basalStatNumber->display(0);
        _ui->pauseBasalButton->setText("Pause");
    }
    else if (isPausedBasal && currBasalRate != 0)
    {
        isPausedBasal = false;
        _machine->setBasalRate(currBasalRate);
        _ui->logger->append("Basal resumed");
        _ui->basalStatNumber->display(currBasalRate);
        _ui->pauseBasalButton->setText("Unpause");
    }
}

// stops basal insulin delivery
void Insulin::stopBasal()
{
    if (currBasalRate == 0)
    {
        return; // basal is currently not running, so return
    }
    currBasalRate = 0;
    _machine->setBasalRate(currBasalRate);
    _ui->logger->append("Basal stopped");
    _ui->yourBasalRateText->setText("0.0 units/hour");
    _ui->basalStatNumber->display(currBasalRate);
}

////////////////////////////////////////////////////
//          Bolus functions start here            //
////////////////////////////////////////////////////

void Insulin::startBolusDelivery(double rate)
{
    cout << "Delivering Bolus: " << rate << " units" << endl;
    _ui->logger->append("Bolus delivery: " + QString::number(rate, 'f', 1) + " units");
    _machine->consumeInsulin(rate);
}

//void Insulin::pauseBolus() no use?
//{

//}

//void Insulin::stopBolus()
//{
//}

////////////////////////////////////////////////////
//          extra functions start here            //
////////////////////////////////////////////////////
void Insulin::connectSlots()
{
    // selects the basal rate to the profile the user clicks
    connect(_ui->basalRate1, &QPushButton::clicked, this, [this]()
            { setCurrBasalRate(0); });
    connect(_ui->basalRate2, &QPushButton::clicked, this, [this]()
            { setCurrBasalRate(1); });
    connect(_ui->basalRate3, &QPushButton::clicked, this, [this]()
            { setCurrBasalRate(2); });
    connect(_ui->basalRate4, &QPushButton::clicked, this, [this]()
            { setCurrBasalRate(3); });
    connect(_ui->basalRate5, &QPushButton::clicked, this, [this]()
            { setCurrBasalRate(4); });

    // Basal button
    connect(_ui->startInsulin, &QPushButton::clicked, this, [this]()
            { startBasalDelivery(); });
    connect(_ui->pauseBasalButton, &QPushButton::clicked, this, [this]()
            { pauseBasal(); });
    connect(_ui->stopBasalButton, &QPushButton::clicked, this, [this]()
            { stopBasal(); });
}
