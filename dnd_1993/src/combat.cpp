#include "combat.h"

#include <iostream>

Monster Monster::create(MonsterType type, Dice& dice) {
  Monster monster;
  monster.type = type;

  switch (type) {
    case MonsterType::GOBLIN:
      monster.name = "Goblin";
      monster.hitDice = 1;
      monster.armorClass = 6;
      monster.attackDamage = "1d6";
      monster.attackBonus = 0;
      monster.experienceValue = 15;
      monster.goldDropMin = 1;
      monster.goldDropMax = 10;
      break;

    case MonsterType::KOBOLD:
      monster.name = "Kobold";
      monster.hitDice = 1;
      monster.armorClass = 7;
      monster.attackDamage = "1d4";
      monster.attackBonus = -1;
      monster.experienceValue = 5;
      monster.goldDropMin = 0;
      monster.goldDropMax = 5;
      break;

    case MonsterType::ORC:
      monster.name = "Orc";
      monster.hitDice = 1;
      monster.armorClass = 6;
      monster.attackDamage = "1d8";
      monster.attackBonus = 0;
      monster.experienceValue = 10;
      monster.goldDropMin = 2;
      monster.goldDropMax = 12;
      break;

    case MonsterType::SKELETON:
      monster.name = "Skeleton";
      monster.hitDice = 1;
      monster.armorClass = 7;
      monster.attackDamage = "1d6";
      monster.attackBonus = 0;
      monster.experienceValue = 35;
      monster.goldDropMin = 0;
      monster.goldDropMax = 0;
      break;

    case MonsterType::ZOMBIE:
      monster.name = "Zombie";
      monster.hitDice = 2;
      monster.armorClass = 8;
      monster.attackDamage = "1d8";
      monster.attackBonus = 0;
      monster.experienceValue = 50;
      monster.goldDropMin = 0;
      monster.goldDropMax = 0;
      break;

    case MonsterType::WOLF:
      monster.name = "Wolf";
      monster.hitDice = 2;
      monster.armorClass = 7;
      monster.attackDamage = "1d6";
      monster.attackBonus = 1;
      monster.experienceValue = 35;
      monster.goldDropMin = 0;
      monster.goldDropMax = 0;
      break;

    case MonsterType::GIANT_RAT:
      monster.name = "Giant Rat";
      monster.hitDice = 1;
      monster.armorClass = 7;
      monster.attackDamage = "1d4";
      monster.attackBonus = 0;
      monster.experienceValue = 7;
      monster.goldDropMin = 0;
      monster.goldDropMax = 2;
      break;

    case MonsterType::OGRE:
      monster.name = "Ogre";
      monster.hitDice = 4;
      monster.armorClass = 5;
      monster.attackDamage = "1d10";
      monster.attackBonus = 2;
      monster.experienceValue = 175;
      monster.goldDropMin = 10;
      monster.goldDropMax = 100;
      break;

    case MonsterType::TROLL:
      monster.name = "Troll";
      monster.hitDice = 6;
      monster.armorClass = 4;
      monster.attackDamage = "2d6";
      monster.attackBonus = 3;
      monster.experienceValue = 650;
      monster.goldDropMin = 20;
      monster.goldDropMax = 200;
      break;

    case MonsterType::DRAGON:
      monster.name = "Young Dragon";
      monster.hitDice = 8;
      monster.armorClass = 2;
      monster.attackDamage = "2d8";
      monster.attackBonus = 5;
      monster.experienceValue = 1750;
      monster.goldDropMin = 100;
      monster.goldDropMax = 1000;
      break;
  }

  // Roll HP
  monster.maxHitPoints = dice.d8(monster.hitDice);
  monster.hitPoints = monster.maxHitPoints;

  return monster;
}

Combat::Combat(Character& player, Dice& dice)
    : player_(player), dice_(dice), inCombat_(false), playerTurn_(true) {}

void Combat::startCombat(const Monster& enemy) {
  currentEnemy_ = enemy;
  inCombat_ = true;

  std::cout << "\n========================================\n";
  std::cout << "  A " << currentEnemy_.name << " appears!\n";
  std::cout << "========================================\n";
  std::cout << "Enemy HP: " << currentEnemy_.hitPoints << "\n";
  std::cout << "Enemy AC: " << currentEnemy_.armorClass << "\n\n";

  // Roll initiative
  playerTurn_ = rollInitiative();

  if (!playerTurn_) {
    std::cout << "The " << currentEnemy_.name << " strikes first!\n";
    enemyAttack();
  }
}

