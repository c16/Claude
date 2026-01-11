#ifndef COMBAT_H
#define COMBAT_H

#include <memory>
#include <string>
#include <vector>

#include "character.h"
#include "dice.h"

enum class MonsterType {
  GOBLIN,
  ORC,
  KOBOLD,
  SKELETON,
  ZOMBIE,
  WOLF,
  GIANT_RAT,
  OGRE,
  TROLL,
  DRAGON
};

struct Monster {
  std::string name;
  MonsterType type;
  int hitDice;
  int hitPoints;
  int maxHitPoints;
  int armorClass;
  std::string attackDamage;  // e.g., "1d6"
  int attackBonus;
  int experienceValue;
  int goldDropMin;
  int goldDropMax;

  bool isAlive() const { return hitPoints > 0; }

  static Monster create(MonsterType type, Dice& dice);
};

class Combat {
 public:
  Combat(Character& player, Dice& dice);

  // Combat flow
  void startCombat(const Monster& enemy);
  bool isInCombat() const { return inCombat_; }
  void playerAttack();
  void enemyAttack();
  void playerCastSpell();
  void playerUseItem();
  void playerFlee();
  void endCombat();

  // Combat results
  bool isPlayerTurn() const { return playerTurn_; }
  Monster& getCurrentEnemy() { return currentEnemy_; }

  // Combat actions
  void executeCombatRound();

 private:
  Character& player_;
  Dice& dice_;
  Monster currentEnemy_;
  bool inCombat_;
  bool playerTurn_;

  // Helper methods
  bool rollInitiative();
  int calculateToHit(int attackBonus, int targetAC);
  void awardVictory();
  void handlePlayerDeath();
};

#endif  // COMBAT_H
