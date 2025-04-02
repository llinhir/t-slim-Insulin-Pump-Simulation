#ifndef OPTIONS_H
#define OPTIONS_H

#include "createprofile.h"

class Options
{
public:
    Options();
    void stopInsulin();
    void shutDown();
    void sleep();
    void setBasalRate(int basal);
    void deleteProfile();
    Profile* createProfile();

    //getters

    //setters


private:
    //User* user;
    CreateProfile* _createProfile;
};

#endif // OPTIONS_H
