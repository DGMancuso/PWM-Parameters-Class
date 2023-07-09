#ifndef PWM_PARAMETERS_CLASS
#define PWM_PARAMETERS_CLASS
#include <Arduino.h>

// returns the - / 0 / + status of an integer
// int8_t sign(int x){return (x > 0) - (x < 0);}; 

// defines pwm endpoints
// numbers are x 10 nanoseconds (e-8 s) - max 2.55 microseconds
struct pwmChParamsStruct {

  uint8_t low = 100;
  uint8_t mid = 150;
  uint8_t high= 200;
};

// defines pwm channel properties such as endpoints and provides means of interaction
// numbers are x 10 nanoseconds (e-8 s) - max 2.55 microseconds
class PWMParametersClass
{
public:
  pwmChParamsStruct outEndPts; // * 10 ns
  pwmChParamsStruct inEndPts;    // * 10 ns
  uint8_t outDeadzone = 0; // * 10 ns
  uint8_t inDeadzone;
  uint8_t pin;
  uint8_t inDeadLowPt;
  uint8_t inDeadHighPt;
  int8_t direction = 1;

public:
    PWMParametersClass(uint8_t pin,
                       pwmChParamsStruct outEndPts = {100, 150, 200},
                       pwmChParamsStruct inEndPts = {100, 150, 200},
                       uint8_t inDeadzone = 0,
                       bool flipDirection = false);

    ~PWMParametersClass();

    // scales an input signal using input endpoints then returns remap to output endpoints scale
    uint8_t mapToOut(uint8_t inputSignal);
    uint16_t mapToOutx10(uint8_t inputSignal);

    // scales input signal and maps to plus or minus 100% of range
    // positive and negative are not nessesarily equal increments
    int8_t mapToSign100(uint8_t inputSignal);
    

    // outputs signal based on signed percentage of 0 to 100 input
    uint8_t sign100ToOut(int8_t input100);
    uint16_t sign100ToOutx10(int8_t input100);

};

#endif // PWM_PARAMETERS_CLASS