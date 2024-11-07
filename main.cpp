#include "linetype.h"   // For geometry functions
#include <fstream>      // For reading files
#include <iostream>     // For input/output
#include <vector>       // For storing our lines
#include <limits>       // For some number limits

// Let the compiler know these functions exist
void compareCustomLinesMenu();
void createCustomShapeMenu();

int main() {
   // Open file and makes sure this file should have sets of lines (4 lines per set)
   std::ifstream inputFile("linesData.txt");
   if (!inputFile) {
       std::cerr << "Error opening file." << std::endl;  
       return 1;
   }

   // This stores all our sets of lines
   std::vector<std::vector<lineType>> allLines;

   // Variables for line coefficients (ax + by = c)
   double a, b, c;

   // Read lines from file, 4 at a time
   while (inputFile >> a >> b >> c) {  // Read first line of a set
       std::vector<lineType> lines;  // Make a new set
       lines.push_back(lineType(a, b, c));  // Add first line

       // Read 3 more lines to complete the set
       for (int i = 1; i < 4; ++i) {
           if (inputFile >> a >> b >> c) {
               lines.push_back(lineType(a, b, c));
           }
           else {
               std::cerr << "Insufficient data for set." << std::endl;
               return 1;
           }
       }
       // Add completed set to our collection
       allLines.push_back(lines);
   }

   inputFile.close();

   // Main program loop 
   while (true) {
       // Shows the main menu
       displayHeader("Main Menu");
       std::cout << "1. Compare Lines from File" << std::endl;
       std::cout << "2. Show Shapes from File" << std::endl;
       std::cout << "3. Compare Custom Lines" << std::endl;
       std::cout << "4. Create and Analyze Custom Shape" << std::endl;
       std::cout << "5. Exit" << std::endl;
       std::cout << "\nChoose an option: ";

       // Get user's choice
       int option = getValidIntegerInput();

       // Handle their choice
       switch (option) {
       case 1:  // Compare lines we read from file
           compareLinesMenu(allLines);
           break;
       case 2:  // Analyze shapes from file
           showShapesMenu(allLines);
           break;
       case 3:  // Let user input their own lines to compare
           compareCustomLinesMenu();
           break;
       case 4:  // Let user make their own shape
           createCustomShapeMenu();
           break;
       case 5:  // Exits program
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
