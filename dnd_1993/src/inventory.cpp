#include "inventory.h"

#include <algorithm>
#include <iostream>
#include <sstream>

Item::Item()
    : name(""),
      type(ItemType::MISC),
      value(0),
      description(""),
      damage(""),
      attackBonus(0),
      armorClass(10),
      effect(""),
      effectValue(0) {}

Item::Item(const std::string& n, ItemType t, int v, const std::string& desc)
    : name(n),
      type(t),
      value(v),
      description(desc),
      damage(""),
      attackBonus(0),
      armorClass(10),
      effect(""),
      effectValue(0) {}

Inventory::Inventory() : equippedWeapon_(nullptr), equippedArmor_(nullptr) {}

void Inventory::addItem(const Item& item) { items_.push_back(item); }

bool Inventory::removeItem(const std::string& itemName) {
  auto it = std::find_if(items_.begin(), items_.end(),
                         [&](const Item& item) { return item.name == itemName; });

  if (it != items_.end()) {
    if (equippedWeapon_ && equippedWeapon_->name == itemName) {
      equippedWeapon_ = nullptr;
    }
    if (equippedArmor_ && equippedArmor_->name == itemName) {
      equippedArmor_ = nullptr;
    }
    items_.erase(it);
    return true;
  }
  return false;
}

bool Inventory::hasItem(const std::string& itemName) const {
  return std::any_of(items_.begin(), items_.end(),
                     [&](const Item& item) { return item.name == itemName; });
}

Item* Inventory::getItem(const std::string& itemName) {
  auto it = std::find_if(items_.begin(), items_.end(),
                         [&](const Item& item) { return item.name == itemName; });

  return (it != items_.end()) ? &(*it) : nullptr;
}

void Inventory::equipWeapon(const std::string& weaponName) {
  Item* item = getItem(weaponName);
  if (item && item->type == ItemType::WEAPON) {
    equippedWeapon_ = item;
    std::cout << "Equipped " << weaponName << "\n";
  }
}

void Inventory::equipArmor(const std::string& armorName) {
  Item* item = getItem(armorName);
  if (item && item->type == ItemType::ARMOR) {
    equippedArmor_ = item;
    std::cout << "Equipped " << armorName << "\n";
  }
}

void Inventory::unequipWeapon() {
  if (equippedWeapon_) {
    std::cout << "Unequipped " << equippedWeapon_->name << "\n";
    equippedWeapon_ = nullptr;
  }
}

void Inventory::unequipArmor() {
  if (equippedArmor_) {
    std::cout << "Unequipped " << equippedArmor_->name << "\n";
    equippedArmor_ = nullptr;
  }
}

void Inventory::displayInventory() const {
  std::cout << "\n=== INVENTORY ===\n";

  if (items_.empty()) {
    std::cout << "Empty\n";
    return;
  }

  for (const auto& item : items_) {
    std::cout << "- " << item.name << " (" << item.value << " GP)";
    if (equippedWeapon_ && equippedWeapon_->name == item.name) {
      std::cout << " [EQUIPPED]";
    }
    if (equippedArmor_ && equippedArmor_->name == item.name) {
      std::cout << " [EQUIPPED]";
    }
    std::cout << "\n  " << item.description << "\n";
  }
}

