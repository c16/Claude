#include "adventure.h"

#include <iostream>
#include <sstream>

Adventure::Adventure(Character& player, Dice& dice)
    : player_(player),
      dice_(dice),
      combat_(player, dice),
      currentLocation_("Town Square"),
      questProgress_(0),
      explorationCount_(0) {
  initializeLocations();
}

void Adventure::start() {
  displayWelcomeMessage();
  displayCurrentLocation();
}

void Adventure::exploreCurrentLocation() {
  Location& loc = getCurrentLocation();
  explorationCount_++;

  std::cout << "\nYou explore the " << loc.name << "...\n";

  // Chance of random encounter based on location
  if (loc.hasMonsters && dice_.d100() <= 40) {  // 40% chance
    triggerRandomEncounter();
  } else if (loc.hasTreasure && dice_.d100() <= 30) {  // 30% chance
    searchForTreasure();
  } else {
    std::cout << "You find nothing of interest.\n";
  }
}

void Adventure::moveToLocation(const std::string& locationName) {
  if (locations_.find(locationName) != locations_.end()) {
    currentLocation_ = locationName;
    std::cout << "\nYou travel to " << locationName << ".\n";
    displayCurrentLocation();
  } else {
    std::cout << "You cannot go there.\n";
  }
}

void Adventure::displayCurrentLocation() const {
  auto it = locations_.find(currentLocation_);
  if (it != locations_.end()) {
    const Location& loc = it->second;
    std::cout << "\n========================================\n";
    std::cout << "  " << loc.name << "\n";
    std::cout << "========================================\n";
    std::cout << loc.description << "\n";
    std::cout << "========================================\n";
  }
}

void Adventure::rest() {
  std::cout << "\nYou find a safe place to rest...\n";

  // Small chance of being attacked while resting in dangerous areas
  Location& loc = getCurrentLocation();
  if (loc.hasMonsters && dice_.d100() <= 20) {
    std::cout << "But you are ambushed!\n";
    triggerRandomEncounter();
  } else {
    player_.rest();
  }
}

void Adventure::searchForTreasure() {
  std::cout << "\nYou search for treasure...\n";

  int roll = dice_.d100();

  if (roll <= 40) {
    int gold = dice_.d20() + dice_.d20();
    player_.addGold(gold);
    std::cout << "You find " << gold << " gold pieces!\n";
  } else if (roll <= 60) {
    std::cout << "You find a Healing Potion!\n";
    player_.getInventory().addItem(Inventory::createHealingPotion(15));
  } else if (roll <= 70 && player_.getLevel() >= 3) {
    std::cout << "You find a magic weapon!\n";
    Item magicSword("Magic Longsword", ItemType::WEAPON, 200, "A sword with a +1 enchantment");
    magicSword.damage = "1d8+1";
    magicSword.attackBonus = 1;
    player_.getInventory().addItem(magicSword);
  } else {
    std::cout << "You find nothing of value.\n";
  }
}

void Adventure::talkToNPC() {
  std::cout << "\n=== TALK TO NPC ===\n";

  if (currentLocation_ == "Town Square") {
    std::cout << "Guard: \"Beware the dungeon to the east! Many adventurers\n";
    std::cout << "        have entered, but few have returned.\"\n";
  } else if (currentLocation_ == "Tavern") {
    std::cout << "Barkeep: \"I've heard rumors of a dragon in the mountains.\n";
    std::cout << "          The beast guards a vast treasure hoard!\"\n";
  } else if (currentLocation_ == "Temple") {
    std::cout << "Priest: \"May the gods watch over you, brave adventurer.\n";
    std::cout << "         The world grows darker with each passing day.\"\n";
  } else {
    std::cout << "There is no one here to talk to.\n";
  }
}

void Adventure::enterCombat() {
  triggerRandomEncounter();
}

