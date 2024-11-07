#include "linetype.h"      // Our special line-related code
#include <limits>          // For using infinity and really big/small numbers
#include <cmath>           // For math functions like sqrt (square root)
#include <algorithm>       // For sorting and other handy operations
#include <iostream>        // For input/output (like cout and cin)
#include <iomanip>        // For making our output look neat
#include <cstdlib>        // For system stuff like clearing the screen
#include <sstream>        // For working with strings as streams
#include <string>         // For text manipulation

using namespace std;      // So we don't have to write std:: all the time
// If two numbers are super close (within 0.000000001), we'll treat them as equal, helps avoid floating point comparison headaches
const double EPSILON = 1e-9;

// Constructor implementation,setting up a line with its a, b, c values, in ax + by = c 
lineType::lineType(double a, double b, double c) : a(a), b(b), c(c) {}

// Simple functions to get our a, b, c values when we need them
double lineType::getA() const { return a; }
double lineType::getB() const { return b; }
double lineType::getC() const { return c; }

// Figures out the slope of the line using -a/b, if b is zero (vertical line), 
//returns infinity since we can't divide by zero.
double lineType::getSlope() const {
    if (abs(b) < EPSILON) {
        return numeric_limits<double>::infinity();
    }
    return -a / b;
}

// Checks if two lines are parallel, either they're both vertical or have the same slope
bool lineType::isParallel(const lineType& other) const {
    if (abs(b) < EPSILON && abs(other.b) < EPSILON) {
        return true;
    }
    return abs(getSlope() - other.getSlope()) < EPSILON;
}

// Checks if lines are perpendicular
bool lineType::isPerpendicular(const lineType& other) const {
    const double slope1 = getSlope();
    const double slope2 = other.getSlope();

    if (isinf(slope1) && abs(slope2) < EPSILON) return true;
    if (isinf(slope2) && abs(slope1) < EPSILON) return true;
    if (isinf(slope1) || isinf(slope2)) return false;

    return abs(slope1 * slope2 + 1) < EPSILON;
}

// Finds where two lines cross
Point lineType::findIntersectionPoint(const lineType& other) const {
    double det = a * other.b - other.a * b;
    if (abs(det) < EPSILON) {
        return Point(numeric_limits<double>::infinity(),
            numeric_limits<double>::infinity());
    }

    double x = (b * other.c - other.b * c) / det;
    double y = (other.a * c - a * other.c) / det;
    return Point(x, y);
}

// Calculate distance between two points
double calculateDistance(const Point& p1, const Point& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// Canvas ipmlementation, this is for drawing our shapes and lines.
Canvas::Canvas() : xMin(-10), xMax(10), yMin(-10), yMax(10) {
    clear();
}
// Wipes the canvas clean and draws coordinate axes
void Canvas::clear() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[y][x] = ' ';
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        grid[HEIGHT / 2][x] = '-';
    }
    for (int y = 0; y < HEIGHT; y++) {
        grid[y][WIDTH / 2] = '|';
    }
    grid[HEIGHT / 2][WIDTH / 2] = '+';
}
// Converts math coordinates to screen position and plots a point
void Canvas::plotPoint(double x, double y, char symbol) {
    int screenX = static_cast<int>((x - xMin) * (WIDTH - 1) / (xMax - xMin));
    int screenY = static_cast<int>((yMax - y) * (HEIGHT - 1) / (yMax - yMin));
    // Only plot if point is actually on our screen
    if (screenX >= 0 && screenX < WIDTH && screenY >= 0 && screenY < HEIGHT) {
        grid[screenY][screenX] = symbol;
    }
}
// Draws a full line on our canvas - handles vertical, horizontal, and sloped lines
void Canvas::plotLine(const lineType& line, char symbol) {
    double a = line.getA();
    double b = line.getB();
    double c = line.getC();

    if (abs(b) < EPSILON) {
        if (abs(a) < EPSILON) return;
        double x = static_cast<double>(c) / static_cast<double>(a); // Fixed overflow
        for (double y = yMin; y <= yMax; y += 0.3) {
            plotPoint(x, y, symbol);
        }
        return;
    }

    if (abs(a) < EPSILON) {
        double y = static_cast<double>(c) / static_cast<double>(b); // Fixed overflow
        for (double x = xMin; x <= xMax; x += 0.3) {
            plotPoint(x, y, symbol);
        }
        return;
    }

    for (double x = xMin; x <= xMax; x += 0.3) {
        double y = (static_cast<double>(c) - static_cast<double>(a) * x) / static_cast<double>(b); // Fixed overflow
        plotPoint(x, y, symbol);
    }
}
// Draws a line segment between two points, for making the shapes
void Canvas::plotSegment(const Point& start, const Point& end, char symbol) {
    double dx = end.x - start.x;
    double dy = end.y - start.y;
    double steps = max(abs(dx), abs(dy)) * 3; 
    steps = max(steps, 50.0); 

    for (double t = 0; t <= 1; t += 1.0 / steps) {
        double x = start.x + dx * t;
        double y = start.y + dy * t;
        plotPoint(x, y, symbol);
    }
}
// Shows our ASCII display
void Canvas::display() const {
    cout << string(WIDTH + 2, '-') << endl;
    for (int y = 0; y < HEIGHT; y++) {
        cout << '|';
        for (int x = 0; x < WIDTH; x++) {
            cout << grid[y][x];
        }
        cout << '|' << endl;
    }
    cout << string(WIDTH + 2, '-') << endl;
}

