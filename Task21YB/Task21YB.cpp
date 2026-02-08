// Команди руху: right, left, forward, backward (ходять на 1 клітинку)
#include <iostream>
#include <vector>
#include <string>

class Character {
private:
    std::string name_;
    int health_;
    int x_;
    int y_;
    int speed_;

public:
    Character(const std::string& name, int health, int x, int y, int speed)
        : name_(name), health_(health), x_(x), y_(y), speed_(speed) {
        if (speed_ <= 0) speed_ = 1; // робимо мінімальну валідацію, щоб не було менше 0
    }

    void Move(int dx, int dy) {
        x_ += dx * speed_;
        y_ += dy * speed_;
    }

    void TakeDamage(int dmg) {
        health_ -= dmg;
    }

    // методи класу (гетери)
    const std::string& GetName() const { return name_; }
    int GetHealth() const { return health_; }
    int GetX() const { return x_; }
    int GetY() const { return y_; }
    int GetSpeed() const { return speed_; }
};

int main()
{
    std::vector<Character> characters;

    int n;
    std::cout << "How many characters? ";
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string name;
        int health, x, y, speed;

        std::cout << "\nCharacter #" << (i + 1) << "\n";
        std::cout << "Name: ";
        std::cin >> name;

        std::cout << "Health: ";
        std::cin >> health;

        std::cout << "Position (x & y): ";
        std::cin >> x >> y;

        std::cout << "Speed: ";
        std::cin >> speed;

        characters.emplace_back(name, health, x, y, speed);
    }

    std::cout << "\nYou entered: \n";
    for (const Character& c : characters) {
        std::cout << c.GetName()
            << " hp=" << c.GetHealth()
            << " speed=" << c.GetSpeed()
            << " pos=(" << c.GetX() << "," << c.GetY() << ")\n";
    }

    int mx, my;
    int damage;
    int power;

    std::cout << "\nMeteor position (mx & my): ";
    std::cin >> mx >> my;

    std::cout << "Meteor damage: ";
    std::cin >> damage;

    std::cout << "Meteor power: ";
    std::cin >> power;

    int radius = 3 * power;
    long long radiusSq = 1LL * radius * radius;
    
    /* Робиться зміна кординат для персонажів,
    обираємо персонажів на цифру наприклад 1, 
    так як це початковий персонаж і далі 
    в залежності скільки є персонажів.
    Потім уводимо наші координати потрібні, 
    так як було до цього через пробіл */
    char choice;
    std::cout << "Do you want to move characters before impact? (y/n): ";
    std::cin >> choice;

    int m = 0;
    if (choice == 'y' || choice == 'Y') {
        std::cout << "How many more commands? ";
        std::cin >> m;
    }

    std::cout << "How many move commands? ";
    std::cin >> m;

    for (int k = 0; k < m; k++) {
        int num, dx, dy;
        std::cout << "Command #" << (k + 1) << ": ";
        std::cin >> num >> dx >> dy;

        if (num < 1 || num > n) {
            std::cout << "Invalid character number: " << num << "\n";
            continue;
        }

        characters[num - 1].Move(dx, dy);
    }

    bool hasDead = false;
    std::vector<std::string> damaged;

    for (Character& c : characters) {
        long long dx = 1LL * c.GetX() - mx;
        long long dy = 1LL * c.GetY() - my;
        long long distSq = dx * dx + dy * dy;

        if (distSq <= radiusSq) {
            c.TakeDamage(damage);
            damaged.push_back(c.GetName());
            if (c.GetHealth() <= 0) hasDead = true;
        }
    }

    std::cout << "\n**********Results**********\n";
    std::cout << "Hit radius: " << radius << "\n";

    if (damaged.empty()) {
        std::cout << "No one was damaged.\n";
    }
    else {
        std::cout << "Damaged characters: ";
        for (size_t k = 0; k < damaged.size(); k++) {
            std::cout << damaged[k];
            if (k + 1 < damaged.size()) std::cout << ", ";
        }
        std::cout << "\n";
    }

    if (hasDead) std::cout << "There are dead characters.\n";
    else std::cout << "No dead characters.\n";

    std::cout << "\nCharacters after meteor:\n";
    for (const Character& c : characters) {
        std::cout << c.GetName() 
            << " hp=" << c.GetHealth()
            << " pos=(" << c.GetX() << "," << c.GetY() << ")\n";
    }

    return 0;
}
