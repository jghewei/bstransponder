#include <iostream>
#include "HAL.h"

int main(void)
{
    HAL::SetInst(new TranspondHAL());
    HAL::GetInst()->SetPortMode(0, 0);

    HAL::SetInst(new MuxponderHAL());
    HAL::GetInst()->SetPortMode(0, 0);

    HAL::SetInst(new RegenHAL());
    HAL::GetInst()->SetPortMode(0, 0);

    return 0;
}

