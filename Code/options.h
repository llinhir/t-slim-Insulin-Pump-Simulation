#ifndef OPTIONS_H
#define OPTIONS_H

#include "createprofile.h"
#include "defs.h"

class Options
{
public:
    Options(Ui::MainWindow *ui);
    void stopInsulin();
    void shutDown();
    void sleep();
    void setBasalRate(double basal);
    Profile* createProfile();

    //getters

    //setters


private:
    Ui::MainWindow *_ui;
    CreateProfile* _createProfile;
};

#endif // OPTIONS_H
