/*代理模式*/
#include <iostream>
#include <string>

class RentHouse
{
public:
    virtual void rentHouse() = 0;
};
/*房东类 - 将房子租出去*/
class Landlord : public RentHouse
{
public:
    void rentHouse()
    {
        std::cout << "将房子租出去" <<std::endl;
    }
};
/*中介代理类 - 对租房功能加强，租客通过中介租房*/
class Intermediary : public Landlord
{
public:
    void rentHouse()
    {
        std::cout << "发布租房公告" << std::endl;
        std::cout << "带人看房" << std::endl;
        _landlord.rentHouse();
        std::cout << "租后维修善后" << std::endl;
    }
private:
    Landlord _landlord;
};

int main()
{
    Intermediary intermediary;
    intermediary.rentHouse();
    return 0;
}