#include "character.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

Character::Character()
    : name_("Unknown"),
      race_(CharacterRace::HUMAN),
      class_(CharacterClass::FIGHTER),
      level_(1),
      experience_(0),
      hitPoints_(10),
      maxHitPoints_(10),
      armorClass_(10),
      gold_(100) {
  abilities_ = {10, 10, 10, 10, 10, 10};
}

Character::Character(const std::string& name, CharacterRace race,
                     CharacterClass cls)
    : name_(name),
      race_(race),
      class_(cls),
      level_(1),
      experience_(0),
      armorClass_(10),
      gold_(100) {
  abilities_ = {10, 10, 10, 10, 10, 10};
  initializeByClass();
  applyRacialModifiers();
  maxHitPoints_ = calculateHitPoints();
  hitPoints_ = maxHitPoints_;
  armorClass_ = calculateBaseAC();
}

void Character::setAbilityScores(const AbilityScores& scores) {
  abilities_ = scores;
  applyRacialModifiers();
  maxHitPoints_ = calculateHitPoints();
  hitPoints_ = maxHitPoints_;
}

void Character::takeDamage(int damage) {
  hitPoints_ -= damage;
  if (hitPoints_ < 0) hitPoints_ = 0;

  std::cout << name_ << " takes " << damage << " damage! ("
            << hitPoints_ << "/" << maxHitPoints_ << " HP)\n";
}

void Character::heal(int amount) {
  hitPoints_ += amount;
  if (hitPoints_ > maxHitPoints_) hitPoints_ = maxHitPoints_;

  std::cout << name_ << " heals " << amount << " HP! ("
            << hitPoints_ << "/" << maxHitPoints_ << " HP)\n";
}

int Character::getAttackBonus() const {
  int bonus = (abilities_.strength - 10) / 2;  // Strength modifier

  // Class-based attack bonus
  switch (class_) {
    case CharacterClass::FIGHTER:
    case CharacterClass::PALADIN:
    case CharacterClass::RANGER:
      bonus += level_;
      break;
    case CharacterClass::CLERIC:
      bonus += level_ * 3 / 4;
      break;
    case CharacterClass::THIEF:
      bonus += level_ / 2;
      break;
    case CharacterClass::MAGE:
      bonus += level_ / 3;
      break;
  }

  return bonus;
}

int Character::getDamageBonus() const {
  return (abilities_.strength - 10) / 2;
}

int Character::getACModifier() const {
  return (abilities_.dexterity - 10) / 2;
}

void Character::gainExperience(int xp) {
  experience_ += xp;
  std::cout << "You gain " << xp << " experience points!\n";

  while (experience_ >= getNextLevelXP()) {
    levelUp();
  }
}

void Character::levelUp() {
  level_++;

  // Increase HP based on class
  int hpGain = 0;
  switch (class_) {
    case CharacterClass::FIGHTER:
    case CharacterClass::PALADIN:
    case CharacterClass::RANGER:
      hpGain = 10;  // d10
      break;
    case CharacterClass::CLERIC:
      hpGain = 8;   // d8
      break;
    case CharacterClass::THIEF:
      hpGain = 6;   // d6
      break;
    case CharacterClass::MAGE:
      hpGain = 4;   // d4
      break;
  }

  // Add Constitution modifier
  hpGain += (abilities_.constitution - 10) / 2;
  if (hpGain < 1) hpGain = 1;

  maxHitPoints_ += hpGain;
  hitPoints_ = maxHitPoints_;

  std::cout << "\n*** LEVEL UP! ***\n";
  std::cout << "You are now level " << level_ << "!\n";
  std::cout << "HP increased by " << hpGain << " to " << maxHitPoints_ << "\n";
  std::cout << "Attack bonus increased!\n\n";
}

