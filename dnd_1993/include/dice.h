#ifndef DICE_H
#define DICE_H

#include <random>
#include <string>
#include <vector>

class Dice {
 public:
  Dice();

  // Roll dice notation (e.g., "3d6", "1d20+5", "2d8-1")
  int roll(const std::string& notation);

  // Roll specific dice
  int d4(int count = 1, int modifier = 0);
  int d6(int count = 1, int modifier = 0);
  int d8(int count = 1, int modifier = 0);
  int d10(int count = 1, int modifier = 0);
  int d12(int count = 1, int modifier = 0);
  int d20(int count = 1, int modifier = 0);
  int d100(int count = 1, int modifier = 0);

  // Ability score generation (3d6)
  int rollAbilityScore();

  // Best 3 of 4d6 (common method)
  int rollAbilityScore4d6DropLowest();

 private:
  std::mt19937 rng_;
  int rollDice(int sides, int count, int modifier);
};

#endif  // DICE_H
