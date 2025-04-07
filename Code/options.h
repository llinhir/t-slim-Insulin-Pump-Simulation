#ifndef OPTIONS_H
#define OPTIONS_H

#include "createprofile.h"

class Options
{
public:
    Options(Ui::MainWindow *ui);
    void stopInsulin();
    void shutDown();
    void sleep();
    void setBasalRate(int basal);
    Profile* createProfile();

    //getters

    //setters


private:
    Ui::MainWindow *_ui;
    CreateProfile* _createProfile;
};

#endif // OPTIONS_H
