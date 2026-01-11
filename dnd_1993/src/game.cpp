#include "game.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

Game::Game() : running_(true), saveFilePath_("saves/savegame.dat") {}

void Game::run() {
  while (running_) {
    mainMenu();
  }
}

void Game::mainMenu() {
  clearScreen();
  displayTitle();

  std::cout << "\n1. New Game\n";
  std::cout << "2. Load Game\n";
  std::cout << "3. Quit\n";
  std::cout << "\nChoice: ";

  int choice = getMenuChoice(1, 3);

  switch (choice) {
    case 1:
      newGame();
      break;
    case 2:
      loadGame();
      break;
    case 3:
      quitGame();
      break;
  }
}

void Game::newGame() {
  clearScreen();
  std::cout << "\n========================================\n";
  std::cout << "     NEW GAME - CHARACTER CREATION\n";
  std::cout << "========================================\n\n";

  createCharacter();

  // Start adventure
  adventure_ = std::make_unique<Adventure>(*player_, dice_);
  adventure_->start();

  // Main game loop
  while (running_ && player_->isAlive()) {
    std::cout << "\n=== ADVENTURE MENU ===\n";
    std::cout << "1. View Character Sheet\n";
    std::cout << "2. View Inventory\n";
    std::cout << "3. Explore Area\n";
    std::cout << "4. Travel to Location\n";
    std::cout << "5. Rest\n";
    std::cout << "6. Visit Shop\n";
    std::cout << "7. Visit Temple\n";
    std::cout << "8. Visit Tavern\n";
    std::cout << "9. Save Game\n";
    std::cout << "10. Return to Main Menu\n";
    std::cout << "\nChoice: ";

    int choice = getMenuChoice(1, 10);

    switch (choice) {
      case 1:
        player_->displayCharacterSheet();
        waitForEnter();
        break;

      case 2:
        player_->getInventory().displayInventory();
        std::cout << "\n1. Equip Item\n2. Back\nChoice: ";
        {
          int invChoice = getMenuChoice(1, 2);
          if (invChoice == 1) {
            std::cout << "Enter item name: ";
            std::string itemName;
            std::cin.ignore();
            std::getline(std::cin, itemName);
            if (player_->getInventory().hasItem(itemName)) {
              Item* item = player_->getInventory().getItem(itemName);
              if (item->type == ItemType::WEAPON) {
                player_->getInventory().equipWeapon(itemName);
              } else if (item->type == ItemType::ARMOR) {
                player_->getInventory().equipArmor(itemName);
              }
            } else {
              std::cout << "Item not found!\n";
            }
          }
        }
        waitForEnter();
        break;

      case 3:
        adventure_->exploreCurrentLocation();
        if (adventure_->combat_.isInCombat()) {
          handleCombat();
        }
        waitForEnter();
        break;

      case 4:
        std::cout << "\n=== TRAVEL ===\n";
        std::cout << "1. Dark Dungeon\n";
        std::cout << "2. Darkwood Forest\n";
        std::cout << "3. Goblin Cave\n";
        std::cout << "4. Ancient Ruins\n";
        std::cout << "5. Return to Town Square\n";
        std::cout << "Choice: ";
        {
          int travelChoice = getMenuChoice(1, 5);
          switch (travelChoice) {
            case 1:
              adventure_->moveToLocation("Dark Dungeon");
              break;
            case 2:
              adventure_->moveToLocation("Darkwood Forest");
              break;
            case 3:
              adventure_->moveToLocation("Goblin Cave");
              break;
            case 4:
              adventure_->moveToLocation("Ancient Ruins");
              break;
            case 5:
              adventure_->moveToLocation("Town Square");
              break;
          }
        }
        waitForEnter();
        break;

      case 5:
        adventure_->rest();
        if (adventure_->combat_.isInCombat()) {
          handleCombat();
        }
        waitForEnter();
        break;

      case 6:
        adventure_->visitShop();
        waitForEnter();
        break;

      case 7:
        adventure_->visitTemple();
        waitForEnter();
        break;

      case 8:
        adventure_->visitTavern();
        waitForEnter();
        break;

      case 9:
        saveGame();
        waitForEnter();
        break;

      case 10:
        return;
    }

    if (!player_->isAlive()) {
      std::cout << "\n*** GAME OVER ***\n";
      std::cout << "Your adventure has ended.\n";
      waitForEnter();
      return;
    }
  }
}

