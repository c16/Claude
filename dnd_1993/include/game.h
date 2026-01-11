#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>

#include "adventure.h"
#include "character.h"
#include "dice.h"

class Game {
 public:
  Game();

  // Game flow
  void run();
  void mainMenu();
  void newGame();
  void loadGame();
  void saveGame();
  void quitGame();

  // Character creation
  void createCharacter();
  void rollAbilityScores();
  CharacterRace chooseRace();
  CharacterClass chooseClass();
  void equipStartingGear();

  // Combat handler
  void handleCombat();

  // Game state
  bool isRunning() const { return running_; }

 private:
  Dice dice_;
  std::unique_ptr<Character> player_;
  std::unique_ptr<Adventure> adventure_;
  bool running_;
  std::string saveFilePath_;

  // Helper methods
  void displayTitle() const;
  int getMenuChoice(int min, int max);
  std::string getStringInput(const std::string& prompt);
  void clearScreen() const;
  void waitForEnter() const;

  // Save/Load helpers
  bool saveToFile(const std::string& filename);
  bool loadFromFile(const std::string& filename);
};

#endif  // GAME_H
