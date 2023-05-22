#include "headers/Env.h"
#include <utility>

//----------------------------------------------------------------------------------------------------------------------
PTR(Env) Env::empty = NEW(EmptyEnv)();

//----------------------------------------------------------------------------------------------------------------------
PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}

//----------------------------------------------------------------------------------------------------------------------
ExtendedEnv::ExtendedEnv(std::string name, std::shared_ptr<Val> val, std::shared_ptr<Env> rest) {
    this->name = std::move(name);
    this->val = std::move(val);
    this->rest = std::move(rest);
}
PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == name)
        return val;
    else
        return rest->lookup(find_name);
}
