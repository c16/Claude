#include "dice.h"

#include <algorithm>
#include <chrono>
#include <regex>
#include <sstream>

Dice::Dice() {
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  rng_.seed(static_cast<unsigned int>(seed));
}

int Dice::roll(const std::string& notation) {
  // Parse notation like "3d6", "1d20+5", "2d8-1"
  std::regex diceRegex(R"((\d+)d(\d+)([+-]\d+)?)");
  std::smatch match;

  if (std::regex_match(notation, match, diceRegex)) {
    int count = std::stoi(match[1].str());
    int sides = std::stoi(match[2].str());
    int modifier = match[3].matched ? std::stoi(match[3].str()) : 0;

    return rollDice(sides, count, modifier);
  }

  return 0;
}

int Dice::d4(int count, int modifier) {
  return rollDice(4, count, modifier);
}

int Dice::d6(int count, int modifier) {
  return rollDice(6, count, modifier);
}

int Dice::d8(int count, int modifier) {
  return rollDice(8, count, modifier);
}

int Dice::d10(int count, int modifier) {
  return rollDice(10, count, modifier);
}

int Dice::d12(int count, int modifier) {
  return rollDice(12, count, modifier);
}

int Dice::d20(int count, int modifier) {
  return rollDice(20, count, modifier);
}

int Dice::d100(int count, int modifier) {
  return rollDice(100, count, modifier);
}

int Dice::rollAbilityScore() {
  return d6(3);
}

int Dice::rollAbilityScore4d6DropLowest() {
  std::vector<int> rolls;
  for (int i = 0; i < 4; i++) {
    rolls.push_back(d6(1));
  }

  std::sort(rolls.begin(), rolls.end());

  // Sum the three highest
  return rolls[1] + rolls[2] + rolls[3];
}

int Dice::rollDice(int sides, int count, int modifier) {
  std::uniform_int_distribution<int> dist(1, sides);

  int total = 0;
  for (int i = 0; i < count; i++) {
    total += dist(rng_);
  }

  return total + modifier;
}
