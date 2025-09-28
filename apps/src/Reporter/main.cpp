#include <iostream>
#include <tchar.h>
#include <sstream>
#include <string>
#include <limits>
#include <core/General/Employee.h>
#include <core/General/File.h>

using namespace core::General;

bool stringToDouble(const std::string& s, double& result) {
    char* end;
    errno = 0;
    result = std::strtod(s.c_str(), &end);

    if (end == s.c_str()) return false;
    if (*end != '\0') return false;
    if (errno == ERANGE) return false;
    return true;
}

std::string getFileName(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

std::string formatFixed(const std::string& input, std::size_t N, char fill = '*') {
    if (input.size() == N)
        return input;
    else if (input.size() < N)
        return input + std::string(N - input.size(), fill);
    else
        return input.substr(0, N);
}

int _tmain(int argc, _TCHAR* argv[])
{
    if(argc != 4)
        return -1;
    
    File binf = File::open(argv[1],
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                        nullptr,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        nullptr);
    if(!binf)
        return -1;
    
    File repf = File::open(argv[2],
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                        nullptr,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        nullptr);
    if(!repf)
        return -1;

    double per_hour;
    if(!stringToDouble(argv[3], per_hour))
        return -1;

    std::stringstream output;
    output << "Report on file \"" << getFileName(argv[1]) << "\"\n";
    output << "Employee id ------ Employee name ------ Employee hours ------ Employee wage ------\n";
    char s[Employee::BUFF_SIZE];
    while(binf.read(s, Employee::SERIALIZED_SIZE))
    {
        Employee temp = Employee::deserialize(s);

        std::string id = std::to_string(temp.id()) + " ";
        output << formatFixed(id, 18, '-') << " ";
        
        std::string name = temp.name();
        output << formatFixed(name, 20, '-') << " ";

        std::string hours = std::to_string(temp.hours()) + " ";
        output << formatFixed(hours, 21, '-') << " ";

        std::string wage = std::to_string(temp.hours() * per_hour) + " ";
        output << formatFixed(wage, 20, '-') << '\n';
    }
    repf.write(output.str().c_str(), output.str().size());
    
    std::cout << output.str();
    
    return 0;
}