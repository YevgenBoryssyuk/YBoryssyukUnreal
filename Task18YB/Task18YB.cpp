#include <iostream>

void ApplyDamage(int healths[], int playerCount, int damage) {
        int i = 0;
        while (i < playerCount) {
            healths[i] -= damage;
            //healths[i] = healths[i] - damage;
            i++;
        }
}

int main()
{
    int playerCount;
    std::string names[100];
    int healths[100];
    int damage;

    std::cout << "Hi! Create plaeyrs and set them the right amount of health. Remark write their amount, name and health" << std::endl;
    std::cin >> playerCount;

    int i = 0;

    while (i < playerCount) {
        std::cin >> names[i];
        std::cin >> healths[i];

        while (healths[i] <= 0 || healths[i] > 100) {
            std::cout << "You write the wrong amount of health. Please write again your health" << std::endl;
            std::cin >> healths[i];
        }

        std::cout << "Player " << i + 1 << ": " << names[i] << " has " << healths[i] << " points" << std::endl;
        i++;
    }
     
    i = 0;

    std::cout << "Write damage value: ";
    std::cin >> damage;
    ApplyDamage(healths, playerCount, damage);

    std::cout << "\nAfter damage: \n";

    i = 0;
    while (i < playerCount) {
        std::cout << names[i] << " has " << healths[i] << " HP\n";
        i++;
    }
        
    int maxHealth = healths[0];
    std::string maxName = names[0];

    i = 1;
        
    while (i < playerCount) {
        if (healths[i] > maxHealth) {
            maxHealth = healths[i];
            maxName = names[i];
        }
       i++;
    }

    int minHealth = healths[0];
    std::string minName = names[0];
    
    i = 1;

    while (i < playerCount) {
        if (healths[i] < minHealth) {
            minHealth = healths[i];
            minName = names[i];
        }
        i++;
    }

    int sum = 0;

    i = 0;
        
    while (i < playerCount) {
        sum = sum + healths[i];
        i++;
    }

    float average = (float)sum / playerCount;

    std::cout << "\nStatistics:\n";
    std::cout << "Max health: " << maxName << " (" << maxHealth << " HP)\n";
    std::cout << "Min health: " << minName << " (" << minHealth << " HP)\n";
    std::cout << "Avarage Health: " << average << "\n";

    std::cout << "\nDead platers:\n";

    bool someoneDead = false;

    i = 0;
    while (i < playerCount) {
        if (healths[i] <= 0) {
            std::cout << names[i] << " is dead, because he/she has " << healths[i] << "\n";
            someoneDead = true;
        }
        i++;
    }

    if (!someoneDead) {
        std::cout << "No dead players\n";
    }
}
