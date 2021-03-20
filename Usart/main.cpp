#include "rccregisters.hpp"
#include "gpioaregisters.hpp"
#include <iostream>
#include <bitset>
#include "nvicregisters.hpp"
#include "tim2registers.hpp"
#include "usart.h"
#include "usartconfig.h"

extern "C"
{
  int __low_level_init(void)
  {
    RCC::CR::HSEON::On::Set();
    while(!RCC::CR::HSERDY::Ready::IsSet())
    {
    }

    RCC::CFGR::SW::Hse::Set();
    while(!RCC::CFGR::SWS::Hse::IsSet())
    {
    }
    RCC::CR::HSION::Off::Set();
    
    RCC::AHB1ENR::GPIOAEN::Enable::Set();
    GPIOA::MODER::MODER3::Alternate::Set(); // TX
    GPIOA::MODER::MODER2::Alternate::Set(); //RX
    GPIOA::AFRL::AFRL2::Af7::Set(); // функция альтернативного порта
    GPIOA::AFRL::AFRL3::Af7::Set();
    GPIOA::OTYPER::OT2::OutputPushPull::Set();
    GPIOA::OTYPER::OT3::OutputPushPull::Set();
    GPIOA::PUPDR::PUPDR2::PullUp::Set();
    GPIOA::PUPDR::PUPDR3::PullUp::Set(); // муть с портами
    

    RCC::APB2ENR::ADC1EN::Enable::Set();
    RCC::APB1ENR::USART2EN::Enable::Set(); // тактиррование
    
    USART2::CR1::M::Data8bits::Set(); // 8 бит длина
    USART2::CR1::PCE::ParityControlDisable::Set(); // четность
    USART2::CR1::OVER8::OversamplingBy16::Set(); // дискретизация 1/16
    USART2::CR2::STOP::Value0::Set(); //1 стоп бит
    USART2::BRR::Write((8'000'000/(9600*8*2) << 4U)); //скорость
    USART2::CR1::UE::Enable::Set(); //включение
    USART2::CR1::TE::Enable::Set();
    
    NVIC::ISER1::Write(1<<6); 
    
    RCC::APB1ENR::TIM2EN::Enable::Set();
    
    TIM2::PSC::Set(7999U);
    TIM2::ARR::Write(1000);
    TIM2::SR::UIF::NoInterruptPending::Set();
    TIM2::CNT::Write(0U);
    TIM2::CR1::CEN::Enable::Set();
    
    return 1;
  }
}

int main()
{

  const char* message = "Hello, world! \n" ;
     
 
  for (;;)
  {
    usartDriver.SendMessage(message,strlen(message));
    while (!TIM2::SR::UIF::InterruptPending::IsSet());
    TIM2::SR::UIF::NoInterruptPending::Set();
    
  }
  return 0;
}
