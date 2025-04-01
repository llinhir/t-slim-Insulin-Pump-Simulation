#ifndef PROFILE_H
#define PROFILE_H

#include <QMainWindow>
#include <QTextBrowser>

using namespace std;

class Profile {
public:
    Profile(int basalRate, int carbohydrateRatio, int correctionFactor, int targetGlucoseLevel, string profileName, int password);

    int getBasalRate() {return _basalRate; }
    int getCarbohydrateRatio() {return _carbohydrateRatio; }
    int getCorrectionFactor() {return _correctionFactor; }
    int getTargetGlucoseLevel() {return _targetGlucoseLevel; }
    string getProfileName() {return _profileName; }
    int getPassword() {return _password; }

private:
    int _basalRate;
    int _carbohydrateRatio;
    int _correctionFactor;
    int _targetGlucoseLevel;
    string _profileName;
    int _password;
};

#endif // PROFILE_H
