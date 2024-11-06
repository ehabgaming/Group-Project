#ifndef LINETYPE_H
#define LINETYPE_H

#ifdef __cplusplus
extern "C++" {
#endif

#include <vector>
#include <string>

    struct Point {
        double x, y;
        Point(double x = 0, double y = 0) : x(x), y(y) {}
    };

    class lineType {
    private:
        double a, b, c; // Coefficients of the line equation Ax + By + C = 0

    public:
        // Constructor
        lineType(double a, double b, double c);

        // Method to get the slope of the line
        double getSlope() const;

        // Methods to access line coefficients
        double getA() const;
        double getB() const;
        double getC() const;

        bool isParallel(const lineType& other) const;
        bool isPerpendicular(const lineType& other) const;
        Point findIntersectionPoint(const lineType& other) const;
    };

    // Function declarations
    void clearScreen();
    void pauseScreen();
    void displayHeader(const std::string& title);
    int getValidIntegerInput();
    double calculateDistance(const Point& p1, const Point& p2);
    void compareLinesMenu(const std::vector<std::vector<lineType>>& allLines);
    void showShapesMenu(const std::vector<std::vector<lineType>>& allLines);
    void findIntersection(const lineType& line1, const lineType& line2);
    void checkQuadrilateral(const std::vector<lineType>& lines);
    void checkLines(const lineType& line1, const lineType& line2);
    void showShape(const std::vector<lineType>& lines);

#ifdef __cplusplus
}
#endif

#endif