// Helper function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
// Makes the program wait until user hits Enter
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
// Makes our menus look nice with a header
void displayHeader(const string& title) {
    clearScreen();
    cout << "==================================" << endl;
    cout << "          " << title << endl;
    cout << "==================================" << endl << endl;
}
// Shows all our lines nicely formatted in sets and makes it easy to see what lines we're working with
void Display(const std::vector<std::vector<lineType>>& allLines) {
    for (size_t i = 0; i < allLines.size(); i++) {
        cout << "Line Set " << i + 1 << endl << "_______________________\n";
        for (size_t j = 0; j < allLines[i].size(); j++) {
            cout << allLines[i][j].getA() << "x + " << allLines[i][j].getB()
                << "y = " << allLines[i][j].getC() << endl;
        }
        cout << endl;
    }
}
// Makes sure users type actual numbers and not random stuff
int getValidIntegerInput() {
    int input;
    string line;

    while (true) {
        if (getline(cin, line)) {
            stringstream ss(line);
            if (ss >> input && !(ss >> line)) {
                return input;
            }
        }
        cout << "Invalid input. Please enter a number: ";
    }
}

// Shows all the lines in each set so we can keep track of what we're working with
void displayVisualization(const vector<lineType>& lines) {
    if (lines.size() != 4) return;

    Canvas canvas; // Create our drawing canvas
    vector<Point> allIntersections; // Will store all points where lines cross
    vector<vector<Point>> lineIntersections(4); // Keeps track of which intersections belong to which line

    // First find all the places where any two lines cross
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = 0; j < lines.size(); j++) {
            if (i != j && !lines[i].isParallel(lines[j])) {
                Point p = lines[i].findIntersectionPoint(lines[j]);
                if (!isinf(p.x) && !isinf(p.y)) {
                    allIntersections.push_back(p);
                    lineIntersections[i].push_back(p);
                    lineIntersections[j].push_back(p);
                }
            }
        }
    }

    // Find parallel lines
    int line1 = -1, line2 = -1;
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = i + 1; j < lines.size(); j++) {
            if (lines[i].isParallel(lines[j])) {
                line1 = i;
                line2 = j;
                break;
            }
        }
        if (line1 != -1) break;
    }

    vector<Point> orderedPoints;

    // If we found parallel lines (trapezoid)
    if (line1 != -1 && line2 != -1) {
        vector<Point> points1, points2;

        // Get points for first parallel line
        for (const Point& p : allIntersections) {
            bool onLine = false;
            for (const Point& lp : lineIntersections[line1]) {
                if (abs(p.x - lp.x) < EPSILON && abs(p.y - lp.y) < EPSILON) {
                    points1.push_back(p);
                    onLine = true;
                    break;
                }
            }
            if (!onLine) {
                for (const Point& lp : lineIntersections[line2]) {
                    if (abs(p.x - lp.x) < EPSILON && abs(p.y - lp.y) < EPSILON) {
                        points2.push_back(p);
                        break;
                    }
                }
            }
        }

        // Sort points from left to right on each parallel line
        sort(points1.begin(), points1.end(),
            [](const Point& a, const Point& b) { return a.x < b.x; });
        sort(points2.begin(), points2.end(),
            [](const Point& a, const Point& b) { return a.x < b.x; });

        // Arrange points in correct order for trapezoid
        if (!points1.empty() && !points2.empty()) {
            // Start with leftmost point of first parallel line
            orderedPoints.push_back(points1.front());

            // Add leftmost point of second parallel line
            orderedPoints.push_back(points2.front());

            // Add rightmost point of second parallel line
            orderedPoints.push_back(points2.back());

            // Add rightmost point of first parallel line
            orderedPoints.push_back(points1.back());

            // Closes the shape
            orderedPoints.push_back(points1.front());
        }
    }
    // This is if something went wrong finding the points

    if (orderedPoints.empty()) {
        cout << "Could not determine shape vertices." << endl;
        return;
    }

    // Figure out how big to make our drawing
    double xMin = orderedPoints[0].x;
    double xMax = orderedPoints[0].x;
    double yMin = orderedPoints[0].y;
    double yMax = orderedPoints[0].y;
    // Find the boundaries of our shape
    for (const Point& p : orderedPoints) {
        xMin = min(xMin, p.x);
        xMax = max(xMax, p.x);
        yMin = min(yMin, p.y);
        yMax = max(yMax, p.y);
    }

    // Add some padding (15%) so shape isn't right at the edges
    double xPadding = (xMax - xMin) * 0.15;
    double yPadding = (yMax - yMin) * 0.15;
    canvas.xMin = xMin - xPadding;
    canvas.xMax = xMax + xPadding;
    canvas.yMin = yMin - yPadding;
    canvas.yMax = yMax + yPadding;

    canvas.clear();

    // Draw each side with a different symbol to make it easier to see
    const char symbols[] = { '#', '@', '*', '+' };
    for (size_t i = 0; i < orderedPoints.size() - 1; i++) {
        canvas.plotSegment(orderedPoints[i], orderedPoints[i + 1], symbols[i % 4]);
    }
    // Show which symbol means which side
    cout << "Shape Visualization:\n" << endl;
    for (size_t i = 0; i < 4; i++) {
        cout << "Segment " << (i + 1) << ": " << symbols[i] << endl;
    }
    cout << endl;

    canvas.display(); // Displays the drawing
}

