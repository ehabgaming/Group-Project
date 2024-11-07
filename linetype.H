#ifndef LINETYPE_H
#define LINETYPE_H

// This makes sure our C++ stuff works with C code if needed
#ifdef __cplusplus
extern "C++" {
#endif

// Include the basic stuff we need
#include <vector>    // For storing lists of lines
#include <string>    // For text handling
#include <iostream>  // For input/output

// A simple struct for points, x and y coordinates
struct Point {
   double x, y;
   // Constructor, makes a point with or without coordinates
   Point(double x = 0, double y = 0) : x(x), y(y) {}
};

// Our main class for handling lines
class lineType {
private:
   // These are the coefficients in ax + by = c
   double a, b, c;

public:
   // Makes a new line with given coefficients
   lineType(double a, double b, double c);

   // Gets the slope of the line (-a/b in our format)
   double getSlope() const;

   // Functions to get our a, b, c values
   double getA() const;
   double getB() const;
   double getC() const;

   // Checks if lines are parallel or perpendicular
   bool isParallel(const lineType& other) const;
   bool isPerpendicular(const lineType& other) const;

   // Finds where two lines intersect
   Point findIntersectionPoint(const lineType& other) const;
};
// Shows lines formatted in a ice way
void Display(const std::vector<std::vector<lineType>>& allLines);

// Screen handling functions
void clearScreen();    // Clears the screen
void pauseScreen();    // Waits for Enter key
void displayHeader(const std::string& title);  

// Gets valid input from user (no crashing on letters!)
int getValidIntegerInput();

// Distance formula between points
double calculateDistance(const Point& p1, const Point& p2);

// Menu functions, these handle user interaction
void compareLinesMenu(const std::vector<std::vector<lineType>>& allLines);
void showShapesMenu(const std::vector<std::vector<lineType>>& allLines);

// Geometry functions 
void findIntersection(const lineType& line1, const lineType& line2);
void checkQuadrilateral(const std::vector<lineType>& lines);
void checkLines(const lineType& line1, const lineType& line2);
void showShape(const std::vector<lineType>& lines);

// Close our C++ section
#ifdef __cplusplus
}
#endif

#endif  // End of header guard