int Character::getNextLevelXP() const {
  // AD&D 2nd Edition XP progression
  switch (class_) {
    case CharacterClass::FIGHTER:
    case CharacterClass::PALADIN:
    case CharacterClass::RANGER:
      return level_ * 2000;
    case CharacterClass::CLERIC:
      return level_ * 1500;
    case CharacterClass::THIEF:
      return level_ * 1250;
    case CharacterClass::MAGE:
      return level_ * 2500;
  }
  return level_ * 2000;
}

void Character::rest() {
  std::cout << "\nYou rest and recover your wounds...\n";
  hitPoints_ = maxHitPoints_;
  std::cout << "HP fully restored to " << maxHitPoints_ << "!\n";
}

void Character::displayCharacterSheet() const {
  std::cout << "\n========================================\n";
  std::cout << "        CHARACTER SHEET\n";
  std::cout << "========================================\n";
  std::cout << "Name:        " << name_ << "\n";
  std::cout << "Race:        " << getRaceName() << "\n";
  std::cout << "Class:       " << getClassName() << "\n";
  std::cout << "Level:       " << level_ << "\n";
  std::cout << "Experience:  " << experience_ << " / " << getNextLevelXP() << "\n";
  std::cout << "----------------------------------------\n";
  std::cout << "Hit Points:  " << hitPoints_ << " / " << maxHitPoints_ << "\n";
  std::cout << "Armor Class: " << armorClass_ << "\n";
  std::cout << "Gold:        " << gold_ << " GP\n";
  std::cout << "----------------------------------------\n";
  std::cout << "ABILITY SCORES:\n";
  std::cout << "  Strength:     " << std::setw(2) << abilities_.strength
            << " (+" << (abilities_.strength - 10) / 2 << ")\n";
  std::cout << "  Dexterity:    " << std::setw(2) << abilities_.dexterity
            << " (+" << (abilities_.dexterity - 10) / 2 << ")\n";
  std::cout << "  Constitution: " << std::setw(2) << abilities_.constitution
            << " (+" << (abilities_.constitution - 10) / 2 << ")\n";
  std::cout << "  Intelligence: " << std::setw(2) << abilities_.intelligence
            << " (+" << (abilities_.intelligence - 10) / 2 << ")\n";
  std::cout << "  Wisdom:       " << std::setw(2) << abilities_.wisdom
            << " (+" << (abilities_.wisdom - 10) / 2 << ")\n";
  std::cout << "  Charisma:     " << std::setw(2) << abilities_.charisma
            << " (+" << (abilities_.charisma - 10) / 2 << ")\n";
  std::cout << "----------------------------------------\n";
  std::cout << "Attack Bonus: +" << getAttackBonus() << "\n";
  std::cout << "Damage Bonus: +" << getDamageBonus() << "\n";
  std::cout << "========================================\n";
}

std::string Character::getRaceName() const {
  switch (race_) {
    case CharacterRace::HUMAN: return "Human";
    case CharacterRace::ELF: return "Elf";
    case CharacterRace::DWARF: return "Dwarf";
    case CharacterRace::HALFLING: return "Halfling";
    case CharacterRace::GNOME: return "Gnome";
    case CharacterRace::HALF_ELF: return "Half-Elf";
  }
  return "Unknown";
}

std::string Character::getClassName() const {
  switch (class_) {
    case CharacterClass::FIGHTER: return "Fighter";
    case CharacterClass::CLERIC: return "Cleric";
    case CharacterClass::MAGE: return "Mage";
    case CharacterClass::THIEF: return "Thief";
    case CharacterClass::RANGER: return "Ranger";
    case CharacterClass::PALADIN: return "Paladin";
  }
  return "Unknown";
}

int Character::getSavingThrow(const std::string& saveType) const {
  int baseSave = 16 - level_;  // Base saving throw improves with level

  if (saveType == "Death" || saveType == "Poison") {
    baseSave -= (abilities_.constitution - 10) / 2;
  } else if (saveType == "Wands" || saveType == "Breath") {
    baseSave -= (abilities_.dexterity - 10) / 2;
  } else if (saveType == "Spells") {
    baseSave -= (abilities_.wisdom - 10) / 2;
  }

  return std::max(2, baseSave);  // Minimum save is 2
}