// Figures out where two lines cross and tells us about it
void findIntersection(const lineType& line1, const lineType& line2) {
    Point intersection = line1.findIntersectionPoint(line2);
    if (isinf(intersection.x) || isinf(intersection.y)) {
        cout << "The lines are parallel and don't intersect." << endl;
    }
    else {
        cout << "The lines intersect at point: (" << fixed << setprecision(3)
            << intersection.x << ", " << intersection.y << ")" << endl;
    }
}
// Tells us everything we want to know about how two lines relate to each other
void checkLines(const lineType& line1, const lineType& line2) {
    bool isParallel = line1.isParallel(line2);
    bool isPerpendicular = line1.isPerpendicular(line2);
    Point intersection = line1.findIntersectionPoint(line2);

    if (isParallel) {
        cout << "The lines are parallel." << endl;
    }
    else if (isPerpendicular) {
        cout << "The lines are perpendicular." << endl;
        if (!isinf(intersection.x) && !isinf(intersection.y)) {
            cout << "The lines intersect at point: (" << fixed << setprecision(3)
                << intersection.x << ", " << intersection.y << ")" << endl;
        }
    }
    else {
        cout << "The lines are neither parallel nor perpendicular." << endl;
        if (!isinf(intersection.x) && !isinf(intersection.y)) {
            cout << "The lines intersect at point: (" << fixed << setprecision(3)
                << intersection.x << ", " << intersection.y << ")" << endl;
        }
    }

    cout << "\nVisualization:\n" << endl;
    cout << "Line 1: /" << endl;
    cout << "Line 2: \\" << endl << endl;

    Canvas canvas;

    if (isParallel) {
        if (abs(line1.getB()) < EPSILON) {  
            double x1 = line1.getC() / line1.getA();
            double x2 = line2.getC() / line2.getA();
            canvas.xMin = min(x1, x2) - 5;
            canvas.xMax = max(x1, x2) + 5;
            canvas.yMin = -10;
            canvas.yMax = 10;
        }
        else {  
            canvas.xMin = -10;
            canvas.xMax = 10;
            double y1 = (-line1.getA() * canvas.xMin + line1.getC()) / line1.getB();
            double y2 = (-line2.getA() * canvas.xMin + line2.getC()) / line2.getB();
            canvas.yMin = min(y1, y2) - 5;
            canvas.yMax = max(y1, y2) + 5;
        }
    }
    // For crossing lines, center the view on where they meet
    else if (!isinf(intersection.x) && !isinf(intersection.y)) {
        double padding = 8.0;  // Increased padding to show more of the lines
        canvas.xMin = intersection.x - padding;
        canvas.xMax = intersection.x + padding;
        canvas.yMin = intersection.y - padding;
        canvas.yMax = intersection.y + padding;
    }

    canvas.clear();

    // Draw both lines
    canvas.plotLine(line1, '/');
    canvas.plotLine(line2, '\\');

    // If they cross, mark the spot!
    if (!isinf(intersection.x) && !isinf(intersection.y)) {
        string coords = "(" + to_string(static_cast<int>(intersection.x)) +
            "," + to_string(static_cast<int>(intersection.y)) + ")";
        int screenX = static_cast<int>((intersection.x - canvas.xMin) * (canvas.WIDTH - 1)
            / (canvas.xMax - canvas.xMin));
        int screenY = static_cast<int>((canvas.yMax - intersection.y) * (canvas.HEIGHT - 1)
            / (canvas.yMax - canvas.yMin));

        // Show the coordinates near the intersection
        for (size_t i = 0; i < coords.length(); i++) {
            if (screenX + 1 + i < canvas.WIDTH) {
                canvas.grid[screenY][screenX + 1 + i] = coords[i];
            }
        }
    }

    canvas.display();
}
// Shows everything about a shape made by 4 lines
void showShape(const vector<lineType>& lines) {
    if (lines.size() != 4) {
        cout << "Error: Need exactly 4 lines to analyze a shape!" << endl;
        return;
    }

    cout << "\nInformation about the lines:" << endl;
    cout << "----------------" << endl;
    for (size_t i = 0; i < lines.size(); i++) {
        cout << "Line " << (i + 1) << ": ";
        if (abs(lines[i].getB()) < EPSILON) {
            cout << "Vertical line";
        }
        else {
            cout << "Slope = " << fixed << setprecision(3) << lines[i].getSlope();
        }
        cout << endl;
    }

    checkQuadrilateral(lines);
    //Shows the drawing of the shape
    cout << "\nVisualization:" << endl;
    displayVisualization(lines);
}