Item Inventory::createWeapon(WeaponType type) {
  Item weapon;
  weapon.type = ItemType::WEAPON;

  switch (type) {
    case WeaponType::LONGSWORD:
      weapon.name = "Longsword";
      weapon.damage = "1d8";
      weapon.attackBonus = 0;
      weapon.value = 15;
      weapon.description = "A versatile sword favored by many warriors";
      break;
    case WeaponType::SHORTSWORD:
      weapon.name = "Shortsword";
      weapon.damage = "1d6";
      weapon.attackBonus = 0;
      weapon.value = 10;
      weapon.description = "A lightweight blade good for quick strikes";
      break;
    case WeaponType::DAGGER:
      weapon.name = "Dagger";
      weapon.damage = "1d4";
      weapon.attackBonus = 0;
      weapon.value = 2;
      weapon.description = "A small blade, easily concealed";
      break;
    case WeaponType::MACE:
      weapon.name = "Mace";
      weapon.damage = "1d6";
      weapon.attackBonus = 0;
      weapon.value = 8;
      weapon.description = "A blunt weapon good against armor";
      break;
    case WeaponType::BATTLE_AXE:
      weapon.name = "Battle Axe";
      weapon.damage = "1d8";
      weapon.attackBonus = 0;
      weapon.value = 10;
      weapon.description = "A fearsome chopping weapon";
      break;
    case WeaponType::QUARTERSTAFF:
      weapon.name = "Quarterstaff";
      weapon.damage = "1d6";
      weapon.attackBonus = 0;
      weapon.value = 1;
      weapon.description = "A simple wooden staff";
      break;
    default:
      weapon.name = "Club";
      weapon.damage = "1d4";
      weapon.attackBonus = 0;
      weapon.value = 0;
      weapon.description = "A basic wooden club";
  }

  return weapon;
}

Item Inventory::createArmor(ArmorType type) {
  Item armor;
  armor.type = ItemType::ARMOR;

  switch (type) {
    case ArmorType::LEATHER:
      armor.name = "Leather Armor";
      armor.armorClass = 8;
      armor.value = 5;
      armor.description = "Light armor made of hardened leather";
      break;
    case ArmorType::CHAINMAIL:
      armor.name = "Chainmail";
      armor.armorClass = 5;
      armor.value = 75;
      armor.description = "Heavy armor made of interlocking rings";
      break;
    case ArmorType::PLATE_MAIL:
      armor.name = "Plate Mail";
      armor.armorClass = 3;
      armor.value = 400;
      armor.description = "The finest full plate armor";
      break;
    case ArmorType::SHIELD:
      armor.name = "Shield";
      armor.armorClass = -1;  // Improves AC by 1
      armor.value = 10;
      armor.description = "A wooden shield with metal reinforcement";
      break;
  }

  return armor;
}

Item Inventory::createHealingPotion(int healAmount) {
  Item potion;
  potion.name = "Healing Potion";
  potion.type = ItemType::POTION;
  potion.value = 50;
  potion.description = "Restores " + std::to_string(healAmount) + " hit points";
  potion.effect = "heal";
  potion.effectValue = healAmount;
  return potion;
}

std::string Inventory::serialize() const {
  std::ostringstream oss;
  oss << items_.size() << "\n";

  for (const auto& item : items_) {
    oss << item.name << "|" << static_cast<int>(item.type) << "|" << item.value << "|"
        << item.description << "|" << item.damage << "|" << item.attackBonus << "|"
        << item.armorClass << "|" << item.effect << "|" << item.effectValue << "\n";
  }

  return oss.str();
}

Inventory Inventory::deserialize(const std::string& data) {
  Inventory inv;
  std::istringstream iss(data);
  int count;
  iss >> count;
  iss.ignore();

  for (int i = 0; i < count; i++) {
    std::string line;
    std::getline(iss, line);
    std::istringstream lineStream(line);

    Item item;
    std::string typeStr, temp;

    std::getline(lineStream, item.name, '|');
    std::getline(lineStream, typeStr, '|');
    item.type = static_cast<ItemType>(std::stoi(typeStr));
    std::getline(lineStream, temp, '|');
    item.value = std::stoi(temp);
    std::getline(lineStream, item.description, '|');
    std::getline(lineStream, item.damage, '|');
    std::getline(lineStream, temp, '|');
    item.attackBonus = std::stoi(temp);
    std::getline(lineStream, temp, '|');
    item.armorClass = std::stoi(temp);
    std::getline(lineStream, item.effect, '|');
    std::getline(lineStream, temp, '|');
    item.effectValue = std::stoi(temp);

    inv.addItem(item);
  }

  return inv;
}
