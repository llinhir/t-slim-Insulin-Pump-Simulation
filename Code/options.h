#ifndef OPTIONS_H
#define OPTIONS_H


class Options
{
public:
    Options();
    void stopInsulin();
    void shutDown();
    void sleep();
    void setBasalRate(int basal);
    void deleteProfile();

private:
    //User* user;
};

#endif // OPTIONS_H
