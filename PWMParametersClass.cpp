#include "PWMParametersClass.h"

PWMParametersClass::PWMParametersClass(uint8_t pin,
                                       pwmChParamsStruct outEndPts,
                                       pwmChParamsStruct inEndPts,
                                       uint8_t inDeadzone,
                                       bool flipDirection):
    pin(pin),
    outEndPts(outEndPts),
    inEndPts(inEndPts),
    inDeadzone(inDeadzone)
{
    inDeadLowPt = inEndPts.mid - inDeadzone/2;
    inDeadHighPt = inEndPts.mid + inDeadzone/2;
}

PWMParametersClass::~PWMParametersClass()
{
}

// Remaps a signal from the input range to a + or - 100% range
int8_t PWMParametersClass::mapToSign100(uint8_t inputSignal)
{
    int8_t returnVal = 0;                                                        // defaults to deadzone 0
    if(inputSignal < inEndPts.low) returnVal = -100;                             // handles out of range low
    else if(inputSignal > inEndPts.high) returnVal = 100;                        // handles out of range high
    else if(inputSignal >= inDeadHighPt)                                         // maps positive range
    { 
        returnVal = round(map(inputSignal, inDeadHighPt, inEndPts.high, 0, 100));
    }
    else if(inputSignal <= inDeadLowPt)                                          // maps negative range
    {
        returnVal = round(map(inputSignal, inEndPts.low, inDeadLowPt, -100, 0));
    }
    return direction * returnVal;                                                // flip input to output direction here (no math needed)
}

uint16_t PWMParametersClass::sign100ToOutx10(int8_t input100)
{
    if(input100 <= -100) return outEndPts.low*10;
    else if(input100 >= 100) return outEndPts.high*10;
    else if(input100 > 0)
    { 
        return round(map(input100, 0, 100, outEndPts.mid*10, outEndPts.high*10));
    }
    else if(input100 < 0)
    {
        return round(map(input100, -100, 0, outEndPts.low*10, outEndPts.mid*10));
    }
    else return outEndPts.mid*10;
}

// Remaps a signal from the input range to the output range
// Above midpoint and below midpoint are seperate
uint16_t PWMParametersClass::mapToOutx10(uint8_t inputSignal)
{
    sign100ToOutx10(mapToSign100(inputSignal)); // eating the computation hit to allow for reversing direction easily
}

// Remaps a signal from the input range to the output range
// Above midpoint and below midpoint are seperate
uint8_t PWMParametersClass::mapToOut(uint8_t inputSignal)
{
    return round(mapToOutx10(inputSignal) / 10.0);
}

uint8_t PWMParametersClass::sign100ToOut(int8_t input100)
{
    return round(sign100ToOutx10(input100)/10.0);
}