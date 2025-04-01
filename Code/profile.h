#ifndef PROFILE_H
#define PROFILE_H

#include <QMainWindow>
#include <QTextBrowser>

using namespace std;

class Profile {
public:
    Profile(int basalRate, int carbohydrateRatio, int correctionFactor, int targetGlucoseLevel, string profileName, string password);

    int getBasalRate() {return _basalRate; }
    int getCarbohydrateRatio() {return _carbohydrateRatio; }
    int getCorrectionFactor() {return _correctionFactor; }
    int getTargetGlucoseLevel() {return _targetGlucoseLevel; }
    string getProfileName() {return _profileName; }
    string getPassword() {return _password; }

private:
    int _basalRate;
    int _carbohydrateRatio;
    int _correctionFactor;
    int _targetGlucoseLevel;
    string _profileName;
    string _password;
};

#endif // PROFILE_H
