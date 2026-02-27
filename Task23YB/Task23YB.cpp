// Команди руху: right, left, forward, backward (ходять на 1 клітинку)
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <limits>

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
    std::string name_;
    int damage_;
    int cooldown_;
    int lastAttackTurn_;

public:
    Weapon(std::string name, int damage, int cooldown)
        : name_(std::move(name)), damage_(damage), cooldown_(cooldown), lastAttackTurn_(-100) {
    }

    const std::string& GetName() const { return name_; }

    bool CanAttack(int currentTurn) const {
        return currentTurn - lastAttackTurn_ >= cooldown_;
    }

    int Attack(int currentTurn) {
        lastAttackTurn_ = currentTurn;
        return damage_;
    }
};

class SpeedItem {
    private:
        std::string name_;
        int speedBonus_; /// може бути від'ємним

    public:
        SpeedItem(std::string name, int speedBonus)
            : name_(std::move(name)), speedBonus_(speedBonus) {
        }

        const std::string& GetName() const { return name_; }
        int GetSpeedBonus() const { return speedBonus_; }
};

class Character {
private:
    std::string name_;
    int health_;
    Stats stats_;
    
    int x_;
    int y_;

    int baseSpeed_;
    int currentSpeed_;

    // Розумний вказівник shared_ptr - декілька власників 
    std::shared_ptr<Weapon> weapon_;

    // Розумний вказівник unique_ptr - один вказівник
    std::vector<std::unique_ptr<SpeedItem>> inventory_;

public:
    Character(const std::string& name, int health, const Stats& stats, int baseSpeed)
        : name_(name), health_(health), stats_(stats), x_(0), y_(0), baseSpeed_(baseSpeed), currentSpeed_(baseSpeed), weapon_(nullptr) {
    }

    void ChooseWeapon(std::shared_ptr<Weapon> weapon) {
        if (weapon_) {
            std::cout << name_ << " already has a weapon: " << weapon_->GetName() << "\n";
            return;
        }

        weapon_ = std::move(weapon);
        std::cout << name_ << " eqiped a weapon: " << weapon_->GetName() << "\n";
        return;
    }

    // додавання прудмета у інвентар
    void AddItemToInventory(std::unique_ptr<SpeedItem> item) {
        if (!item) {
            std::cout << "Cannot add empty item.\n";
            return;
        }

        std::cout << name_ << " picked up item: " << item->GetName() << " (speed " << (item->GetSpeedBonus() >= 0 ? "+" : "") << item->GetSpeedBonus() << ")\n";

        inventory_.push_back(std::move(item));

        // перерховуємо швидкість після додавання
        RecalculateCurrentSpeed();
    }
    // підрахунок швидкості з урахуванням усіх предметів
    void RecalculateCurrentSpeed() {
        currentSpeed_ = baseSpeed_;

        for (const auto& item : inventory_) {
            if (item) {
                currentSpeed_ += item->GetSpeedBonus();
            }
        }

        // Захист від нульової та відьємної швидкості
        if (currentSpeed_ < 1) {
            currentSpeed_ = 1;
        }
    }

    void Move(int x, int y) {
        x_ += x * currentSpeed_;
        y_ += y * currentSpeed_;

        std::cout << name_ << " moved to (" << x_ << ", " << y_ << ") with speed " << currentSpeed_ << "\n";
    }

    void Attack(Character& target, int currentTurn) {
        if (!weapon_) {
            std::cout << name_ << " has no weapon!\n";
            return;
        }

        if (weapon_->CanAttack(currentTurn)) {
            int damage = weapon_->Attack(currentTurn);
            target.TakeDamage(damage);
            std::cout << name_ << " attacked " << target.name_ << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name_ << "'s weapon is still cooling down.\n";
        }
    }

    void TakeDamage(int dmg) {
        health_ -= dmg;
        if (health_ < 0) health_ = 0;
    }

    void PrintInventory() const {
        std::cout << "Inventory of " << name_ << ":\n";
        if (inventory_.empty()) {
            std::cout << " (empty)\n";
            return;
        }

        for (size_t i = 0; i < inventory_.size(); ++i) {
            std::cout << "  " << (i + 1) << ") " << inventory_[i]->GetName() << " (speed " << (inventory_[i]->GetSpeedBonus() >= 0 ? "+" : "") << inventory_[i]->GetSpeedBonus() << ")\n";
        }
    }