void Game::handleCombat() {
  while (adventure_->combat_.isInCombat() && player_->isAlive()) {
    std::cout << "\n=== COMBAT ===\n";
    std::cout << "Your HP: " << player_->getHitPoints() << "/"
              << player_->getMaxHitPoints() << "\n";
    std::cout << "Enemy: " << adventure_->combat_.getCurrentEnemy().name
              << " (" << adventure_->combat_.getCurrentEnemy().hitPoints
              << " HP)\n\n";

    if (adventure_->combat_.isPlayerTurn()) {
      std::cout << "1. Attack\n";
      std::cout << "2. Use Item\n";
      std::cout << "3. Flee\n";
      std::cout << "Choice: ";

      int choice = getMenuChoice(1, 3);

      switch (choice) {
        case 1:
          adventure_->combat_.playerAttack();
          break;
        case 2:
          adventure_->combat_.playerUseItem();
          break;
        case 3:
          adventure_->combat_.playerFlee();
          break;
      }
    } else {
      // Enemy's turn is handled automatically
    }

    if (adventure_->combat_.isInCombat()) {
      waitForEnter();
    }
  }
}

void Game::loadGame() {
  if (loadFromFile(saveFilePath_)) {
    std::cout << "\nGame loaded successfully!\n";

    // Recreate adventure with loaded character
    adventure_ = std::make_unique<Adventure>(*player_, dice_);
    adventure_->displayCurrentLocation();

    waitForEnter();

    // Continue game loop
    while (running_ && player_->isAlive()) {
      // Same game loop as newGame
      std::cout << "\n=== ADVENTURE MENU ===\n";
      std::cout << "1. View Character Sheet\n";
      std::cout << "2. View Inventory\n";
      std::cout << "3. Explore Area\n";
      std::cout << "4. Travel to Location\n";
      std::cout << "5. Rest\n";
      std::cout << "6. Visit Shop\n";
      std::cout << "7. Visit Temple\n";
      std::cout << "8. Visit Tavern\n";
      std::cout << "9. Save Game\n";
      std::cout << "10. Return to Main Menu\n";
      std::cout << "\nChoice: ";

      int choice = getMenuChoice(1, 10);

      switch (choice) {
        case 1:
          player_->displayCharacterSheet();
          waitForEnter();
          break;
        case 2:
          player_->getInventory().displayInventory();
          waitForEnter();
          break;
        case 3:
          adventure_->exploreCurrentLocation();
          if (adventure_->combat_.isInCombat()) {
            handleCombat();
          }
          waitForEnter();
          break;
        case 4:
          std::cout << "\n=== TRAVEL ===\n";
          std::cout << "1. Dark Dungeon\n2. Darkwood Forest\n3. Goblin Cave\n";
          std::cout << "4. Ancient Ruins\n5. Return to Town Square\n";
          std::cout << "Choice: ";
          {
            int travelChoice = getMenuChoice(1, 5);
            const char* locations[] = {"", "Dark Dungeon", "Darkwood Forest",
                                       "Goblin Cave", "Ancient Ruins", "Town Square"};
            if (travelChoice >= 1 && travelChoice <= 5) {
              adventure_->moveToLocation(locations[travelChoice]);
            }
          }
          waitForEnter();
          break;
        case 5:
          adventure_->rest();
          if (adventure_->combat_.isInCombat()) {
            handleCombat();
          }
          waitForEnter();
          break;
        case 6:
          adventure_->visitShop();
          waitForEnter();
          break;
        case 7:
          adventure_->visitTemple();
          waitForEnter();
          break;
        case 8:
          adventure_->visitTavern();
          waitForEnter();
          break;
        case 9:
          saveGame();
          waitForEnter();
          break;
        case 10:
          return;
      }

      if (!player_->isAlive()) {
        std::cout << "\n*** GAME OVER ***\n";
        waitForEnter();
        return;
      }
    }
  } else {
    std::cout << "\nFailed to load game!\n";
    waitForEnter();
  }
}

void Game::saveGame() {
  if (player_ && saveToFile(saveFilePath_)) {
    std::cout << "\nGame saved successfully!\n";
  } else {
    std::cout << "\nFailed to save game!\n";
  }
}

void Game::quitGame() {
  running_ = false;
  std::cout << "\nThanks for playing!\n";
}

void Game::createCharacter() {
  // Get name
  std::string name = getStringInput("Enter your character's name: ");

  // Roll ability scores
  std::cout << "\nRolling ability scores (4d6, drop lowest)...\n";
  AbilityScores scores;
  scores.strength = dice_.rollAbilityScore4d6DropLowest();
  scores.dexterity = dice_.rollAbilityScore4d6DropLowest();
  scores.constitution = dice_.rollAbilityScore4d6DropLowest();
  scores.intelligence = dice_.rollAbilityScore4d6DropLowest();
  scores.wisdom = dice_.rollAbilityScore4d6DropLowest();
  scores.charisma = dice_.rollAbilityScore4d6DropLowest();

  std::cout << "\nYour ability scores:\n";
  std::cout << "Strength:     " << scores.strength << "\n";
  std::cout << "Dexterity:    " << scores.dexterity << "\n";
  std::cout << "Constitution: " << scores.constitution << "\n";
  std::cout << "Intelligence: " << scores.intelligence << "\n";
  std::cout << "Wisdom:       " << scores.wisdom << "\n";
  std::cout << "Charisma:     " << scores.charisma << "\n";

  // Choose race
  CharacterRace race = chooseRace();

  // Choose class
  CharacterClass cls = chooseClass();

  // Create character
  player_ = std::make_unique<Character>(name, race, cls);
  player_->setAbilityScores(scores);

  // Equip starting gear
  equipStartingGear();

  std::cout << "\nCharacter created!\n";
  player_->displayCharacterSheet();
  waitForEnter();
}

