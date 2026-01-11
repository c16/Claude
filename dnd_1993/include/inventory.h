#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <string>
#include <vector>

enum class ItemType {
  WEAPON,
  ARMOR,
  POTION,
  SCROLL,
  MISC
};

enum class WeaponType {
  LONGSWORD,
  SHORTSWORD,
  DAGGER,
  MACE,
  WAR_HAMMER,
  BATTLE_AXE,
  LONGBOW,
  SHORTBOW,
  QUARTERSTAFF
};

enum class ArmorType {
  LEATHER,
  CHAINMAIL,
  PLATE_MAIL,
  SHIELD
};

struct Item {
  std::string name;
  ItemType type;
  int value;  // in gold pieces
  std::string description;

  // Weapon properties
  std::string damage;  // e.g., "1d8"
  int attackBonus;

  // Armor properties
  int armorClass;

  // Potion/Scroll properties
  std::string effect;
  int effectValue;

  Item();
  Item(const std::string& n, ItemType t, int v, const std::string& desc);
};

class Inventory {
 public:
  Inventory();

  // Item management
  void addItem(const Item& item);
  bool removeItem(const std::string& itemName);
  bool hasItem(const std::string& itemName) const;
  Item* getItem(const std::string& itemName);

  // Equipment
  void equipWeapon(const std::string& weaponName);
  void equipArmor(const std::string& armorName);
  void unequipWeapon();
  void unequipArmor();

  const Item* getEquippedWeapon() const { return equippedWeapon_; }
  const Item* getEquippedArmor() const { return equippedArmor_; }

  // Display
  void displayInventory() const;
  std::vector<Item> getItems() const { return items_; }

  // Serialization
  std::string serialize() const;
  static Inventory deserialize(const std::string& data);

  // Static item creators
  static Item createWeapon(WeaponType type);
  static Item createArmor(ArmorType type);
  static Item createHealingPotion(int healAmount = 10);

 private:
  std::vector<Item> items_;
  Item* equippedWeapon_;
  Item* equippedArmor_;
};

#endif  // INVENTORY_H
