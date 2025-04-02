#include "options.h"

Options::Options()
{

}

Profile* Options::createProfile(){
    return _createProfile->completeProfile();
}
