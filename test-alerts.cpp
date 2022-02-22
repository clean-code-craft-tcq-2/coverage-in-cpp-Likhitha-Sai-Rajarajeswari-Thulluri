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
}

TEST_CASE("classifies temperature breach")
{
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41) == TOO_HIGH);
}
            
TEST_CASE("sending feed to controller based on breach value")
{
  REQUIRE(sendToController(TOO_HIGH) == "feed : 2");
}

TEST_CASE("Sending mail to recepient based on breach value")
{
  std::string recepient = "ab@c.com";
  REQUIRE(sendToEmail(TOO_HIGH, recepient) == "To: ab@c.com .Hi, the temperature is too high");
}
