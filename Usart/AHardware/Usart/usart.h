#pragma once
#include <cstdint>
#include "itransmit.h"
#include "usart2registers.hpp"
#include "usartdriver.h"

template<typename TUSARTReg, auto& aTransmitter>

class Usart
{
private:
  inline static ITransmit& transmitter = aTransmitter;
  
public:
  static void WriteByte(std::uint8_t byte)
  {
    TUSARTReg::DR::Write(byte);
  }
  static void InterruptHandler()
  {
    if(TUSARTReg::SR::TXE::DataRegisterEmpty::IsSet() && TUSARTReg::CR1::TXEIE::InterruptWhenTXE::IsSet())
      {
        transmitter.OnNextByteTransmit();
      }
  }
  static void EnableTransmit()
  {
    TUSARTReg::CR1::TE::Enable::Set();
  }
  static void DisableTransmit()
  {
    TUSARTReg::CR1::TE::Disable::Set();
    
  } 
  static void InterruptEnable ()
  {
    TUSARTReg::CR1::TXEIE::InterruptWhenTXE::Set();
  }
  static void InterruptDisable ()
  {
    TUSARTReg::CR1::TXEIE::InterruptInhibited::Set();
  }
};