    void Print() const {
        std::cout << name_ << " | HP=" << health_ << " | STR=" << stats_.GetStrength() << " AGI=" << stats_.GetAgility() << " INT=" << stats_.GetIntelligence() << " | BaseSpeed=" << baseSpeed_ << " CurrentSpeed=" << currentSpeed_ << " | Pos=(" << x_ << ", " << y_ << ")\n";
    }
};

int ReadIntSafe() {
    int value;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter a valid number: ";
    }
    return value;
}

int main() {
    std::cout << "=== Character creation ===\n";

    std::string heroName;
    int heroHP;
    int heroStrength, heroAgility, heroIntelligence;
    int heroBaseSpeed;

    std::cout << "Enter hero name: ";
    std::cin >> heroName;

    std::cout << "Enter hero HP: ";
    heroHP = ReadIntSafe();

    std::cout << "Enter hero Strength: ";
    heroStrength = ReadIntSafe();

    std::cout << "Enter hero Agility: ";
    heroAgility = ReadIntSafe();

    std::cout << "Enter hero Intelligence: ";
    heroIntelligence = ReadIntSafe();

    std::cout << "Enter hero BASE speed: ";
    heroBaseSpeed = ReadIntSafe();

    Character hero(heroName, heroHP, Stats(heroStrength, heroAgility, heroIntelligence), heroBaseSpeed);
    Character enemy("Enemy", 100, Stats(5, 5, 5), 3);

    // Каталог зброї: shared_ptr для демонстрації спільного володіння
    std::vector<std::shared_ptr<Weapon>> weapons;
    weapons.push_back(std::make_shared<Weapon>("ShortSword", 25, 2));
    weapons.push_back(std::make_shared<Weapon>("Mace", 45, 5));
    weapons.push_back(std::make_shared<Weapon>("Axe", 35, 3));
    weapons.push_back(std::make_shared<Weapon>("LongSword", 40, 4));

    std::cout << "\nChoose weapon:\n"
        << "1) ShortSword\n"
        << "2) Mace\n"
        << "3) Axe\n"
        << "4) LongSword\n"
        << "Your choice: ";

    int weaponChoice = ReadIntSafe();
    if (weaponChoice >= 1 && weaponChoice <= 4) {
        hero.ChooseWeapon(weapons[weaponChoice - 1]);
    }
    else {
        std::cout << "Invalid weapon choice. Hero will have no weapon.\n";
    }

    std::cout << "\nHow many speed items to add (0..4)? ";
    int itemCount = ReadIntSafe();
    if (itemCount < 0) itemCount = 0;
    if (itemCount > 4) itemCount = 4;

    for (int i = 0; i < itemCount; ++i) {
        std::cout << "\nChoose speed item:\n"
            << "1) Boots of Wind (+2 speed)\n"
            << "2) Ring of Haste (+1 speed)\n"
            << "3) Heavy Armor (-2 speed)\n"
            << "4) Feather Cloak (+3 speed)\n"
            << "Your choice: ";

        int choice = ReadIntSafe();

        switch (choice) {
        case 1:
            hero.AddItemToInventory(std::make_unique<SpeedItem>("Boots of Wind", 2));
            break;
        case 2:
            hero.AddItemToInventory(std::make_unique<SpeedItem>("Ring of Haste", 1));
            break;
        case 3:
            hero.AddItemToInventory(std::make_unique<SpeedItem>("Heavy Armor", -2));
            break;
        case 4:
            hero.AddItemToInventory(std::make_unique<SpeedItem>("Feather Cloak", 3));
            break;
        default:
            std::cout << "Invalid choice, item skipped.\n";
            break;
        }
    }

    std::cout << "\n=== Before actions ===\n";
    hero.Print();
    hero.PrintInventory();
    enemy.Print();

    std::cout << "\n=== Movement demo (speed affects distance) ===\n";
    hero.Move(1, 0);
    hero.Move(0, 1);

    std::cout << "\n=== Combat demo ===\n";
    int turn = 1;
    hero.Attack(enemy, turn);   // атака
    hero.Attack(enemy, turn);   // кулдаун
    turn += 2;
    hero.Attack(enemy, turn);   // знову атака (якщо кулдаун дозволяє)

    std::cout << "\n=== Final state ===\n";
    hero.Print();
    enemy.Print();

    return 0;
}