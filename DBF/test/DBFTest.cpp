#include <iostream>
#include "PortMob.h"

bool set_callback(BaseMob* part, std::string& atrb)
{
    PortMob* mob = (PortMob*)part;
    std::cout << "PortMob " << mob->key.name() << " " << atrb << " set " << std::endl;
    return true;
}

bool get_callback(BaseMob* part, std::string& atrb)
{
    PortMob* mob = (PortMob*)part;
    std::cout << "PortMob " << mob->key.name() << " " << atrb << " get " << std::endl;
    return true;
}

int main(void)
{
    PortMob::RegSetCallback(set_callback);
    PortMob::RegGetCallback(get_callback);

    PortMob *p0 = PortMob::AddInst(KeyType::Port, 1);
    PortMob *p3 = PortMob::AddInst(KeyType::Port, 3);

    p0->PortMode.Set(0);
    p3->PortMode.Set(3);

    std::cout << "0 PortMode is " << PortMob::GetInst(0)->PortMode.Get() << std::endl;
    std::cout << "3 PortMode is " << p3->PortMode.Get() << std::endl;

    std::cout << "Inst Number is: " << PortMob::GetInstNum() << std::endl;

    return 0;
}

