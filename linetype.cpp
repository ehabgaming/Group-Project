#include "linetype.h"
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <string>

using namespace std;

const double EPSILON = 1e-9;

// Forward declarations for new functions
void compareCustomLinesMenu();
void createCustomShapeMenu();
lineType getLineFromUser(const std::string& lineNumber);


int getValidIntegerInput() {
    int input;
    string line;

    while (true) {
        if (getline(cin, line)) {
            stringstream ss(line);
            if (ss >> input && !(ss >> line)) {  // Check if we got exactly one integer
                return input;
            }
        }
        cout << "Invalid input. Please enter a number: ";
    }
}




void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void displayHeader(const string& title) {
    clearScreen();
    cout << "==================================" << endl;
    cout << "          " << title << endl;
    cout << "==================================" << endl << endl;
}

lineType::lineType(double a, double b, double c) : a(a), b(b), c(c) {}

double lineType::getA() const { return a; }
double lineType::getB() const { return b; }
double lineType::getC() const { return c; }

double lineType::getSlope() const {
    if (abs(b) < EPSILON) {
        return numeric_limits<double>::infinity();
    }
    return -a / b;
}

void Display(const std::vector<std::vector<lineType>>& allLines) // Displays all lines in sets
{
    for (size_t i = 0; i < allLines.size(); i++)
    {
        cout <<"Line Set " << i + 1 << endl << "_______________________\n";
        for (size_t j = 0; j < allLines[i].size(); j++)
        {
            cout << allLines[i][j].getA() << "x + " << allLines[i][j].getB() << "y = " << allLines[i][j].getC() << endl;
        }
        cout << endl;
    }
}


bool lineType::isParallel(const lineType& other) const {
    if (abs(b) < EPSILON && abs(other.b) < EPSILON) {
        return true;
    }
    return abs(getSlope() - other.getSlope()) < EPSILON;
}

bool lineType::isPerpendicular(const lineType& other) const {
    const double slope1 = getSlope();
    const double slope2 = other.getSlope();

    if (isinf(slope1) && abs(slope2) < EPSILON) return true;
    if (isinf(slope2) && abs(slope1) < EPSILON) return true;
    if (isinf(slope1) || isinf(slope2)) return false;

    return abs(slope1 * slope2 + 1) < EPSILON;
}

void checkLines(const lineType& line1, const lineType& line2) {
    if (line1.isParallel(line2)) {
        cout << "The lines are parallel." << endl;
    }
    else if (line1.isPerpendicular(line2)) {
        cout << "The lines are perpendicular." << endl;
        findIntersection(line1, line2);
    }
    else {
        cout << "The lines are neither parallel nor perpendicular." << endl;
        findIntersection(line1, line2);
    }
}

void findIntersection(const lineType& line1, const lineType& line2) {
    double a1 = line1.getA();
    double b1 = line1.getB();
    double c1 = line1.getC();

    double a2 = line2.getA();
    double b2 = line2.getB();
    double c2 = line2.getC();

    double determinant = a1 * b2 - a2 * b1;

    if (abs(determinant) < EPSILON) {
        cout << "The lines are parallel and don't cross." << endl;
    }
    else {
        double x = (-(b1 * c2 - b2 * c1)) / determinant;
        double y = (-(a2 * c1 - a1 * c2)) / determinant;
        cout << "The lines intersect at point: (" << fixed << setprecision(3)
            << x << ", " << y << ")" << endl;
    }
}

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

double calculateDistance(const Point& p1, const Point& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

void showShape(const vector<lineType>& lines) {
    if (lines.size() != 4) {
        cout << "Error: Need exactly 4 lines to analyze a shape!" << endl;
        return;
    }

    cout << "Analyzing the shape formed by these 4 lines..." << endl;

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

    cout << "\nParallel Lines:" << endl;
    cout << "--------------" << endl;
    bool hasParallel = false;
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = i + 1; j < lines.size(); j++) {
            if (lines[i].isParallel(lines[j])) {
                cout << "Lines " << (i + 1) << " and " << (j + 1) << " are parallel" << endl;
                hasParallel = true;
            }
        }
    }
    if (!hasParallel) cout << "No parallel lines found." << endl;

    cout << "\nPerpendicular Lines:" << endl;
    cout << "------------------" << endl;
    bool hasPerpendicular = false;
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = i + 1; j < lines.size(); j++) {
            if (lines[i].isPerpendicular(lines[j])) {
                cout << "Lines " << (i + 1) << " and " << (j + 1) << " are perpendicular" << endl;
                hasPerpendicular = true;
            }
        }
    }
    if (!hasPerpendicular) cout << "No perpendicular lines found." << endl;

    cout << "\nShape Analysis:" << endl;
    cout << "--------------" << endl;
    checkQuadrilateral(lines);
}

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
            cout << "Choose first line to compare (1-4): ";
            line1 = getValidIntegerInput();
            if (line1 < 1 || line1 > 4) {
                cout << "Invalid line number! Please choose between 1 and 4" << endl;
            }
        } while (line1 < 1 || line1 > 4);

        do {
            cout << "Choose second line to compare (1-4): ";
            line2 = getValidIntegerInput();
            if (line2 < 1 || line2 > 4 || line2 == line1) {
                cout << "Invalid line number! Please choose a different line between 1 and 4" << endl;
            }
        } while (line2 < 1 || line2 > 4 || line2 == line1);

        displayHeader("Line Comparison Results");
        checkLines(allLines[setChoice - 1][line1 - 1], allLines[setChoice - 1][line2 - 1]);

        cout << "\nWhat would you like to do?" << endl;
        cout << "1. Compare more lines" << endl;
        cout << "2. Return to main menu" << endl;
        cout << "3. Exit program" << endl;
        cout << "\nChoice: ";

        int option;
        do {
            option = getValidIntegerInput();
            if (option < 1 || option > 3) {
                cout << "Invalid choice! Please choose between 1 and 3: ";
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

        cout << "\nWhat would you like to do?" << endl;
        cout << "1. Analyze another shape" << endl;
        cout << "2. Return to main menu" << endl;
        cout << "3. Exit program" << endl;
        cout << "\nChoice: ";

        int option;
        do {
            option = getValidIntegerInput();
            if (option < 1 || option > 3) {
                cout << "Invalid choice! Please choose between 1 and 3: ";
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

void compareCustomLinesMenu() {
    while (true) {
        displayHeader("Compare Custom Lines");

        lineType line1 = getLineFromUser("first line");
        lineType line2 = getLineFromUser("second line");

        displayHeader("Line Comparison Results");
        checkLines(line1, line2);

        cout << "\nWhat would you like to do?" << endl;
        cout << "1. Compare more lines" << endl;
        cout << "2. Return to main menu" << endl;
        cout << "3. Exit program" << endl;
        cout << "\nChoice: ";

        int option;
        do {
            option = getValidIntegerInput();
            if (option < 1 || option > 3) {
                cout << "Invalid choice! Please choose between 1 and 3: ";
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

        cout << "\nWhat would you like to do?" << endl;
        cout << "1. Create another shape" << endl;
        cout << "2. Return to main menu" << endl;
        cout << "3. Exit program" << endl;
        cout << "\nChoice: ";

        int option;
        do {
            option = getValidIntegerInput();
            if (option < 1 || option > 3) {
                cout << "Invalid choice! Please choose between 1 and 3: ";
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

        // Find remaining three points by distance
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

