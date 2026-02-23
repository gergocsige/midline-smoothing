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
Point operator*( const Point& p1, double m) { return {p1.x * m, p1.y * m}; }
Point operator/( const Point& p1, double d) { return {p1.x / d, p1.y / d}; }
// TODO Remove this after testing
ostream& operator<<(ostream& os, const Point& pt) {
    os << "X: " << pt.x << " | Y: " << pt.y;
    return os;
};
// TODO Remove this after testing
ostream& operator<<(ostream& os, const vector<Point>& pts) {
    cout << "-[ Points ]-" << endl << "--------------------" << endl;
    for(Point pt : pts) {
        os << pt << endl;
    }
    return os;
};

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

const vector<double> weights = { 0.2, 1.6, 3.8, 4.9, 3.8, 1.6, 0.2 };

inline int clamp(int n, int low_bound, int high_bound) { return ( n < low_bound ? low_bound : ( n > high_bound ? high_bound : n )); };

void smooth_weighted(const vector<Point>& jagged, vector<Point>& smooth) {
    int len = jagged.size();
    int k = 0;
    double weightsum = 0.0;
    for( int i = 0; i < len; ++i ) {
        smooth.at(i) = {0.0, 0.0};
        for (int j = i-3; j <= i+3; ++j) {
            if (j == clamp(j, 0, len-1)) {
                if (i == j) {
                    cout << i << " <" << endl;
                } else {
                    cout << j << " " << k << endl;
                }
                smooth.at(i) += jagged.at(j) * weights.at(k);
                weightsum += weights.at(k);
            }
            k++;
        } cout << endl; smooth.at(i) = smooth.at(i) / weightsum; k = 0; weightsum = 0.0;
    }
}

int main(void) {
    int n;
    cin >> n;
    vector<Point> points(n);
    vector<Point> smooth(n);

    for(Point& pt : points) {
        cout << "Enter Point x and y coordinate: ";
        cin >> pt.x >> pt.y;
    }

    cout << points << endl;

    smooth_moving_avg(points, smooth);

    cout << smooth << endl;

    smooth_weighted(points, smooth);

    cout << smooth << endl;

    return 0;
}
