#include "createprofile.h"
#include <iostream>

CreateProfile::CreateProfile(Ui::MainWindow *ui) :
    _ui(ui)
{

}


Profile* CreateProfile::completeProfile(){
    _inputBasalRate = _ui->editBasalRate->text().toDouble(); //Name of UI Element for this field
    _inputCarbohydrateRatio = _ui->editCarbohydrateRatio->text().toInt();//Name of UI Element for this field
    _inputCorrectionFactor = _ui->editCorrectionFactor->text().toInt();//Name of UI Element for this field
    _inputTargetGlucoseLevel = _ui->editTargetGlucose->text().toInt();//Name of UI Element for this field
    _inputProfileName = _ui->editProfileName->text().toStdString();//Name of UI Element for this field
    return new Profile(_inputBasalRate, _inputCarbohydrateRatio, _inputCorrectionFactor, _inputTargetGlucoseLevel, _inputProfileName);
}
