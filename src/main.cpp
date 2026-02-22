#include <iostream>
#include <vector>
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
Point operator/( const Point& p1, double d) { return {p1.x / d, p1.y / d}; }

void smooth_moving_avg(const vector<Point>& jagged, vector<Point>& smooth) {
    int len = jagged.size();
    if (len < 3) {
        for( int i = 0; i < len; ++i ) {
            smooth.at(i) = jagged.at(i);
        } return;
    } else {
        smooth.at(0) = jagged.at(0);
        for( int i = 1; i < len - 1; ++i ) {
            smooth.at(i) = ( jagged.at(i-1) + jagged.at(i) + jagged.at(i+1) ) / 3;
        }
        smooth.at(len-1) = jagged.at(len-1);
    }
}

int main(void) {
    int n;
    cin >> n;
    vector<Point> points(n);
    vector<Point> smooth(n);

    for(Point& pt : points) {
        cout << "Enter Point x coordinate: ";
        cin >> pt.x;
        cout << "Enter Point y coordinate: ";
        cin >> pt.y;
    }

    for(Point pt : points) {
        cout << pt.x << " " << pt.y << endl;
    }

    smooth_moving_avg(points, smooth);

    for(Point pt : smooth) {
        cout << pt.x << " " << pt.y << endl;
    }

    return 0;
}
