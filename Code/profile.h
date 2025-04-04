#ifndef PROFILE_H
#define PROFILE_H

#include <QMainWindow>
#include <QTextBrowser>

using namespace std;

class Profile
{
public:
    Profile(int basalRate, int carbohydrateRatio, int correctionFactor, int targetGlucoseLevel, string profileName);

    int getBasalRate() { return _basalRate; }
    int getCarbohydrateRatio() { return _carbohydrateRatio; } // insulin to carb ratio
    int getCorrectionFactor() { return _correctionFactor; }
    int getTargetGlucoseLevel() { return _targetGlucoseLevel; }
    string getProfileName() { return _profileName; }

private:
    int _basalRate;
    int _carbohydrateRatio;
    int _correctionFactor;
    int _targetGlucoseLevel;
    string _profileName;
};

#endif // PROFILE_H
