#pragma once
#include "itransmit.h"
#include <cassert>
#include <array>

template<typename TUsart>
class UsartDriver: public ITransmit
{
private:
  uint8_t transmitBuffer[255];
  size_t size = 0U;
  uint8_t i = 0U;
  
public:
  void SendMessage(const char* message, size_t aSize)
  {
    assert(size<=255);
    memcpy(transmitBuffer, message, aSize);
    size = aSize;
    i=0U;
    TUsart::WriteByte(transmitBuffer[i++]);
    TUsart::EnableTransmit();
    TUsart::InterruptEnable();
  }
  
  void OnNextByteTransmit()
  {
    TUsart::WriteByte(transmitBuffer[i++]);
    if (i>=size)
    {
      TUsart::DisableTransmit();
      TUsart::InterruptDisable();
      i=0U;
    }    
  }
  
};