CharacterRace Game::chooseRace() {
  std::cout << "\n=== CHOOSE YOUR RACE ===\n";
  std::cout << "1. Human (No modifiers, +50 gold)\n";
  std::cout << "2. Elf (+1 Dex, -1 Con)\n";
  std::cout << "3. Dwarf (+1 Con, -1 Cha)\n";
  std::cout << "4. Halfling (+1 Dex, -1 Str)\n";
  std::cout << "5. Gnome (+1 Int, -1 Wis)\n";
  std::cout << "6. Half-Elf (No modifiers)\n";
  std::cout << "Choice: ";

  int choice = getMenuChoice(1, 6);

  return static_cast<CharacterRace>(choice - 1);
}

CharacterClass Game::chooseClass() {
  std::cout << "\n=== CHOOSE YOUR CLASS ===\n";
  std::cout << "1. Fighter (Best combat, d10 HP)\n";
  std::cout << "2. Cleric (Healing, d8 HP)\n";
  std::cout << "3. Mage (Magic, d4 HP)\n";
  std::cout << "4. Thief (Stealth, d6 HP)\n";
  std::cout << "5. Ranger (Wilderness, d10 HP)\n";
  std::cout << "6. Paladin (Holy warrior, d10 HP)\n";
  std::cout << "Choice: ";

  int choice = getMenuChoice(1, 6);

  return static_cast<CharacterClass>(choice - 1);
}

void Game::equipStartingGear() {
  // Give basic weapon based on class
  switch (player_->getClass()) {
    case CharacterClass::FIGHTER:
    case CharacterClass::PALADIN:
    case CharacterClass::RANGER:
      player_->getInventory().addItem(
          Inventory::createWeapon(WeaponType::LONGSWORD));
      player_->getInventory().equipWeapon("Longsword");
      break;
    case CharacterClass::CLERIC:
      player_->getInventory().addItem(Inventory::createWeapon(WeaponType::MACE));
      player_->getInventory().equipWeapon("Mace");
      break;
    case CharacterClass::MAGE:
      player_->getInventory().addItem(
          Inventory::createWeapon(WeaponType::QUARTERSTAFF));
      player_->getInventory().equipWeapon("Quarterstaff");
      break;
    case CharacterClass::THIEF:
      player_->getInventory().addItem(Inventory::createWeapon(WeaponType::DAGGER));
      player_->getInventory().equipWeapon("Dagger");
      break;
  }

  // Give basic armor
  player_->getInventory().addItem(Inventory::createArmor(ArmorType::LEATHER));
  player_->getInventory().equipArmor("Leather Armor");

  // Give starting potion
  player_->getInventory().addItem(Inventory::createHealingPotion(10));

  std::cout << "\nYou have been equipped with basic gear!\n";
}

void Game::displayTitle() const {
  std::cout << "\n";
  std::cout << "========================================\n";
  std::cout << "                                        \n";
  std::cout << "    D & D   T E X T   R P G   1 9 9 3  \n";
  std::cout << "                                        \n";
  std::cout << "========================================\n";
}

int Game::getMenuChoice(int min, int max) {
  int choice;
  while (true) {
    std::cin >> choice;
    if (std::cin.fail() || choice < min || choice > max) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid choice. Try again: ";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return choice;
    }
  }
}

std::string Game::getStringInput(const std::string& prompt) {
  std::cout << prompt;
  std::string input;
  std::getline(std::cin, input);
  return input;
}

void Game::clearScreen() const {
  // Simple clear - works on most terminals
  std::cout << "\n\n\n\n\n\n\n\n\n\n";
}

void Game::waitForEnter() const {
  std::cout << "\nPress ENTER to continue...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool Game::saveToFile(const std::string& filename) {
  if (!player_) return false;

  std::ofstream file(filename);
  if (!file.is_open()) return false;

  file << player_->serialize();

  if (adventure_) {
    file << "ADVENTURE_DATA\n";
    file << adventure_->serialize();
  }

  file.close();
  return true;
}

bool Game::loadFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) return false;

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string data = buffer.str();

  file.close();

  // Split into character and adventure data
  size_t adventurePos = data.find("ADVENTURE_DATA\n");
  std::string characterData;
  std::string adventureData;

  if (adventurePos != std::string::npos) {
    characterData = data.substr(0, adventurePos);
    adventureData = data.substr(adventurePos + 15);  // Skip "ADVENTURE_DATA\n"
  } else {
    characterData = data;
  }

  player_ = std::make_unique<Character>(Character::deserialize(characterData));

  return true;
}
