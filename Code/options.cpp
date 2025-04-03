#include "options.h"

Options::Options(Ui::MainWindow *ui) :
    _ui(ui)
{
    _createProfile = new CreateProfile(ui);
}

Profile* Options::createProfile(){
    return _createProfile->completeProfile();
}