void Combat::playerAttack() {
  if (!inCombat_ || !playerTurn_) return;

  std::cout << "\nYou attack the " << currentEnemy_.name << "!\n";

  // Roll to hit (d20 + attack bonus)
  int roll = dice_.d20();
  int totalRoll = roll + player_.getAttackBonus();

  std::cout << "Attack roll: " << roll;
  if (player_.getAttackBonus() != 0) {
    std::cout << " + " << player_.getAttackBonus() << " = " << totalRoll;
  }
  std::cout << " vs AC " << currentEnemy_.armorClass << "\n";

  // In AD&D, you need to roll >= (20 - AC) to hit, simplified here
  int targetRoll = 20 - currentEnemy_.armorClass;

  if (roll == 20) {
    std::cout << "*** CRITICAL HIT! ***\n";

    // Get weapon damage
    const Item* weapon = player_.getInventory().getEquippedWeapon();
    int damage = 0;

    if (weapon) {
      damage = dice_.roll(weapon->damage) * 2;  // Double damage on crit
    } else {
      damage = dice_.d4() * 2;  // Unarmed crit
    }

    damage += player_.getDamageBonus();
    if (damage < 1) damage = 1;

    std::cout << "You deal " << damage << " damage!\n";
    currentEnemy_.hitPoints -= damage;

  } else if (roll == 1) {
    std::cout << "*** CRITICAL MISS! ***\n";
    std::cout << "You miss completely!\n";

  } else if (totalRoll >= targetRoll) {
    std::cout << "Hit!\n";

    // Calculate damage
    const Item* weapon = player_.getInventory().getEquippedWeapon();
    int damage = 0;

    if (weapon) {
      damage = dice_.roll(weapon->damage);
      std::cout << "Weapon damage: " << weapon->damage << " = " << damage << "\n";
    } else {
      damage = dice_.d4();
      std::cout << "Unarmed damage: 1d4 = " << damage << "\n";
    }

    damage += player_.getDamageBonus();
    if (damage < 1) damage = 1;

    std::cout << "Total damage: " << damage << "\n";
    currentEnemy_.hitPoints -= damage;

  } else {
    std::cout << "Miss!\n";
  }

  // Check if enemy is dead
  if (currentEnemy_.hitPoints <= 0) {
    awardVictory();
    return;
  }

  std::cout << "Enemy HP: " << currentEnemy_.hitPoints << "/"
            << currentEnemy_.maxHitPoints << "\n";

  playerTurn_ = false;
  enemyAttack();
}

void Combat::enemyAttack() {
  if (!inCombat_) return;

  std::cout << "\nThe " << currentEnemy_.name << " attacks!\n";

  // Roll to hit
  int roll = dice_.d20();
  int totalRoll = roll + currentEnemy_.attackBonus;

  std::cout << "Attack roll: " << roll;
  if (currentEnemy_.attackBonus != 0) {
    std::cout << " + " << currentEnemy_.attackBonus << " = " << totalRoll;
  }
  std::cout << " vs AC " << player_.getArmorClass() << "\n";

  int targetRoll = 20 - player_.getArmorClass();

  if (roll == 20) {
    std::cout << "*** CRITICAL HIT! ***\n";
    int damage = dice_.roll(currentEnemy_.attackDamage) * 2;
    player_.takeDamage(damage);

  } else if (roll == 1) {
    std::cout << "The " << currentEnemy_.name << " misses!\n";

  } else if (totalRoll >= targetRoll) {
    std::cout << "The " << currentEnemy_.name << " hits!\n";
    int damage = dice_.roll(currentEnemy_.attackDamage);
    player_.takeDamage(damage);

  } else {
    std::cout << "The " << currentEnemy_.name << " misses!\n";
  }

  // Check if player is dead
  if (!player_.isAlive()) {
    handlePlayerDeath();
    return;
  }

  playerTurn_ = true;
}

void Combat::playerUseItem() {
  if (!inCombat_ || !playerTurn_) return;

  std::cout << "\n=== USE ITEM ===\n";

  // Check for healing potions
  if (player_.getInventory().hasItem("Healing Potion")) {
    Item* potion = player_.getInventory().getItem("Healing Potion");
    if (potion) {
      player_.heal(potion->effectValue);
      player_.getInventory().removeItem("Healing Potion");
      std::cout << "You drink a Healing Potion!\n";

      playerTurn_ = false;
      enemyAttack();
      return;
    }
  }

  std::cout << "You have no usable items!\n";
}

void Combat::playerFlee() {
  if (!inCombat_ || !playerTurn_) return;

  std::cout << "\nYou attempt to flee!\n";

  // 50% chance to flee, modified by dexterity
  int fleeChance = 50 + (player_.getAbilityScores().dexterity - 10) * 5;
  int roll = dice_.d100();

  if (roll <= fleeChance) {
    std::cout << "You successfully escape!\n";
    endCombat();
  } else {
    std::cout << "You fail to escape!\n";
    playerTurn_ = false;
    enemyAttack();
  }
}

void Combat::endCombat() {
  inCombat_ = false;
  std::cout << "\nCombat ended.\n";
}

void Combat::executeCombatRound() {
  if (!inCombat_) return;

  if (playerTurn_) {
    // Player's turn is handled by menu choices
  } else {
    enemyAttack();
  }
}

bool Combat::rollInitiative() {
  // Simple initiative: compare dexterity + d20
  int playerInit = dice_.d20() + (player_.getAbilityScores().dexterity - 10) / 2;
  int enemyInit = dice_.d20();

  std::cout << "Initiative: You roll " << playerInit << ", Enemy rolls "
            << enemyInit << "\n";

  return playerInit >= enemyInit;
}

void Combat::awardVictory() {
  std::cout << "\n========================================\n";
  std::cout << "  Victory!\n";
  std::cout << "========================================\n";
  std::cout << "The " << currentEnemy_.name << " has been defeated!\n";

  // Award XP
  player_.gainExperience(currentEnemy_.experienceValue);

  // Award gold
  int goldFound = dice_.roll("1d" + std::to_string(currentEnemy_.goldDropMax - currentEnemy_.goldDropMin + 1)) + currentEnemy_.goldDropMin - 1;
  if (goldFound > 0) {
    player_.addGold(goldFound);
    std::cout << "You found " << goldFound << " gold pieces!\n";
  }

  endCombat();
}

void Combat::handlePlayerDeath() {
  std::cout << "\n========================================\n";
  std::cout << "  DEFEAT!\n";
  std::cout << "========================================\n";
  std::cout << "You have been slain by the " << currentEnemy_.name << "!\n";
  std::cout << "Your adventure ends here...\n\n";

  endCombat();
}
