#include "options.h"

Options::Options(Ui::MainWindow *ui) : _ui(ui)
{
    _createProfile = new CreateProfile(ui);
}

Profile *Options::createProfile()
{
    qInfo("Creating profile");
    return _createProfile->completeProfile();
}
