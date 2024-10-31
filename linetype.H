#ifndef LINETYPE_H
#define LINETYPE_H
#include <vector>
#include <iostream> 
#include <iomanip>  
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
    // Function prototypes for additional functionality
    void checkLines(const lineType& line1, const lineType& line2);
    void findIntersection(const lineType& line1, const lineType& line2);
    void compareLinesMenu(const std::vector<std::vector<lineType>>& allLines);
    void showShape(const std::vector<lineType>& lines);
    void checkQuadrilateral(const std::vector<lineType>& lines);

};
void compareLinesMenu(const std::vector<std::vector<lineType>>& allLines);
void showShapesMenu(const std::vector<std::vector<lineType>>& allLines);
void findIntersection(const lineType& line1, const lineType& line2);
void checkQuadrilateral(const std::vector<lineType>& lines);
#endif // LINETYPE_H
