#include <iostream>
#include <tchar.h>
#include <string>
#include <limits>
#include <core/General/Process.h>

using namespace core::General;

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

void finish()
{
    std::cout << "Something went wrong. Aborting the app..." << std::endl;
    system("pause");
    exit(-1);
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::cout << "Enter name of binary file where data will be stored."
              << std::endl;
    std::string path;
    std::getline(std::cin, path);


    std::cout << "Enter amount of records to be made. " << std::endl;
    int records;
    loopUntilCorr(records);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    STARTUPINFOW si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    std::string args = path + " " + std::to_string(records);
    std::string cmd = "Creator.exe " + args;

    Process p = Process::create_utf8("", cmd,  NULL,  NULL,  false, 0, NULL, "", si);

    p.wait();

    if(!p.try_exit_code().has_value() || p.try_exit_code().value() != 0)
        finish();

    double per_hour;
    std::cout << "Enter wage per hour." << std::endl;
    loopUntilCorr(per_hour);

    std::cout << "Enter name of file where report will be stored."
              << std::endl;
    std::string reppath;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, reppath);

    args = path + " " + reppath + " " + std::to_string(per_hour);
    cmd = "Reporter.exe " + args;

    p = Process::create_utf8("", cmd,  NULL,  NULL,  false, 0, NULL, "", si);

    p.wait();

    if(!p.try_exit_code().has_value() || p.try_exit_code().value() != 0)
        finish();

    return 0;
}