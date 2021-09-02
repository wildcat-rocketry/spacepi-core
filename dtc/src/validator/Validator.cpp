#include <memory>
#include <spacepi/dtc/validator/Validator.hpp>
#include <spacepi/dtc/validator/ValidatorImpl.hpp>

using namespace std;
using namespace spacepi::dtc::validator;

shared_ptr<Validator> Validator::instance(new ValidatorImpl());
