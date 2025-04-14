#ifndef PROFILE_H
#define PROFILE_H

#include <QMainWindow>
#include <QTextBrowser>

using namespace std;

class Profile
{
public:
    Profile(double basalRate, int carbohydrateRatio, int correctionFactor, int targetGlucoseLevel, string profileName);

    // Getters
    double getBasalRate() { return _basalRate; }
    int getCarbohydrateRatio() { return _carbohydrateRatio; } // insulin to carb ratio
    int getCorrectionFactor() { return _correctionFactor; }
    int getTargetGlucoseLevel() { return _targetGlucoseLevel; }
    string getProfileName() { return _profileName; }

    // Setters
    void setBasalRate(double basalRate) { _basalRate = basalRate; }
    void setCarbohydrateRatio(int carbohydrateRatio) { _carbohydrateRatio = carbohydrateRatio; }
    void setCorrectionFactor(int correctionFactor) { _correctionFactor = correctionFactor; }
    void setTargetGlucoseLevel(int targetGlucoseLevel) { _targetGlucoseLevel = targetGlucoseLevel; }
    void setProfileName(string profileName) { _profileName = profileName; }

private:
    double _basalRate;
    int _carbohydrateRatio;
    int _correctionFactor;
    int _targetGlucoseLevel;
    string _profileName;
};

#endif // PROFILE_H