// Menu for comparing different lines
void compareLinesMenu(const vector<vector<lineType>>& allLines) {
    while (true) {
        displayHeader("Compare Lines");
        Display(allLines); 
        int setChoice;
        do {
            cout << "Choose a set of lines (1-" << allLines.size() << "): ";
            setChoice = getValidIntegerInput();
            if (setChoice < 1 || setChoice > static_cast<int>(allLines.size())) {
                cout << "Invalid set number! Please choose between 1 and " << allLines.size() << endl;
            }
        } while (setChoice < 1 || setChoice > static_cast<int>(allLines.size()));

        int line1, line2;
        do {
            cout << "Choose first line (1-4): ";
            line1 = getValidIntegerInput();
            if (line1 < 1 || line1 > 4) {
                cout << "Invalid line number! Please choose between 1 and 4" << endl;
            }
        } while (line1 < 1 || line1 > 4);

        do {
            cout << "Choose second line (1-4): ";
            line2 = getValidIntegerInput();
            if (line2 < 1 || line2 > 4 || line2 == line1) {
                cout << "Invalid line number! Please choose a different line between 1 and 4" << endl;
            }
        } while (line2 < 1 || line2 > 4 || line2 == line1);

        displayHeader("Line Comparison Results");
        checkLines(allLines[setChoice - 1][line1 - 1], allLines[setChoice - 1][line2 - 1]);

        int option;
        do {
            cout << "\nWhat would you like to do?" << endl;
            cout << "1. Compare more lines" << endl;
            cout << "2. Return to main menu" << endl;
            cout << "3. Exit program" << endl;
            cout << "\nChoice: ";

            option = getValidIntegerInput();
            if (option < 1 || option > 3) {
                cout << "Invalid choice! Please enter 1 to compare more lines, 2 to return to main menu, or 3 to exit." << endl;
            }
        } while (option < 1 || option > 3);

        if (option == 2) break;
        if (option == 3) exit(0);
    }
}
// Menu for analyzing shapes
void showShapesMenu(const vector<vector<lineType>>& allLines) {
    while (true) {
        displayHeader("Shape Analysis");
        Display(allLines); 
        int setNumber;
        do {
            cout << "Choose a set of lines (1-" << allLines.size() << "): ";
            setNumber = getValidIntegerInput();
            if (setNumber < 1 || setNumber > static_cast<int>(allLines.size())) {
                cout << "Invalid set number! Please choose between 1 and " << allLines.size() << endl;
            }
        } while (setNumber < 1 || setNumber > static_cast<int>(allLines.size()));

        displayHeader("Shape Analysis Results");
        showShape(allLines[setNumber - 1]);

        int option;
        do {
            cout << "\nWhat would you like to do?" << endl;
            cout << "1. Analyze another shape" << endl;
            cout << "2. Return to main menu" << endl;
            cout << "3. Exit program" << endl;
            cout << "\nChoice: ";

            option = getValidIntegerInput();
            if (option < 1 || option > 3) {
                cout << "Invalid choice! Please enter 1 to analyze another shape, 2 to return to main menu, or 3 to exit." << endl;
            }
        } while (option < 1 || option > 3);

        if (option == 2) break;
        if (option == 3) exit(0);
    }
}
// Gets a line equation from the user, ax + by = c
lineType getLineFromUser(const string& lineNumber) {
    double a, b, c;
    cout << "\nEnter coefficients for " << lineNumber << " (ax + by = c):" << endl;

    do {
        cout << "Enter a: ";
        a = getValidIntegerInput();

        cout << "Enter b: ";
        b = getValidIntegerInput();

        if (abs(a) < EPSILON && abs(b) < EPSILON) {
            cout << "Both a and b cannot be zero. Please enter valid coefficients." << endl;
            continue;
        }

        cout << "Enter c: ";
        c = getValidIntegerInput();
        break;

    } while (true);

    return lineType(a, b, c);
}
// Menu for comparing lines that the user types in
void compareCustomLinesMenu() {
    while (true) {
        displayHeader("Compare Custom Lines");

        lineType line1 = getLineFromUser("first line");
        lineType line2 = getLineFromUser("second line");

        displayHeader("Line Comparison Results");
        checkLines(line1, line2);

        int option;
        do {
            cout << "\nWhat would you like to do?" << endl;
            cout << "1. Compare more lines" << endl;
            cout << "2. Return to main menu" << endl;
            cout << "3. Exit program" << endl;
            cout << "\nChoice: ";

            option = getValidIntegerInput();
            if (option < 1 || option > 3) {
                cout << "Invalid choice! Please enter 1 to compare more lines, 2 to return to main menu, or 3 to exit." << endl;
            }
        } while (option < 1 || option > 3);

        if (option == 2) {
            clearScreen();
            break;
        }
        else if (option == 3) {
            cout << "Thank you for using our program!" << endl;
            exit(0);
        }
    }
}
// Menu for creating and analyzing your own shape
void createCustomShapeMenu() {
    while (true) {
        displayHeader("Create Custom Shape");

        vector<lineType> lines;
        cout << "Enter coefficients for 4 lines to create a quadrilateral." << endl;

        for (int i = 1; i <= 4; i++) {
            lines.push_back(getLineFromUser("line " + to_string(i)));
        }

        displayHeader("Shape Analysis Results");
        showShape(lines);

        int option;
        do {
            cout << "\nWhat would you like to do?" << endl;
            cout << "1. Create another shape" << endl;
            cout << "2. Return to main menu" << endl;
            cout << "3. Exit program" << endl;
            cout << "\nChoice: ";

            option = getValidIntegerInput();
            if (option < 1 || option > 3) {
                cout << "Invalid choice! Please enter 1 to create another shape, 2 to return to main menu, or 3 to exit." << endl;
            }
        } while (option < 1 || option > 3);

        if (option == 2) {
            clearScreen();
            break;
        }
        else if (option == 3) {
            cout << "Thank you for using our program!" << endl;
            exit(0);
        }
    }
}
// The big function that figures out what kind of shape we have!
void checkQuadrilateral(const vector<lineType>& lines) {
    if (lines.size() != 4) {
        cout << "Hey, we need exactly 4 lines to make a quadrilateral!" << endl;
        return;
    }

    // Get all possible intersections first
    vector<Point> allIntersections;
    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = i + 1; j < lines.size(); ++j) {
            Point p = lines[i].findIntersectionPoint(lines[j]);
            if (!isinf(p.x) && !isinf(p.y)) {
                allIntersections.push_back(p);
            }
        }
    }

    // Reorder points to form the quadrilateral
    vector<Point> orderedPoints;
    if (allIntersections.size() >= 4) {
        // Find the topmost point to start
        int topmost = 0;
        for (size_t i = 1; i < allIntersections.size(); i++) {
            if (allIntersections[i].y > allIntersections[topmost].y) {
                topmost = i;
            }
        }
        orderedPoints.push_back(allIntersections[topmost]);

        // Find remaining points based on proximity
        vector<bool> used(allIntersections.size(), false);
        used[topmost] = true;

        for (int i = 0; i < 3; i++) {
            double minDist = numeric_limits<double>::max();
            int nextPoint = -1;

            for (size_t j = 0; j < allIntersections.size(); j++) {
                if (!used[j]) {
                    double dist = calculateDistance(orderedPoints.back(), allIntersections[j]);
                    if (dist < minDist) {
                        minDist = dist;
                        nextPoint = j;
                    }
                }
            }

            if (nextPoint != -1) {
                orderedPoints.push_back(allIntersections[nextPoint]);
                used[nextPoint] = true;
            }
        }
    }

    // Calculate side lengths using ordered points
    vector<double> sideLengths;
    if (orderedPoints.size() == 4) {
        sideLengths.push_back(calculateDistance(orderedPoints[0], orderedPoints[1]));
        sideLengths.push_back(calculateDistance(orderedPoints[1], orderedPoints[2]));
        sideLengths.push_back(calculateDistance(orderedPoints[2], orderedPoints[3]));
        sideLengths.push_back(calculateDistance(orderedPoints[3], orderedPoints[0]));
    }

    // Store original order for display
    vector<double> originalLengths = sideLengths;
    sort(sideLengths.begin(), sideLengths.end());

    // Reorganize lines so parallel pairs are grouped correctly
    vector<lineType> reorderedLines = lines;
    if (!lines[0].isParallel(lines[2])) {
        swap(reorderedLines[1], reorderedLines[2]);
    }

    bool equalSides = (sideLengths.size() == 4) &&
        (abs(sideLengths[0] - sideLengths[3]) < EPSILON);

    bool equalOpposites = (sideLengths.size() == 4) &&
        (abs(sideLengths[0] - sideLengths[1]) < EPSILON) &&
        (abs(sideLengths[2] - sideLengths[3]) < EPSILON);

    bool isParallelogram = reorderedLines[0].isParallel(reorderedLines[2]) &&
        reorderedLines[1].isParallel(reorderedLines[3]);

    bool isRectangle = reorderedLines[0].isPerpendicular(reorderedLines[1]) &&
        reorderedLines[1].isPerpendicular(reorderedLines[2]) &&
        reorderedLines[2].isPerpendicular(reorderedLines[3]) &&
        reorderedLines[3].isPerpendicular(reorderedLines[0]) &&
        isParallelogram &&
        equalOpposites;

    bool isRhombus = reorderedLines[0].isParallel(reorderedLines[2]) &&
        reorderedLines[1].isParallel(reorderedLines[3]) &&
        equalSides;

    bool isSquare = reorderedLines[0].isPerpendicular(reorderedLines[1]) &&
        reorderedLines[1].isPerpendicular(reorderedLines[2]) &&
        reorderedLines[2].isPerpendicular(reorderedLines[3]) &&
        reorderedLines[3].isPerpendicular(reorderedLines[0]) &&
        isParallelogram &&
        equalSides;

    bool isTrapezoid = (reorderedLines[0].isParallel(reorderedLines[2]) &&
        !reorderedLines[1].isParallel(reorderedLines[3])) ||
        (reorderedLines[1].isParallel(reorderedLines[3]) &&
            !reorderedLines[0].isParallel(reorderedLines[2]));

    // Output results
    cout << "\nHere is the information about the shape you chose:" << endl;
    if (sideLengths.size() == 4) {
        cout << "The sides lengths are: ";
        for (double length : originalLengths) {
            cout << fixed << setprecision(3) << length << " ";
        }
        cout << endl;
    }

    if (isSquare) cout << "The shape you have chosen is a square! (all sides equal and all angles 90 degree)" << endl;
    else if (isRectangle) cout << "The shape you have chosen is a rectangle! (opposite sides equal and all angles 90 degree)" << endl;
    else if (isRhombus) cout << "The shape you have chosen is a rhombus! (all sides equal but angles aren't 90 degree)" << endl;
    else if (isParallelogram) cout << "The shape you have chosen is a parallelogram! (opposite sides are equal and but angles aren't 90 degree)" << endl;
    else if (isTrapezoid) cout << "The shape you have chosen is a trapezoid! (there is only one pair of parallel sides)" << endl;
    else cout << "Looks like the shape you have chosen, is an irregular quadrilateral!" << endl;
}
