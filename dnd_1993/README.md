# D&D 1993 - Text-Based RPG

A single-player text-based Dungeons & Dragons game inspired by AD&D 2nd Edition (1993).

## Features

### Character System
- **6 Classic Races**: Human, Elf, Dwarf, Halfling, Gnome, Half-Elf
- **6 Character Classes**: Fighter, Cleric, Mage, Thief, Ranger, Paladin
- **Ability Scores**: Strength, Dexterity, Constitution, Intelligence, Wisdom, Charisma
- **Character Progression**: Experience points, leveling, and ability improvements
- **AD&D 2nd Edition Rules**: Authentic saving throws and combat mechanics

### Combat System
- **Turn-Based Combat**: Classic D&D initiative and attack rolls
- **10 Monster Types**: Goblins, Orcs, Skeletons, Zombies, Wolves, Trolls, Dragons, and more
- **Attack Rolls**: d20-based hit system with AD&D THAC0 mechanics
- **Damage System**: Weapon-based damage with modifiers
- **Experience Rewards**: Gain XP from defeating monsters

### Inventory & Equipment
- **Weapons**: Longsword, Shortsword, Dagger, Mace, Battle Axe, Bow, and more
- **Armor**: Leather, Chainmail, Plate Mail, Shields
- **Potions**: Healing potions and magical items
- **Equipment Management**: Equip/unequip weapons and armor
- **Gold Economy**: Buy, sell, and manage your wealth

### Adventure System
- **8 Explorable Locations**: Towns, Dungeons, Forests, Caves, Ruins, Taverns, Temples, Shops
- **Quest System**: Progress through a storyline with objectives
- **Random Encounters**: Unpredictable monster appearances
- **Rest System**: Recover hit points at safe locations
- **NPCs**: Talk to characters and gather information
- **Treasure Hunting**: Search for gold and items

### Dice System
- **Authentic D&D Dice**: d4, d6, d8, d10, d12, d20, d100
- **Dice Notation Parser**: Supports "3d6", "1d20+5", "2d8-1" format
- **Ability Score Rolling**: Classic 3d6 or 4d6-drop-lowest methods
- **Random Number Generation**: C++ Mersenne Twister for fair rolls

### Save/Load System
- **Save Game**: Save your progress at any time
- **Load Game**: Continue from where you left off
- **Character Persistence**: All stats, inventory, and progress saved

## Building

### Requirements
- C++17 compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher

### Compilation

```bash
cd dnd_1993
mkdir build && cd build
cmake ..
make
```

### Running

```bash
./dnd_1993
```

## How to Play

### Main Menu
1. **New Game**: Create a new character and start your adventure
2. **Load Game**: Continue a saved game
3. **Quit**: Exit the game

### Character Creation
1. **Name Your Character**: Choose a memorable name
2. **Roll Ability Scores**: Generate your character's stats
3. **Choose Race**: Select from 6 races (affects stats and abilities)
4. **Choose Class**: Select your character class (determines skills and progression)
5. **Receive Starting Equipment**: Get initial weapons, armor, and gold

### Gameplay

**In Town**:
- Visit the **Shop** to buy equipment
- Visit the **Temple** for healing
- Visit the **Tavern** for rumors and quests
- **Rest** to recover hit points
- **Explore** to find adventures

**During Combat**:
- **Attack**: Strike your enemy with your equipped weapon
- **Use Item**: Drink potions or use magical items
- **Flee**: Attempt to escape (may fail)

**In the Wilderness**:
- **Search**: Look for treasure and items
- **Rest**: Recover HP (risk random encounters)
- **Continue**: Move to the next area

### Character Advancement
- Defeat monsters to earn **Experience Points (XP)**
- Level up when you reach XP thresholds
- Gain more **Hit Points**
- Improve **Attack Bonuses**
- Better **Saving Throws**

### Saving Your Game
- From the adventure menu, select "Save Game"
- Your character, inventory, and progress are saved
- Load anytime from the main menu

## AD&D 2nd Edition Mechanics

### Armor Class (AC)
- **Lower is better** (as in classic AD&D)
- Base AC 10 (unarmored)
- Armor and Dexterity improve (lower) AC
- Shields provide -1 AC bonus

### Attack Rolls
- Roll 1d20 + Attack Bonus
- Compare to enemy's Armor Class
- Critical hits on natural 20
- Critical misses on natural 1

### Saving Throws
- **Death/Poison**: Constitution-based
- **Wands/Rods**: Dexterity-based
- **Paralysis/Petrification**: Constitution-based
- **Breath Weapons**: Dexterity-based
- **Spells**: Wisdom-based

