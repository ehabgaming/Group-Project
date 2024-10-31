#include "linetype.h"
#include <limits>
#include <cmath> // For std::abs

// Constructor to initialize the line coefficients
lineType::lineType(double a, double b, double c) : a(a), b(b), c(c) {}

// Getter methods for line coefficients
double lineType::getA() const { return a; }
double lineType::getB() const { return b; }
double lineType::getC() const { return c; }

// Method to calculate the slope of the line
double lineType::getSlope() const {
    if (b != 0) {
        return -a / b; // Slope calculation
    }
    else {
        return std::numeric_limits<double>::infinity(); // Vertical line
    }
}

// Method to check if this line is parallel to another line
bool lineType::isParallel(const lineType& other) const {
    // Check if both lines are vertical
    if (this->b == 0 && other.b == 0) {
        return true; // Both lines are vertical
    }

    // Compare slopes, allowing for some precision error if needed
    return std::abs(this->getSlope() - other.getSlope()) < 1e-9; // Use a small epsilon value
}

// Method to check if this line is perpendicular to another line
bool lineType::isPerpendicular(const lineType& other) const {
    const double slope1 = this->getSlope();
    const double slope2 = other.getSlope();

    // Check for perpendicular lines
    return (slope1 * slope2 == -1) ||
        (std::isinf(slope1) && slope2 == 0) ||
        (std::isinf(slope2) && slope1 == 0);
}

// Function to check if two lines are parallel or perpendicular
void checkLines(const lineType& line1, const lineType& line2) {
    if (line1.isParallel(line2)) {
        std::cout << "The lines are parallel." << std::endl;
    }
    else if (line1.isPerpendicular(line2)) {
        std::cout << "The lines are perpendicular." << std::endl;
        // Calculate and display the intersection point
        findIntersection(line1, line2);
    }
    else {
        std::cout << "The lines are neither parallel nor perpendicular." << std::endl;
        // Calculate and display the intersection point
        findIntersection(line1, line2);
    }
}

// Function to find the intersection point of two lines
void findIntersection(const lineType& line1, const lineType& line2) {
    double a1 = line1.getA();
    double b1 = line1.getB();
    double c1 = line1.getC();

    double a2 = line2.getA();
    double b2 = line2.getB();
    double c2 = line2.getC();

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        std::cout << "The lines are parallel and do not intersect." << std::endl;
    }
    else {
        double x = (b1 * c2 - b2 * c1) / determinant;
        double y = (a2 * c1 - a1 * c2) / determinant;

        std::cout << "The lines intersect at point: (" << std::fixed << std::setprecision(2) << x << ", " << y << ")" << std::endl;
    }
}

// Function for comparing lines from a chosen set
void compareLinesMenu(const std::vector<std::vector<lineType>>& allLines) {
    while (true) {
        std::cout << "\nChoose a set of lines (1-" << allLines.size() << "): ";
        int setChoice;
        std::cin >> setChoice;

        if (setChoice < 1 || setChoice > allLines.size()) {
            std::cout << "Invalid set number." << std::endl;
            continue;
        }

        std::cout << "Choose two lines to compare (1-4): ";
        int line1, line2;
        std::cin >> line1 >> line2;

        if (line1 < 1 || line1 > 4 || line2 < 1 || line2 > 4 || line1 == line2) {
            std::cout << "Invalid line choice." << std::endl;
            continue;
        }

        checkLines(allLines[setChoice - 1][line1 - 1], allLines[setChoice - 1][line2 - 1]);

        // Options after showing results
        std::cout << "\nWould you like to:" << std::endl;
        std::cout << "1. Compare more lines" << std::endl;
        std::cout << "2. Go back to the main menu" << std::endl;
        std::cout << "3. Exit" << std::endl;

        int option;
        std::cin >> option;

        if (option == 2) break; // Go back to main menu
        else if (option == 3) exit(0); // Exit
    }
}

// Function to show the shape formed by the lines
void showShape(const std::vector<lineType>& lines) {
    std::cout << "Shape formed by the lines:" << std::endl;
    checkQuadrilateral(lines);
}

// Function to show shapes based on user's choice
void showShapesMenu(const std::vector<std::vector<lineType>>& allLines) {
    while (true) {
        std::cout << "\nChoose a set to show the shape (1-" << allLines.size() << "): ";
        int setNumber;
        std::cin >> setNumber;

        if (setNumber < 1 || setNumber > allLines.size()) {
            std::cout << "Invalid set number." << std::endl;
            continue;
        }

        showShape(allLines[setNumber - 1]);

        // Options after showing results
        std::cout << "\nWould you like to:" << std::endl;
        std::cout << "1. Show the shape of another set" << std::endl;
        std::cout << "2. Go back to the main menu" << std::endl;
        std::cout << "3. Exit" << std::endl;

        int option;
        std::cin >> option;

        if (option == 2) break; // Go back to main menu
        else if (option == 3) exit(0); // Exit
    }
}

// Function to check the properties of the quadrilateral formed by the lines
void checkQuadrilateral(const std::vector<lineType>& lines) {
    if (lines.size() != 4) {
        std::cout << "Error: A quadrilateral requires exactly 4 lines." << std::endl;
        return;
    }

    double slope1 = lines[0].getSlope();
    double slope2 = lines[1].getSlope();
    double slope3 = lines[2].getSlope();
    double slope4 = lines[3].getSlope();
    bool equalLengths = true;
    bool length1Equals4 = true; // Assume Line 1 and Line 4 are of equal length
    bool length2Equals3 = true; // Assume Line 2 and Line 3 are of equal length
    // Check for parallelogram (opposite slopes must be equal)
    bool isParallelogram = (slope1 == slope4) && (slope2 == slope3);

    // Check for rectangle (opposite slopes equal and adjacent slopes are perpendicular)
    bool isRectangle =lines[0].isPerpendicular(lines[1]) &&
        lines[0].isPerpendicular(lines[2]) &&
        lines[0].isParallel(lines[3]) &&
        lines[3].isPerpendicular(lines[1]) &&
        lines[3].isPerpendicular(lines[2]) &&
        lines[3].isParallel(lines[0]);

    // Check for rhombus (all sides are equal)
    bool isRhombus = equalLengths &&
        lines[0].isParallel(lines[3]) &&
        lines[3].isParallel(lines[0]);

    // Check for square (must satisfy both rectangle and rhombus conditions)
    bool isSquare = equalLengths &&
        lines[0].isPerpendicular(lines[1]) &&
        lines[0].isPerpendicular(lines[2]) &&
        lines[0].isParallel(lines[3]) &&
        lines[3].isPerpendicular(lines[1]) &&
        lines[3].isPerpendicular(lines[2]) &&
        lines[3].isParallel(lines[0]);
    // Check for trapezoid (at least one pair of opposite sides parallel)
    bool isTrapezoid = (std::abs(slope1) == std::abs(slope4) && slope1 * slope4 < 0) &&
        (slope2 == slope3); 
    // Display the type of quadrilateral
    if (isSquare) {
        std::cout << "It is a square." << std::endl;
    }
    else if (isRectangle) {
        std::cout << "It is a rectangle." << std::endl;
    }
    else if (isRhombus) {
        std::cout << "It is a rhombus." << std::endl;
    }
    else if (isParallelogram) {
        std::cout << "It is a parallelogram." << std::endl;
    }
    else if (isTrapezoid) {
        std::cout << "It is a trapezoid." << std::endl;
    }
    else {
        std::cout << "It is an irregular quadrilateral." << std::endl;
    }
}
