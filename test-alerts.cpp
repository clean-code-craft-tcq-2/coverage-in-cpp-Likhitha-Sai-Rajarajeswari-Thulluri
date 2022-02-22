#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
  REQUIRE(inferBreach(25, 20, 30) == NORMAL);
  REQUIRE(inferBreach(35, 20, 30) == TOO_HIGH);
}

TEST_CASE("gets breach values based on cooling type")
{
  std::pair<int, int> breachValues = getTemperatureBreachValues(MED_ACTIVE_COOLING);
  REQUIRE(breachValues.first == 0);
  REQUIRE(breachValues.second == 40);
  breachValues = getTemperatureBreachValues(HI_ACTIVE_COOLING);
  REQUIRE(breachValues.first == 0);
  REQUIRE(breachValues.second == 45);
  breachValues = getTemperatureBreachValues(PASSIVE_COOLING);
  REQUIRE(breachValues.first == 0);
  REQUIRE(breachValues.second == 35);
}

TEST_CASE("classifies temperature breach")
{
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 39) == NORMAL);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1) == TOO_LOW);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 45) == NORMAL);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, -5) == TOO_LOW);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 36) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 20) == NORMAL);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, -3) == TOO_LOW);
}
            
TEST_CASE("sending feed to controller based on breach value")
{
  REQUIRE(sendToController(TOO_HIGH) == "feed : 2");
  REQUIRE(sendToController(TOO_LOW) == "feed : 1");
  REQUIRE(sendToController(NORMAL) == "feed : 0");
}

TEST_CASE("Sending mail to recepient based on breach value")
{
  std::string recepient = "ab@c.com";
  REQUIRE(sendToEmail(TOO_HIGH, recepient) == "To: ab@c.com .Hi, the temperature is too high");
  REQUIRE(sendToEmail(TOO_LOW, recepient) == "To: ab@c.com .Hi, the temperature is too low");
  REQUIRE(sendToEmail(NORMAL, recepient) == "");
}
