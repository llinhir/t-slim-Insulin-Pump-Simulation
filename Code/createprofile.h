#ifndef CREATEPROFILE_H
#define CREATEPROFILE_H

#include <QMainWindow>
#include <QTextBrowser>
#include "profile.h"
#include "ui_mainwindow.h"

class CreateProfile {
public:
    CreateProfile(Ui::MainWindow *ui);

    Profile* completeProfile();

private:
    Ui::MainWindow *_ui;
    int _inputBasalRate;
    int _inputCarbohydrateRatio;
    int _inputCorrectionFactor;
    int _inputTargetGlucoseLevel;
    string _inputProfileName;
};

#endif // CREATEPROFILE_H
