#include <iostream>
#include <tchar.h>
#include <string>
#include <limits>
#include <core/General/Employee.h>
#include <core/General/File.h>

using namespace core::General;


bool stringToInt(const std::string& str, int& number) {
    char* end;
    long val = std::strtol(str.c_str(), &end, 10);

    if (*end != '\0') return false;
    if (val < INT_MIN || val > INT_MAX) return false;

    number = static_cast<int>(val);
    return true;
}

template<class T>
void loopUntilCorr(T& n)
{
    while(true)
    {
        std::cin >> n;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again." << std::endl;
        } else
            break;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    if(argc != 3)
        return -1;
    File f = File::open(argv[1],
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                        nullptr,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        nullptr);
    int num;
    if(!stringToInt(argv[2], num))
        return -1;
    
    for(int i = 0; i < num; i++) {
        std::cout << "Employee N" << (i + 1) << std::endl;

        Employee entered;
        std::cout << "Enter employee ID." << std::endl;
        loopUntilCorr(entered.id());

        std::cout << "Enter employee name." << std::endl;
        std::string name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, name);
        memcpy(entered.name(), name.c_str(), Employee::BUFF_SIZE - 1);
        entered.name()[Employee::BUFF_SIZE - 1] = '\0';

        std::cout << "Enter amount of hours employee worked." << std::endl;
        loopUntilCorr(entered.hours());

        auto m = entered.serialize();
        if(!f.write(m.data(), Employee::SERIALIZED_SIZE))
            return -1;
    }
    return 0;
}