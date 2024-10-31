#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <limits>
#include "linetype.h"


int main() {
    std::ifstream inputFile("linesData.txt");
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::vector<std::vector<lineType>> allLines;
    double a, b, c;

    // Read lines from the file and store them in a vector of vectors
    while (inputFile >> a >> b >> c) {
        std::vector<lineType> lines;
        lines.push_back(lineType(a, b, c));
        for (int i = 1; i < 4; ++i) { // Read the next three lines
            if (inputFile >> a >> b >> c) {
                lines.push_back(lineType(a, b, c));
            }
            else {
                std::cerr << "Insufficient data for set." << std::endl;
                return 1;
            }
        }
        allLines.push_back(lines);
    }
    inputFile.close(); // Close the file

    // Main menu loop
    while (true) {
        std::cout << "\n--- Main Menu ---" << std::endl;
        std::cout << "1. Compare Lines" << std::endl;
        std::cout << "2. Show Shapes" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose an option: ";
        int option;
        std::cin >> option;

        switch (option) {
        case 1:
            compareLinesMenu(allLines);
            break;
        case 2:
            showShapesMenu(allLines);
            break;
        case 3:
            std::cout << "Exiting the program." << std::endl;
            return 0; // Exit the program
        default:
            std::cout << "Invalid option. Please choose again." << std::endl;
            break;
        }
    }

    return 0; // Main should return an integer value
}