### Experience Progression
| Level | XP Required | Fighter HP | Mage HP | Cleric HP | Thief HP |
|-------|-------------|------------|---------|-----------|----------|
| 1     | 0           | 1d10       | 1d4     | 1d8       | 1d6      |
| 2     | 2,000       | +1d10      | +1d4    | +1d8      | +1d6     |
| 3     | 4,000       | +1d10      | +1d4    | +1d8      | +1d6     |
| 4     | 8,000       | +1d10      | +1d4    | +1d8      | +1d6     |
| 5     | 16,000      | +1d10      | +1d4    | +1d8      | +1d6     |

## Monsters

### Common Encounters
- **Goblin**: AC 6, HD 1-1, Damage 1d6, XP 7
- **Kobold**: AC 7, HD 1d4, Damage 1d4, XP 5
- **Skeleton**: AC 7, HD 1, Damage 1d6, XP 35
- **Orc**: AC 6, HD 1, Damage 1d8, XP 10
- **Wolf**: AC 7, HD 2+2, Damage 1d4+1, XP 35

### Rare Encounters
- **Ogre**: AC 5, HD 4+1, Damage 1d10, XP 175
- **Troll**: AC 4, HD 6+3, Damage 2d6, XP 650
- **Young Dragon**: AC 2, HD 8, Damage 2d8, XP 1,750

## Tips for Survival

1. **Start with Good Armor**: Visit the shop first
2. **Heal Often**: Don't let HP get too low
3. **Flee When Necessary**: Live to fight another day
4. **Save Regularly**: Don't lose progress
5. **Manage Resources**: Keep healing potions in inventory
6. **Choose Battles Wisely**: Some enemies are too powerful early on
7. **Rest When Needed**: HP doesn't regenerate during combat
8. **Upgrade Equipment**: Better weapons mean more damage

## Character Classes

### Fighter
- **Best At**: Combat and physical challenges
- **Hit Die**: d10 (most HP)
- **Prime Requisite**: Strength
- **Weapons**: All types
- **Armor**: All types including shields

### Cleric
- **Best At**: Healing and undead combat
- **Hit Die**: d8
- **Prime Requisite**: Wisdom
- **Weapons**: Blunt weapons only
- **Armor**: All types
- **Special**: Can turn undead

### Mage
- **Best At**: Magical damage and utility
- **Hit Die**: d4 (least HP)
- **Prime Requisite**: Intelligence
- **Weapons**: Staff, dagger only
- **Armor**: None (robes only)
- **Special**: Spell casting (future feature)

### Thief
- **Best At**: Stealth and traps
- **Hit Die**: d6
- **Prime Requisite**: Dexterity
- **Weapons**: Light weapons
- **Armor**: Light armor only
- **Special**: Backstab bonus (future feature)

### Ranger
- **Best At**: Wilderness survival
- **Hit Die**: d10
- **Prime Requisite**: Strength, Dexterity, Wisdom
- **Weapons**: All types
- **Armor**: Light to medium
- **Special**: Tracking (future feature)

### Paladin
- **Best At**: Holy combat
- **Hit Die**: d10
- **Prime Requisite**: Strength, Charisma
- **Weapons**: All types
- **Armor**: All types
- **Special**: Lay on hands (future feature)

## File Locations

- **Game Executable**: `build/dnd_1993`
- **Save Files**: `saves/savegame.dat`
- **Game Data**: `data/` (future: monster stats, locations, items)

## Development Roadmap

### Version 1.0 (Current)
- [x] Character creation
- [x] Basic combat system
- [x] Inventory management
- [x] Random encounters
- [x] Save/load functionality
- [x] Multiple locations
- [x] Experience and leveling

### Version 1.1 (Planned)
- [ ] Spell casting system
- [ ] More monster types
- [ ] Boss encounters
- [ ] Enhanced quest system
- [ ] Special class abilities
- [ ] More equipment types

### Version 1.2 (Future)
- [ ] Party system (companions)
- [ ] Skills and abilities
- [ ] Advanced magic items
- [ ] Dungeon generation
- [ ] Multiple quests
- [ ] Character portraits (ASCII art)

## Credits

Inspired by:
- **Advanced Dungeons & Dragons 2nd Edition** (TSR, 1989-2000)
- Classic CRPGs like Eye of the Beholder and Baldur's Gate
- Text-based adventures and MUDs from the early 1990s

## License

This project is open source and available for personal use. D&D is a trademark of Wizards of the Coast.

## Contributing

Contributions welcome! Areas of interest:
- Additional monster types
- Spell system implementation
- Quest scripting engine
- ASCII art and visual improvements
- Balance tweaks
- Bug fixes

## Contact

For issues and feature requests, please use the repository's issue tracker.

---

**May your dice roll high!** 🎲