void Adventure::visitShop() {
  if (currentLocation_ != "Town Square") {
    std::cout << "There is no shop here.\n";
    return;
  }

  std::cout << "\n=== WEAPON SHOP ===\n";
  std::cout << "1. Longsword (15 GP) - 1d8 damage\n";
  std::cout << "2. Shortsword (10 GP) - 1d6 damage\n";
  std::cout << "3. Battle Axe (10 GP) - 1d8 damage\n";
  std::cout << "4. Leather Armor (5 GP) - AC 8\n";
  std::cout << "5. Chainmail (75 GP) - AC 5\n";
  std::cout << "6. Healing Potion (50 GP) - Restores 15 HP\n";
  std::cout << "7. Leave\n";
  std::cout << "\nYour gold: " << player_.getGold() << " GP\n";
  std::cout << "What would you like to buy? ";

  int choice;
  std::cin >> choice;

  switch (choice) {
    case 1:
      if (player_.getGold() >= 15) {
        player_.spendGold(15);
        player_.getInventory().addItem(Inventory::createWeapon(WeaponType::LONGSWORD));
        std::cout << "You purchased a Longsword!\n";
      } else {
        std::cout << "Not enough gold!\n";
      }
      break;
    case 2:
      if (player_.getGold() >= 10) {
        player_.spendGold(10);
        player_.getInventory().addItem(Inventory::createWeapon(WeaponType::SHORTSWORD));
        std::cout << "You purchased a Shortsword!\n";
      } else {
        std::cout << "Not enough gold!\n";
      }
      break;
    case 3:
      if (player_.getGold() >= 10) {
        player_.spendGold(10);
        player_.getInventory().addItem(Inventory::createWeapon(WeaponType::BATTLE_AXE));
        std::cout << "You purchased a Battle Axe!\n";
      } else {
        std::cout << "Not enough gold!\n";
      }
      break;
    case 4:
      if (player_.getGold() >= 5) {
        player_.spendGold(5);
        player_.getInventory().addItem(Inventory::createArmor(ArmorType::LEATHER));
        std::cout << "You purchased Leather Armor!\n";
      } else {
        std::cout << "Not enough gold!\n";
      }
      break;
    case 5:
      if (player_.getGold() >= 75) {
        player_.spendGold(75);
        player_.getInventory().addItem(Inventory::createArmor(ArmorType::CHAINMAIL));
        std::cout << "You purchased Chainmail!\n";
      } else {
        std::cout << "Not enough gold!\n";
      }
      break;
    case 6:
      if (player_.getGold() >= 50) {
        player_.spendGold(50);
        player_.getInventory().addItem(Inventory::createHealingPotion(15));
        std::cout << "You purchased a Healing Potion!\n";
      } else {
        std::cout << "Not enough gold!\n";
      }
      break;
    default:
      std::cout << "Leaving shop.\n";
      break;
  }
}

void Adventure::visitTemple() {
  if (currentLocation_ != "Town Square") {
    std::cout << "There is no temple here.\n";
    return;
  }

  std::cout << "\n=== TEMPLE ===\n";
  std::cout << "The priest offers healing for 20 gold pieces.\n";
  std::cout << "Your HP: " << player_.getHitPoints() << "/" << player_.getMaxHitPoints() << "\n";
  std::cout << "Your gold: " << player_.getGold() << " GP\n";
  std::cout << "\nPay for healing? (1=Yes, 2=No): ";

  int choice;
  std::cin >> choice;

  if (choice == 1) {
    if (player_.getGold() >= 20) {
      player_.spendGold(20);
      player_.rest();
      std::cout << "The priest heals your wounds!\n";
    } else {
      std::cout << "Not enough gold!\n";
    }
  }
}

void Adventure::visitTavern() {
  if (currentLocation_ != "Town Square") {
    std::cout << "There is no tavern here.\n";
    return;
  }

  std::cout << "\n=== THE PRANCING PONY TAVERN ===\n";
  std::cout << "You enter a warm, bustling tavern.\n";
  std::cout << "The barkeep nods at you.\n";
  std::cout << "\n1. Buy a meal (5 GP) - Restore 5 HP\n";
  std::cout << "2. Listen to rumors\n";
  std::cout << "3. Leave\n";
  std::cout << "Choice: ";

  int choice;
  std::cin >> choice;

  switch (choice) {
    case 1:
      if (player_.getGold() >= 5) {
        player_.spendGold(5);
        player_.heal(5);
        std::cout << "You enjoy a hearty meal!\n";
      } else {
        std::cout << "Not enough gold!\n";
      }
      break;
    case 2:
      std::cout << "\nYou overhear patrons talking:\n";
      std::cout << "\"Did you hear? Goblins have been raiding the eastern road!\"\n";
      std::cout << "\"Aye, and there's talk of treasure in the old ruins...\"\n";
      break;
    default:
      break;
  }
}

void Adventure::advanceQuest() {
  questProgress_++;
  std::cout << "\nQuest progress: " << questProgress_ << "\n";
}

