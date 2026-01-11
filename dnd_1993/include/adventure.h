#ifndef ADVENTURE_H
#define ADVENTURE_H

#include <map>
#include <string>
#include <vector>

#include "character.h"
#include "combat.h"
#include "dice.h"

enum class LocationType {
  TOWN,
  DUNGEON,
  FOREST,
  CAVE,
  RUINS,
  TAVERN,
  TEMPLE,
  SHOP
};

struct Location {
  std::string name;
  std::string description;
  LocationType type;
  std::vector<std::string> availableActions;
  bool hasMonsters;
  bool hasNPC;
  bool hasTreasure;
};

class Adventure {
 public:
  Adventure(Character& player, Dice& dice);

  // Adventure flow
  void start();
  void exploreCurrentLocation();
  void moveToLocation(const std::string& locationName);
  void displayCurrentLocation() const;

  // Actions
  void rest();
  void searchForTreasure();
  void talkToNPC();
  void enterCombat();
  void visitShop();
  void visitTemple();
  void visitTavern();

  // Story progression
  int getQuestProgress() const { return questProgress_; }
  void advanceQuest();

  // Random encounters
  void triggerRandomEncounter();

  // Serialization
  std::string serialize() const;
  void deserialize(const std::string& data);

 private:
  Character& player_;
  Dice& dice_;
  Combat combat_;

  std::string currentLocation_;
  std::map<std::string, Location> locations_;
  int questProgress_;
  int explorationCount_;

  // Helpers
  void initializeLocations();
  void displayWelcomeMessage() const;
  Location& getCurrentLocation();
  Monster generateRandomMonster();
};

#endif  // ADVENTURE_H
