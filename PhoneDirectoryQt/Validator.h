#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include "Contact.h"
using namespace std;
class Validator {
public:
    static bool validateName(const string& name);
    static bool validatePhone(const string& phone);
    static bool validateEmail(const string& email);
    static bool validateDate(const string& date);
};

#endif // VALIDATOR_H
