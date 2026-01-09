#include <iostream>

int main()
{
    std::string Name;
    int Health;
    std::string CharacterClass;

    std::cout << "Hi player! To create your character you need to write your name, write your health from 1 to 100 and choose class from (warrior, mage, archer):\n";
    std::cin >> Name >> Health >> CharacterClass;
    std::cout << "Hi " << Name << ", your current health is " << Health << " and your class is " << CharacterClass << "!\n";
    
    if (Health == 100) {
        std::cout << "Your health is max!\n";
    }
    else {
        std::cout << "Your health is " << Health << "\n";
    }
    if (CharacterClass == "warrior" || CharacterClass == "mage" || CharacterClass == "archer") {
        std::cout << "You choose right class!\n";
    }
    else {
        std::cout << "You choose wrong class!\n";
    }
}
