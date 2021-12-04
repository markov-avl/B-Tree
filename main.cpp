#include "src/btree.h"
#include <regex>


int main() {
    BTree<int> bTree;
    std::string command;

    while (command != "end") {
        std::cout << "Input command: ";
        std::getline(std::cin, command);
        if (std::regex_match(command, std::regex("add [-]?[\\d]*"))) {
            command.erase(0, 4);
            bTree.add(stoi(command));
            bTree.print();
        } else if (std::regex_match(command, std::regex("del [-]?[\\d]*"))) {
            command.erase(0, 4);
            bTree.remove(stoi(command));
            bTree.print();
        } else if (command != "end") {
            std::cout << std::endl << "Unknown command";
        }
        std::cout << std::endl << std::endl;
    }

    return 0;
}