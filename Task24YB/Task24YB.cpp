// Команди руху: right, left, forward, backward (ходять на 1 клітинку)
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

class Stats {
private:
    int strength_;
    int agility_;
    int intelligence_;

public:
    Stats(int strength = 0, int agility = 0, int intelligence = 0)
        : strength_(strength), agility_(agility), intelligence_(intelligence) {
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

// Observer pattern
enum class EventType {
    Moved,
    Attacked,
    Damaged,
    ItemAdded,
    LowHealth
};

struct EventData {
    EventType type;
    std::string actor;
    std::string target;
    std::string objectName;
    int value = 0;
    int x = 0;
    int y = 0;
    int health = 0;
};

class Observer {
public:
    virtual void Update(const EventData& data) = 0;
    virtual ~Observer() = default;
};

class Subject {
private:
    std::vector<Observer*> observers_;

public:
    void Attach(Observer* observer) {
        if (observer) {
            observers_.push_back(observer);
        }
    }

    void Detach(Observer* observer) {
        observers_.erase(
            std::remove(observers_.begin(), observers_.end(), observer),
            observers_.end()
        );
    }

protected:
    void Notify(const EventData& data) {
        for (Observer* observer : observers_) {
            if (observer) {
                observer->Update(data);
            }
        }
    }
};

//Concrete Observers
class ConsoleLogger : public Observer {
    public:
        void Update(const EventData& data) override {
            switch (data.type) {
            case EventType::Moved:
                std::cout << "[LOG] " << data.actor
                          << " moved to (" << data.x << ", " << data.y 
                          << ") with speed " << data.value << "\n";
                break;

            case EventType::Attacked:
                std::cout << "[LOG] " << data.actor
                          << " attacked " << data.target
                          << " with " << data.objectName
                          << " for " << data.value << " damage\n";
                break;

            case EventType::Damaged:
                std::cout << "[LOG] " << data.actor
                          << " picked up " << data.objectName
                          << " (speed "
                          << (data.value >= 0 ? "+" : "") << data.value << ")\n";
                break;
                
            case EventType::LowHealth:
                std::cout << "[WARNING] " << data.actor
                          << " has low HP! Current HP = " << data.health << "\n";
                break;
            }
        }
};

class LowHealthObserver : public Observer {
public:
    void Update(const EventData& data) override {
        if (data.type == EventType::LowHealth) {
            std::cout << "[LOW HP OBSERVER] " << data.actor
                      << " is in danger!\n";
        }
    }
};

class StatisticsObserver : public Observer {
private:
    int moveCount_ = 0;
    int attackCount_ = 0;
    int itemCount_ = 0;

public: 
    void Update(const EventData& data) override {
        switch(data.type) {
            case EventType::Moved:
                ++moveCount_;
                break;
            case EventType::Attacked:
                ++attackCount_;
                break;
            case EventType::ItemAdded:
                ++itemCount_;
                break;
            default:
                break;
        }
    }

    void PrintReport() const {
        std::cout << "\n=== StaticsObserver report ===\n";
        std::cout << "Moves: " << moveCount_ << "\n";
        std::cout << "Attacks: " << attackCount_ << "\n";
        std::cout << "Items picked: " << itemCount_ << "\n";
    }
};

class Character : public Subject {
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

    const std::string& GetNAme() const { return name_; }

    void ChooseWeapon(std::shared_ptr<Weapon> weapon) {
        if (weapon_) {
            std::cout << name_ << " already has a weapon: " << weapon_->GetName() << "\n";
            return;
        }

        weapon_ = std::move(weapon);
        std::cout << name_ << " eqiped a weapon: " << weapon_->GetName() << "\n";
        return;
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

    // додавання предмета у інвентар
    void AddItemToInventory(std::unique_ptr<SpeedItem> item) {
        if (!item) {
            std::cout << "Cannot add empty item.\n";
            return;
        }

        int bonus = item->GetSpeedBonus();
        std::string itemName = item->GetName();

        inventory_.push_back(std::move(item));
        RecalculateCurrentSpeed();

        Notify({ EventType::ItemAdded, name_, "", itemName, bonus, x_, y_, health_ });
    }

    void Move(int dx, int dy) {
        x_ += dx * currentSpeed_;
        y_ += dy * currentSpeed_;

        Notify({ EventType::Moved, name_, "", "", currentSpeed_, x_, y_, health_ });
    }

    void Attack(Character& target, int currentTurn) {
        if (!weapon_) {
            std::cout << name_ << " has no weapon!\n";
            return;
        }

        if (!weapon_->CanAttack(currentTurn)) {
            std::cout << name_ << "'s weapon is still cooling down.\n";
            return;
        }

        int damage = weapon_->Attack(currentTurn);

        Notify({ EventType::Attacked, name_, target.GetNAme(), weapon_->GetName(), damage, x_, y_ < health_ });
        target.TakeDamage(damage);
    }

    void TakeDamage(int dmg) {
        health_ -= dmg;
        if (health_ < 0) {
            health_ = 0;
        }

        Notify({ EventType::Damaged, name_, "", "", dmg, x_, y_, health_ });

        if (health_ > 0 && health_ <= 30) {
            Notify({ EventType::LowHealth, name_, "", "", 0, x_, y_, health_ });
        }
    }

    void Print() const {
        std::cout << name_ << " | HP=" << health_ << " | STR=" << stats_.GetStrength() << " AGI=" << stats_.GetAgility() << " INT=" << stats_.GetIntelligence() << " | BaseSpeed=" << baseSpeed_ << " CurrentSpeed=" << currentSpeed_ << " | Pos=(" << x_ << ", " << y_ << ")\n";
    }
};

int main() {
    Character hero("Hero", 120, Stats(10, 8, 6), 3);
    Character enemy("Enemy", 70, Stats(5, 5, 5), 2);

    auto sword = std::make_shared<Weapon>("LongSword", 25, 2);
    hero.ChooseWeapon(sword);

    // Спостерігачі
    ConsoleLogger logger;
    LowHealthObserver lowHpObserver;
    StatisticsObserver statsObserver;

    // Підписка
    hero.Attach(&logger);
    hero.Attach(&lowHpObserver);
    hero.Attach(&statsObserver);

    enemy.Attach(&logger);
    enemy.Attach(&lowHpObserver);

    std::cout << "\n=== Initial state ===\n";
    hero.Print();
    enemy.Print();

    std::cout << "\n=== Add items ===\n";
    hero.AddItemToInventory(std::make_unique<SpeedItem>("Boots of Wind", 2));
    hero.AddItemToInventory(std::make_unique<SpeedItem>("Heavy Armor", -1));

    std::cout << "\n=== Movement ===\n";
    hero.Move(1, 0);
    hero.Move(0, 1);

    std::cout << "\n=== Combat ===\n";
    int turn = 1;
    hero.Attack(enemy, turn);   // атака
    hero.Attack(enemy, turn);   // кулдаун
    turn += 2;
    hero.Attack(enemy, turn);   // знову атака

    std::cout << "\n=== Final state ===\n";
    hero.Print();
    enemy.Print();

    statsObserver.PrintReport();

    return 0;
}