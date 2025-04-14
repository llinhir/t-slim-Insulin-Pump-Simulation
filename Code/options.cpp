#include "options.h"

Options::Options(Ui::MainWindow *ui) : _ui(ui)
{
    _createProfile = new CreateProfile(ui);
}

Profile *Options::createProfile()
{
    qInfo("Creating profile");
    //_ui->stackedWidget->setCurrentIndex(EDIT_SPECIFIC_PROFILE_PAGE); // brings the user to see all the profiles
    return _createProfile->completeProfile();
}
