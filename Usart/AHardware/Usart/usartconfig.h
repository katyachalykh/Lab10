#pragma once
#include "usartDriver.h"
#include "usart.h"
#include "usart2registers.hpp"

class MyUsart;

inline UsartDriver<MyUsart> usartDriver;

class MyUsart : public Usart<USART2, usartDriver>
{
};