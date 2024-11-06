#include "linetype.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <limits>

// Forward declarations
void compareCustomLinesMenu();
void createCustomShapeMenu();

int main() {
    std::ifstream inputFile("linesData.txt");
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::vector<std::vector<lineType>> allLines;
    double a, b, c;
    while (inputFile >> a >> b >> c) {
        std::vector<lineType> lines;
        lines.push_back(lineType(a, b, c));
        for (int i = 1; i < 4; ++i) {
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
    inputFile.close();

    while (true) {
        displayHeader("Main Menu");
        std::cout << "1. Compare Lines from File" << std::endl;
        std::cout << "2. Show Shapes from File" << std::endl;
        std::cout << "3. Compare Custom Lines" << std::endl;
        std::cout << "4. Create and Analyze Custom Shape" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "\nChoose an option: ";

        int option = getValidIntegerInput();

        switch (option) {
        case 1:
            compareLinesMenu(allLines);
            break;
        case 2:
            showShapesMenu(allLines);
            break;
        case 3:
            compareCustomLinesMenu();
            break;
        case 4:
            createCustomShapeMenu();
            break;
        case 5:
            clearScreen();
            std::cout << "Thank you for using the program!" << std::endl;
            return 0;
        default:
            std::cout << "Invalid option. Please choose again." << std::endl;
            pauseScreen();
            break;
        }
    }
    return 0;
}
