#ifndef CHARACTER_H
#define CHARACTER_H

#include <map>
#include <string>
#include <vector>

#include "inventory.h"

enum class CharacterRace {
  HUMAN,
  ELF,
  DWARF,
  HALFLING,
  GNOME,
  HALF_ELF
};

enum class CharacterClass {
  FIGHTER,
  CLERIC,
  MAGE,
  THIEF,
  RANGER,
  PALADIN
};

struct AbilityScores {
  int strength;
  int dexterity;
  int constitution;
  int intelligence;
  int wisdom;
  int charisma;
};

class Character {
 public:
  Character();
  Character(const std::string& name, CharacterRace race, CharacterClass cls);

  // Getters
  std::string getName() const { return name_; }
  CharacterRace getRace() const { return race_; }
  CharacterClass getClass() const { return class_; }
  int getLevel() const { return level_; }
  int getExperience() const { return experience_; }
  int getHitPoints() const { return hitPoints_; }
  int getMaxHitPoints() const { return maxHitPoints_; }
  int getArmorClass() const { return armorClass_; }
  int getGold() const { return gold_; }

  const AbilityScores& getAbilityScores() const { return abilities_; }
  Inventory& getInventory() { return inventory_; }

  // Setters
  void setName(const std::string& name) { name_ = name; }
  void setRace(CharacterRace race) { race_ = race; }
  void setClass(CharacterClass cls) { class_ = cls; }
  void setAbilityScores(const AbilityScores& scores);

  // Combat
  void takeDamage(int damage);
  void heal(int amount);
  int getAttackBonus() const;
  int getDamageBonus() const;
  int getACModifier() const;

  // Experience and leveling
  void gainExperience(int xp);
  void levelUp();
  int getNextLevelXP() const;

  // Status
  bool isAlive() const { return hitPoints_ > 0; }
  bool isConscious() const { return hitPoints_ > 0; }

  // Rest
  void rest();

  // Gold
  void addGold(int amount) { gold_ += amount; }
  void spendGold(int amount) { gold_ -= amount; }

  // Display
  void displayCharacterSheet() const;
  std::string getRaceName() const;
  std::string getClassName() const;

  // Saving throws (AD&D 2nd Edition style)
  int getSavingThrow(const std::string& saveType) const;

  // Serialization
  std::string serialize() const;
  static Character deserialize(const std::string& data);

 private:
  std::string name_;
  CharacterRace race_;
  CharacterClass class_;
  int level_;
  int experience_;
  int hitPoints_;
  int maxHitPoints_;
  int armorClass_;  // Lower is better in AD&D
  int gold_;

  AbilityScores abilities_;
  Inventory inventory_;

  // Helper methods
  void initializeByClass();
  int calculateHitPoints();
  int calculateBaseAC();
  void applyRacialModifiers();
};

#endif  // CHARACTER_H
