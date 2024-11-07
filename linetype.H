#ifndef LINETYPE_H    
#define LINETYPE_H    

// This makes sure our C++ stuff works with C code if needed
#ifdef __cplusplus
extern "C++" {       
#endif

// Get the tools we need
#include <vector>    // For storing lists of lines
#include <string>    // For text handling
#include <iostream>  // For input/output

// A simple struct for points, x and y coordinates
    struct Point {
        double x, y;                                     
        // Constructor, makes a point with or without coordinates

        Point(double x = 0, double y = 0) : x(x), y(y) {} 
    };

    // This is our drawing canvas, where we can draw our lines and shapes using ASCII characters
    struct Canvas {
        static const int WIDTH = 70;    // How wide our canvas is
        static const int HEIGHT = 30;   // How tall our canvas is
        char grid[HEIGHT][WIDTH];       // The actual grid we draw on, like pixels
        double xMin, xMax, yMin, yMax;  // The boundaries of what we can see

        // Things that canvas can do:
        Canvas();        // Creates a new blank canvas
        void clear();    // Wipes the canvas clean
        void plotSegment(const Point& start, const Point& end, char symbol);  // Draws a line segment
        void plotPoint(double x, double y, char symbol);                      // Plots a single point
        void plotLine(const class lineType& line, char symbol);              // Draws a whole line
        void plotIntersection(const Point& p, const std::string& label);     // Marks where lines cross
        void autoScale(const std::vector<class lineType>& lines);            // Adjusts view to fit lines
        void display() const;                                                // Shows the canvas
    };

    // Our main class for handling lines
    class lineType {
    private:
        // These are the coefficients in ax + by = c
        double a, b, c;

    public:
        // Makes a new line with given coefficients
        lineType(double a, double b, double c);    // Makes a new line
           // Functions to get our a, b, c values
        double getA() const;     
        double getB() const;       
        double getC() const;       
           // Gets the slope of the line (-a/b in our format)
        double getSlope() const;   

        // Checks if lines are parallel or perpendicular
        bool isParallel(const lineType& other) const;          
        bool isPerpendicular(const lineType& other) const;     
        // Finds where two lines intersect
        Point findIntersectionPoint(const lineType& other) const;  
    };

    // Screen handling functions
    void clearScreen();     // Clears the screen
    void pauseScreen();     // Waits for user to press Enter

    void displayHeader(const std::string& title);  // Shows pretty menu headers
    int getValidIntegerInput();    // Makes sure user types actual numbers
    double calculateDistance(const Point& p1, const Point& p2);  // Finds distance between points
    void displayVisualization(const std::vector<lineType>& lines);  // Shows lines visually

    // Functions for analyzing lines:
    void findIntersection(const lineType& line1, const lineType& line2);  // Finds crossing point
    void checkLines(const lineType& line1, const lineType& line2);        // Analyzes line relationships

    // Functions for analyzing shapes:
    void showShape(const std::vector<lineType>& lines);         // Shows shape properties
    void checkQuadrilateral(const std::vector<lineType>& lines);  // Identifies shape type

    // Menu functions that handle user interaction:
    void compareLinesMenu(const std::vector<std::vector<lineType>>& allLines);  // For comparing lines
    void showShapesMenu(const std::vector<std::vector<lineType>>& allLines);    // For analyzing shapes
    void compareCustomLinesMenu();    // For comparing user's own lines
    void createCustomShapeMenu();     // For creating user's own shapes
    lineType getLineFromUser(const std::string& lineNumber);    // Gets line input from user

    // End of C++ specific code
#ifdef __cplusplus
}
#endif

// End of include guard
#endif
