#include "insulin.h"

Insulin::Insulin(Ui::MainWindow *ui, machine* machine)
{
    _machine = machine;
    _ui = ui;
    qInfo(" inin sulin");
    connect(_ui->insulinButton, &QPushButton::clicked, this, [this]()
                { setBasalRates(); });
}


void Insulin::setBasalRates(){
    qInfo("setting basal rates");
} // grabs the basal rates from profiles -> sets them as options

void Insulin::startInsulinDelivery(float insulinAmt){
    cout << "Delivering insulin: " << insulinAmt << " units" << endl;
    // edit device and user values
}
void Insulin::pauseInsulin(){

}

void Insulin::manualStopInsulin(){

}
void Insulin::autoStopInsulin(){
    // write to logger with _ui->logger->append or something like that
}
