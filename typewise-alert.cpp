#include "typewise-alert.h"
#include <iostream>
#include <sstream>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  std::pair<int, int> breachValues = getTemperatureBreachValues(coolingType);
  return inferBreach(temperatureInC, breachValues.first, breachValues.second);
}

std::pair<int, int> getTemperatureBreachValues(CoolingType coolingType)
{
  std::pair<int, int> breachValues{0,35};
  switch(coolingType) {
    case HI_ACTIVE_COOLING:
      breachValues = {0,45};
      break;
    case MED_ACTIVE_COOLING:
      breachValues = {0,40};
      break;
  }
  return breachValues;
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  std::string output = (alertTarget == TO_CONTROLLER) ? sendToController(breachType) : sendToEmail(breachType, "a.b@c.com");
  printOnConsole(output);
}

std::string sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  std::stringstream ss;
  ss<<std::hex<<header<<" : "<<breachType;
  return ss.str();
}

std::string sendToEmail(BreachType breachType, std::string recepient) {
  std::string output;
  switch(breachType) {
    case TOO_LOW:
      output = "To: "+recepient+" .Hi, the temperature is too low";
      break;
    case TOO_HIGH:
      output = "To: "+recepient+" .Hi, the temperature is too high";
      break;
  }
  return output;
}

void printOnConsole(std::string st)
{
  std::cout<<st<<std::endl;
}
