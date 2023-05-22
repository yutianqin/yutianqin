#ifndef MSD_CS6015_PROJECT_ENV_H
#define MSD_CS6015_PROJECT_ENV_H

#include <string>
#include "pointer.h"


//----------------------------------------------------------------------------------------------------------------------
class Val;


//----------------------------------------------------------------------------------------------------------------------
class Env {
public:
    static PTR(Env) empty;
    virtual PTR(Val) lookup(std::string find_name) = 0;
};

//----------------------------------------------------------------------------------------------------------------------
class EmptyEnv : public Env {
public:
    EmptyEnv() = default;
    PTR(Val) lookup(std::string find_name) override;
};

//----------------------------------------------------------------------------------------------------------------------
class ExtendedEnv : public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(std::string find_name) override;
};

#endif //MSD_CS6015_PROJECT_ENV_H