void Adventure::triggerRandomEncounter() {
  std::cout << "\n*** RANDOM ENCOUNTER! ***\n";

  Monster monster = generateRandomMonster();
  combat_.startCombat(monster);

  // Combat loop is handled by game.cpp
}

Monster Adventure::generateRandomMonster() {
  // Generate appropriate monster based on player level and location
  int roll = dice_.d100();
  MonsterType type;

  if (player_.getLevel() == 1) {
    if (roll <= 40) {
      type = MonsterType::KOBOLD;
    } else if (roll <= 80) {
      type = MonsterType::GOBLIN;
    } else {
      type = MonsterType::GIANT_RAT;
    }
  } else if (player_.getLevel() <= 3) {
    if (roll <= 30) {
      type = MonsterType::GOBLIN;
    } else if (roll <= 60) {
      type = MonsterType::ORC;
    } else if (roll <= 85) {
      type = MonsterType::SKELETON;
    } else {
      type = MonsterType::WOLF;
    }
  } else if (player_.getLevel() <= 6) {
    if (roll <= 25) {
      type = MonsterType::ORC;
    } else if (roll <= 50) {
      type = MonsterType::SKELETON;
    } else if (roll <= 75) {
      type = MonsterType::ZOMBIE;
    } else {
      type = MonsterType::OGRE;
    }
  } else {
    if (roll <= 20) {
      type = MonsterType::OGRE;
    } else if (roll <= 50) {
      type = MonsterType::TROLL;
    } else {
      type = MonsterType::DRAGON;
    }
  }

  return Monster::create(type, dice_);
}

void Adventure::initializeLocations() {
  Location townSquare;
  townSquare.name = "Town Square";
  townSquare.type = LocationType::TOWN;
  townSquare.description = "A bustling town square with shops and townsfolk.\nSafe haven for weary adventurers.";
  townSquare.hasMonsters = false;
  townSquare.hasNPC = true;
  townSquare.hasTreasure = false;
  locations_["Town Square"] = townSquare;

  Location dungeon;
  dungeon.name = "Dark Dungeon";
  dungeon.type = LocationType::DUNGEON;
  dungeon.description = "A dark, foreboding dungeon filled with dangers.\nThe walls drip with moisture and echo with strange sounds.";
  dungeon.hasMonsters = true;
  dungeon.hasNPC = false;
  dungeon.hasTreasure = true;
  locations_["Dark Dungeon"] = dungeon;

  Location forest;
  forest.name = "Darkwood Forest";
  forest.type = LocationType::FOREST;
  forest.description = "An ancient forest where sunlight barely penetrates.\nStrange creatures lurk among the trees.";
  forest.hasMonsters = true;
  forest.hasNPC = false;
  forest.hasTreasure = true;
  locations_["Darkwood Forest"] = forest;

  Location cave;
  cave.name = "Goblin Cave";
  cave.type = LocationType::CAVE;
  cave.description = "A dank cave system inhabited by goblins.\nThe stench is overwhelming.";
  cave.hasMonsters = true;
  cave.hasNPC = false;
  cave.hasTreasure = true;
  locations_["Goblin Cave"] = cave;

  Location ruins;
  ruins.name = "Ancient Ruins";
  ruins.type = LocationType::RUINS;
  ruins.description = "Crumbling stone structures from a forgotten age.\nMagic still lingers in the air.";
  ruins.hasMonsters = true;
  ruins.hasNPC = false;
  ruins.hasTreasure = true;
  locations_["Ancient Ruins"] = ruins;
}

void Adventure::displayWelcomeMessage() const {
  std::cout << "\n";
  std::cout << "========================================\n";
  std::cout << "    WELCOME TO YOUR ADVENTURE!\n";
  std::cout << "========================================\n";
  std::cout << "\nYour quest begins in a small town.\n";
  std::cout << "Darkness threatens the land, and only\n";
  std::cout << "brave adventurers like you can stop it.\n\n";
  std::cout << "Seek glory, treasure, and power!\n";
  std::cout << "========================================\n";
}

Location& Adventure::getCurrentLocation() {
  return locations_[currentLocation_];
}

std::string Adventure::serialize() const {
  std::ostringstream oss;
  oss << currentLocation_ << "\n";
  oss << questProgress_ << "\n";
  oss << explorationCount_ << "\n";
  return oss.str();
}

void Adventure::deserialize(const std::string& data) {
  std::istringstream iss(data);
  std::getline(iss, currentLocation_);
  iss >> questProgress_ >> explorationCount_;
}
