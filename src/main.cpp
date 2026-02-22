#include <iostream>
using namespace std;

struct Point {
    double x, y;

    Point& operator+=(const Point& other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
};

Point operator+( const Point& p1, const Point& p2) { return {p1.x + p2.x, p1.y + p2.y}; }
Point operator+( const Point& p1, double c) { return {p1.x + c, p1.y + c}; }

int main(void) {
    return 0;
}