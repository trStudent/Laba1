#include <core/General/Employee.h>

using namespace core::General;

Employee::Employee() noexcept 
    : id_(ID_MIN), name_(""), hours_(0)
{
}

Employee::Employee(ID_TYPE id, const char* name, double hours) 
    : id_(id), hours_(hours)
{
    for(size_t i = 0; name[i] != '\0' && i < BUFF_SIZE; i++)
        name_[i] = name[i];
}

Employee::ID_TYPE Employee::id() const noexcept
{ return id_; }
const char* Employee::name() const noexcept
{ return name_; }
double Employee::hours() const noexcept
{ return hours_; }

Employee::ID_TYPE& Employee::id() noexcept
{ return id_; }
char* Employee::name() noexcept
{ return name_; }
double& Employee::hours() noexcept
{ return hours_; }