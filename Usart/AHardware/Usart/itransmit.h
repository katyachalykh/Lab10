#pragma once

class ITransmit
{
public:
  virtual void OnNextByteTransmit() = 0;
};
