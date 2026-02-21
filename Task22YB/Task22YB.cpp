// Команди руху: right, left, forward, backward (ходять на 1 клітинку)
#include <iostream>
#include <vector>
#include <string>

class Stats {
private:
    int strength_;
    int agility_;
    int intelligence_;

public:
    Stats(int strength = 0, int agility = 0, int intelligence = 0)
        : strength_(strength), agility_(agility), intelligence_(intelligence) {
    }

    void Add(const Stats& other) {
        strength_ += other.strength_;
        agility_ += other.agility_;
        intelligence_ += other.intelligence_;
    }
    // methods
    int GetStrength() const { return strength_; }
    int GetAgility() const { return agility_; }
    int GetIntelligence() const { return intelligence_; }
};

class Weapon {
private:
    int damage_;
    int cooldown_;
    int lastAttackTurn_;

public:
    Weapon(int damage, int cooldown)
        : damage_(damage), cooldown_(cooldown), lastAttackTurn_(-100) {
    }

    bool CanAttack(int currentTurn) const {
        return currentTurn - lastAttackTurn_ >= cooldown_;
    }

    int Attack(int currentTurn) {
        lastAttackTurn_ = currentTurn;
        return damage_;
    }
};

class PassiveItem {
private:
    Stats bonus_;

public:
    PassiveItem(const Stats& bonus)
        : bonus_(bonus) {
    }

    const Stats& GetBonus() const {
        return bonus_;
    }
};

class Character {
private:
    std::string name_;
    int health_;
    Stats stats_;
    Weapon* weapon_;
    std::vector<PassiveItem> passiveItems_;

public:
    Character(const std::string& name, int health, const Stats& stats)
        : name_(name), health_(health), stats_(stats), weapon_(nullptr) {}

    void ChooseWeapon(Weapon* weapon) {
        if (weapon_ != nullptr) {
            std::cout << name_ << " already has a weapon.\n";
            return;
        }

        weapon_ = weapon;

        std::cout << name_ << " eqiped a weapon.\n";
    }

    void ChoosePassiveItem(const PassiveItem& item) {
        if (passiveItems_.size() >= 3) {
            std::cout << "Cannot equip more than 3 passive items.\n";
            return;
        }

        passiveItems_.push_back(item);
        stats_.Add(item.GetBonus());

        std::cout << name_ << " picked up a passive item.\n";
    }

    void Attack(Character& target, int currentTurn) {
        if (!weapon_) {
            std::cout << name_ << " has no weapon!\n";
            return;
        }

        if (weapon_->CanAttack(currentTurn)) {
            int damage = weapon_->Attack(currentTurn);
            target.TakeDamage(damage);
            std::cout << name_ << " attacked for " << damage << " damage!\n";
        }
        else {
            std::cout << name_ << "'s weapon is still cooling down.\n";
        }
    }

    void TakeDamage(int dmg) {
        health_ -= dmg;
    }

    void Print() const {
        std::cout << name_
            << " HP=" << health_
            << " STR=" << stats_.GetStrength()
            << " AGI=" << stats_.GetAgility()
            << " INT=" << stats_.GetIntelligence()
            << "\n";
    }
};

int main()
{
    std::string heroName;
    int heroHP;

    int heroStrength, heroAgility, heroIntelligence;

    std::cout << "\nEnter hero Name: ";
    std::cin >> heroName;
    
    std::cout << "Enter hero HP: ";
    std::cin >> heroHP;

    std::cout << "Enter hero Strength: ";
    std::cin >> heroStrength;

    std::cout << "Enter hero Agility: ";
    std::cin >> heroAgility;

    std::cout << "Enter hero Intelligence: ";
    std::cin >> heroIntelligence;

    Stats heroStats(heroStrength, heroAgility, heroIntelligence);

    Character hero(heroName, heroHP, heroStats);

    std::string enemyName;
    int enemyHP;

    int enemyStrength, enemyAgility, enemyIntelligence;

    std::cout << "\nEnter enemy name: ";
    std::cin >> enemyName;

    std::cout << "Enter enemy HP: ";
    std::cin >> enemyHP;

    std::cout << "Enter enemy Strength: ";
    std::cin >> enemyStrength;

    std::cout << "Enter enemy Agility: ";
    std::cin >> enemyAgility;

    std::cout << "Enter enemy Intelligence: ";
    std::cin >> enemyIntelligence;

    Stats enemyStats(enemyStrength, enemyAgility, enemyIntelligence);
    
    Character enemy(enemyName, enemyHP, enemyStats);

    std::vector<Weapon> weapons;

    weapons.emplace_back(25, 2);
    weapons.emplace_back(45, 5);
    weapons.emplace_back(35, 3);
    weapons.emplace_back(40, 4);
    
    int weaponChoice;

    std::cout << "\nChoose weapon:\n";

    std::cout << "1 ShortSword\n";
    std::cout << "2 Mace\n";
    std::cout << "3 Axe\n";
    std::cout << "4 LongSword\n";

    std::cin >> weaponChoice;

    hero.ChooseWeapon(&weapons[weaponChoice - 1]);

    std::vector<PassiveItem> items;

    items.emplace_back(Stats(2, 1, 1));
    items.emplace_back(Stats(0, 3, 1));
    items.emplace_back(Stats(1, 2, 0));
    items.emplace_back(Stats(3, 0, 0));

    int itemCount;

    std::cout << "\nHow many passive items (max 3)? ";
    std::cin >> itemCount;
    
    for (int i = 0; i < itemCount; i++) {
        int choice;

        std::cout << "\nChoose passive item:\n";

        std::cout << "1 Ring\n";
        std::cout << "2 Necklace\n";
        std::cout << "3 Boots\n";
        std::cout << "4 Gloves\n";

        std::cin >> choice;

        hero.ChoosePassiveItem(items[choice - 1]);
    }

    hero.Print();
    enemy.Print();

    int turn = 1;

    hero.Attack(enemy, turn); // атакує
    hero.Attack(enemy, turn); // кулдаун

    turn += 2;

    hero.Attack(enemy, turn); // знову йде атака

    hero.Print();
    enemy.Print();

    return 0;
}