// Команди руху: right, left, forward, backward (ходять на 1 клітинку)
#include <iostream>
#include <vector>
#include <string>

struct Character {
    std::string name;
    int health;
    int x;
    int y;
};

int main()
{
    std::vector<Character> characters;

    int n;
    std::cout << "How many ccharacters? ";
    std::cin >> n;

    int i = 0;
    while (i < n) {
        Character c;
        std::cout << "\nCharacter #" << (i + 1) << "\n";
        std::cout << "Name: ";
        std::cin >> c.name;
        std::cout << "Health: ";
        std::cin >> c.health;
        std::cout << "Position (x & y): ";
        std::cin >> c.x >> c.y;

        characters.push_back(c);
        i++;
    }

    std::cout << "\nYou entered: \n";
    for (const Character& c : characters) {
        std::cout << c.name << " hp=" << c.health
            << " pos=(" << c.x << "," << c.y << ")\n";
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

    bool hasDead = false;
    std::vector<std::string> damaged;

    for (Character& c : characters) {
        long long dx = 1LL * c.x - mx;
        long long dy = 1LL * c.y - my;
        long long distSq = dx * dx + dy * dy;

        if (distSq <= radiusSq) {
            c.health -= damage;
            damaged.push_back(c.name);

            if (c.health <= 0) hasDead = true;
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
        std::cout << c.name << " hp=" << c.health
            << " pos=(" << c.x << "," << c.y << ")\n";
    }
}