std::string Character::serialize() const {
  std::ostringstream oss;
  oss << name_ << "\n";
  oss << static_cast<int>(race_) << "\n";
  oss << static_cast<int>(class_) << "\n";
  oss << level_ << "\n";
  oss << experience_ << "\n";
  oss << hitPoints_ << "\n";
  oss << maxHitPoints_ << "\n";
  oss << armorClass_ << "\n";
  oss << gold_ << "\n";
  oss << abilities_.strength << " " << abilities_.dexterity << " "
      << abilities_.constitution << " " << abilities_.intelligence << " "
      << abilities_.wisdom << " " << abilities_.charisma << "\n";
  oss << inventory_.serialize();
  return oss.str();
}

Character Character::deserialize(const std::string& data) {
  Character character;
  std::istringstream iss(data);

  std::getline(iss, character.name_);

  int raceInt, classInt;
  iss >> raceInt >> classInt;
  character.race_ = static_cast<CharacterRace>(raceInt);
  character.class_ = static_cast<CharacterClass>(classInt);

  iss >> character.level_ >> character.experience_
      >> character.hitPoints_ >> character.maxHitPoints_
      >> character.armorClass_ >> character.gold_;

  iss >> character.abilities_.strength >> character.abilities_.dexterity
      >> character.abilities_.constitution >> character.abilities_.intelligence
      >> character.abilities_.wisdom >> character.abilities_.charisma;

  iss.ignore();  // Skip newline

  std::string inventoryData;
  std::string line;
  while (std::getline(iss, line)) {
    inventoryData += line + "\n";
  }

  character.inventory_ = Inventory::deserialize(inventoryData);

  return character;
}

void Character::initializeByClass() {
  // Set initial gold and HP based on class
  switch (class_) {
    case CharacterClass::FIGHTER:
    case CharacterClass::PALADIN:
    case CharacterClass::RANGER:
      gold_ = 150;
      break;
    case CharacterClass::CLERIC:
      gold_ = 120;
      break;
    case CharacterClass::THIEF:
      gold_ = 100;
      break;
    case CharacterClass::MAGE:
      gold_ = 80;
      break;
  }
}

int Character::calculateHitPoints() {
  int hp = 0;

  // First level HP
  switch (class_) {
    case CharacterClass::FIGHTER:
    case CharacterClass::PALADIN:
    case CharacterClass::RANGER:
      hp = 10;  // d10
      break;
    case CharacterClass::CLERIC:
      hp = 8;   // d8
      break;
    case CharacterClass::THIEF:
      hp = 6;   // d6
      break;
    case CharacterClass::MAGE:
      hp = 4;   // d4
      break;
  }

  // Add Constitution modifier
  hp += (abilities_.constitution - 10) / 2;

  if (hp < 1) hp = 1;

  return hp;
}

int Character::calculateBaseAC() {
  int ac = 10;  // Base AC

  // Dexterity modifier (lowers AC)
  ac -= (abilities_.dexterity - 10) / 2;

  // Equipment modifiers applied elsewhere

  return ac;
}

void Character::applyRacialModifiers() {
  switch (race_) {
    case CharacterRace::ELF:
      abilities_.dexterity += 1;
      abilities_.constitution -= 1;
      break;
    case CharacterRace::DWARF:
      abilities_.constitution += 1;
      abilities_.charisma -= 1;
      break;
    case CharacterRace::HALFLING:
      abilities_.dexterity += 1;
      abilities_.strength -= 1;
      break;
    case CharacterRace::GNOME:
      abilities_.intelligence += 1;
      abilities_.wisdom -= 1;
      break;
    case CharacterRace::HALF_ELF:
      // No modifiers, but versatile
      break;
    case CharacterRace::HUMAN:
      // No modifiers, but extra starting gold
      gold_ += 50;
      break;
  }
}
