#include "typewise-alert.h"
#include <stdio.h>

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
  std::pair<int, int> breachValue{0,0};
  switch(coolingType) {
    case PASSIVE_COOLING:
      breachValues{0,35};
      break;
    case HI_ACTIVE_COOLING:
      breachValues{0,45};
      break;
    case MED_ACTIVE_COOLING:
      breachValues{0,40};
      break;
  }
  return breachValue;
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
  std::string st;
  switch(breachType) {
    case TOO_LOW:
      st = "To: "+recepient+" .Hi, the temperature is too low";
      break;
    case TOO_HIGH:
      st = "To: "+recepient+" .Hi, the temperature is too high";
      break;
  }
}

void printOnConsole(std::string st)
{
  printf("%s\n", st);
}
