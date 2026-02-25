#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;

struct Point {
    double x, y;

    Point& operator+=(const Point& other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    Point& operator/=(const double d) {
        this->x /= d;
        this->y /= d;
        return *this;
    }
};

Point operator+( const Point& p1, const Point& p2) { return {p1.x + p2.x, p1.y + p2.y}; }
Point operator*( const Point& p1, double m) { return {p1.x * m, p1.y * m}; }
Point operator/( const Point& p1, double d) { return {p1.x / d, p1.y / d}; }
// TODO Remove this after testing
ostream& operator<<(ostream& os, const Point& pt) {
    os << "X: " << setprecision(5) << fixed << pt.x << " | Y: " << setprecision(5) << fixed << pt.y;
    return os;
};
istream& operator>>(istream& is, Point& pt) {
    is >> pt.x >> pt.y;
    return is;
};
// TODO Remove this after testing
ostream& operator<<(ostream& os, const vector<Point>& pts) {
    cout << "     -[ Points ]-" << endl << "-----------------------" << endl;
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

void smooth_weighted(const vector<Point>& jagged, vector<Point>& smooth) {
    int len = jagged.size();
    int k = 0;
    double weightsum = 0.0;
    for( int i = 0; i < len; ++i ) {
        smooth.at(i) = {0.0, 0.0};
        for (int j = i-3; j <= i+3; ++j) {
            if (j >= 0 && j <= len - 1) {
                smooth.at(i) += jagged.at(j) * weights.at(k);
                weightsum += weights.at(k);
            } k++;
        } smooth.at(i) /= weightsum; k = 0; weightsum = 0.0;
    }
}

inline void remove_bad_stdin() { cin.ignore(numeric_limits<streamsize>::max(), '\n'); };

bool get_num_of_points(int& n) {
    cout << "Enter the number of points in the trajectory: ";
    cin >> n;
    if (cin.fail()) {
        cin.clear();
        remove_bad_stdin();
        return false;
    } else if (n <= 0) {
        return false;
    }
    remove_bad_stdin();
    return true;
}

bool get_points(vector<Point>& points) {
    for(Point& pt : points) {
        cout << "Enter Point x and y coordinate: ";
        cin >> pt;
        if (cin.fail()) {
            cin.clear();
            remove_bad_stdin();
            return false;
        }
    }
    return true;
}

int main(void) {
    int n;
    while (!get_num_of_points(n)) {
        cout << "Invalid input, try again!" << endl;
    }
    
    vector<Point> points(n);
    vector<Point> smooth(n);

    for(Point& pt : points) {
        cout << "Enter Point x and y coordinate: ";
        cin >> pt;
    }

    cout << endl << "Moving average smoothing:" << endl;

    smooth_moving_avg(points, smooth);

    cout << smooth << endl << "Weighted smoothing:" << endl;

    smooth_weighted(points, smooth);

    cout << smooth << endl;

    return 0;
}
