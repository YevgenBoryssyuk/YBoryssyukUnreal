// Команди руху: right, left, forward, backward (ходять на 1 клітинку)
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

int main()
{
    int commandCount;
    int i = 0;

    std::vector<std::string> commands;

    std::cout << "How many commands? ";
    std::cin >> commandCount;

    while (i < commandCount) {
        std::string command;
        std::cin >> command;
        commands.push_back(command);
        i++;
    }

    int x, y;
    std::cout << "Enter start position (x y); ";
    std::cin >> x >> y;
    
    std::cout << "Commands stored:\n";
    
    for (std::string& c : commands) {
        for (char& ch : c) {
            ch = std::tolower(ch);
        }

        if (c == "right") {
            x += 1;
        }
        else if (c == "left") {
            x -= 1;
        }
        else if (c == "forward") {
            y += 1;
        }
        else if (c == "backward") {
            y -= 1;
        }
    }

    std::cout << "Final position: (" << x << ", " << y << ")\n";
}
