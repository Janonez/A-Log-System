#include "util.hpp"

int main()
{
    std::cout << Log_System::util::Date::GetTime() << std::endl;
    std::string pathname = "./abc/bcd/a.txt";
    Log_System::util::File::CreateDirectory(Log_System::util::File::Path(pathname));
    return 0;
}