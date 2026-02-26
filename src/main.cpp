#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
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
Point operator*( const Point& p1, double m) { return {p1.x * m, p1.y * m}; }
Point operator/( const Point& p1, double d) { return {p1.x / d, p1.y / d}; }

ostream& operator<<(ostream& os, const Point& pt) {
    os << pt.x << " " << pt.y;
    return os;
};
istream& operator>>(istream& is, Point& pt) {
    is >> pt.x >> pt.y;
    return is;
};
ostream& operator<<(ostream& os, const vector<Point>& pts) {
    for(Point pt : pts) {
        os << pt << endl;
    } return os;
};

void smooth_moving_avg(const vector<Point>& jagged, vector<Point>& smooth) {
    int len = jagged.size();
    if (len < 3) {
        smooth = jagged;
        return;
    }
    smooth[0] = jagged[0];
    smooth[len-1] = jagged[len-1];
    for( int i = 1; i < len - 1; ++i ) {
        smooth[i] = ( jagged[i-1] + jagged[i] + jagged[i+1] ) / 3;
    }
}

const vector<double> weights = { 0.2, 1.6, 3.8, 4.9, 3.8, 1.6, 0.2 };

void smooth_weighted(const vector<Point>& jagged, vector<Point>& smooth) {
    int len = jagged.size();
    for( int i = 0; i < len; ++i ) {
        Point weighted_pointsum = {0.0, 0.0};
        double weightsum = 0.0;
        int start = max(0, i-3);
        int end = min(len-1, i+3);
        
        for (int j = start; j <= end; ++j) {
            int k = j - i + 3;
            weighted_pointsum += jagged[j] * weights[k];
            weightsum += weights[k];
        }
        smooth[i] = weighted_pointsum / weightsum;
    }
}

inline void remove_bad_stdin() { cin.ignore(numeric_limits<streamsize>::max(), '\n'); };

bool get_num_of_points(int& n) {
    cout << "Number of points in the trajectory: ";
    cin >> n;
    if (cin.fail()) {
        cin.clear();
        remove_bad_stdin();
        return false;
    } else if (n <= 0) {
        return false;
    }
    return true;
}

bool get_points(vector<Point>& points) {
    for(Point& pt : points) {
        cout << "Point x and y coordinate: ";
        cin >> pt;
        if (cin.fail()) {
            cin.clear();
            remove_bad_stdin();
            return false;
        }
    }
    return true;
}

bool get_smoothing_type(string& type) {
    cout << "Type of smoothing (NONE, MOVING_AVG, WEIGHTED): ";
    cin >> type;
    if (type != "NONE" && type != "MOVING_AVG" && type != "WEIGHTED")
        return false;
    return true;
}

int main(void) {
    int n;
    while (!get_num_of_points(n)) { cout << "Invalid input, try again!" << endl; }
    
    vector<Point> points(n);
    vector<Point> smooth(n);
    while (!get_points(points)) { cout << "Invalid input, try again (all previous points ignored)" << endl; }

    string type;
    while (!get_smoothing_type(type)) { cout << "Invalid smoothing type, try again!" << endl; }

    if (type == "MOVING_AVG") {
        smooth_moving_avg(points, smooth);
        cout << smooth;
    } else if (type == "WEIGHTED") {
        smooth_weighted(points, smooth);
        cout << smooth;
    } else {
        smooth = points;
        cout << smooth;
    }

    return 0;
}
