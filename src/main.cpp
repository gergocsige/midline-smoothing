#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

struct Point {
    // Point cordinates
    double x, y;

    // Operator overload for +=-ing points ("this" is a pointer to the point itself)
    Point& operator+=(const Point& other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
};

// Operator overload for basic arithmetic with points and constants
Point operator+( const Point& p1, const Point& p2) { return {p1.x + p2.x, p1.y + p2.y}; }
Point operator*( const Point& p1, double m) { return {p1.x * m, p1.y * m}; }
Point operator/( const Point& p1, double d) { return {p1.x / d, p1.y / d}; }

// Operator overload for printing a single point to stdout
ostream& operator<<(ostream& os, const Point& pt) {
    os << pt.x << " " << pt.y;
    return os;
};
// Operator overload for getting a point's coordinates from stdin
istream& operator>>(istream& is, Point& pt) {
    is >> pt.x >> pt.y;
    return is;
};
// Operator overload for printing a vector of points
ostream& operator<<(ostream& os, const vector<Point>& pts) {
    for(const Point& pt : pts) {
        os << pt << endl;
    } return os;
};

/**
 * @brief An implementation of a moving average smoothing algorithm
 * 
 * @param jagged The source vector of points to be smoothed
 * @param smooth The destination vector of points
 */
void smooth_moving_avg(const vector<Point>& jagged, vector<Point>& smooth) {
    int len = jagged.size();
    // If len < 3 just copy over the vector and return
    if (len < 3) {
        smooth = jagged;
        return;
    }
    // Copy over the first and last element without changes
    smooth[0] = jagged[0];
    smooth[len-1] = jagged[len-1];
    // Average out point "triplets"
    for( int i = 1; i < len - 1; ++i ) {
        smooth[i] = ( jagged[i-1] + jagged[i] + jagged[i+1] ) / 3;
    }
}

// Global constant vector of weights to be used in weighted smoothing
const vector<double> weights = { 0.2, 1.6, 3.8, 4.9, 3.8, 1.6, 0.2 };

/**
 * @brief An implementation of a weighted smoothing algorithm
 * 
 * @param jagged The source vector of points to be smoothed
 * @param smooth The destination vector of points
 */
void smooth_weighted(const vector<Point>& jagged, vector<Point>& smooth) {
    int len = jagged.size();
    // Loop over every point
    for( int i = 0; i < len; ++i ) {
        // Accumulator variables for both the numerator and denominator
        Point weighted_pointsum = {0.0, 0.0};
        double weightsum = 0.0;
        // Start and end points for the smoothing kernel to avoid indexing out-of-bounds
        int start = max(0, i-3);
        int end = min(len-1, i+3);
        // Sum up the weighted neighbors (and self) and the weights for the current point
        for (int j = start; j <= end; ++j) {
            int k = j - i + 3;
            weighted_pointsum += jagged[j] * weights[k];
            weightsum += weights[k];
        }
        // Calculate smoothed point coordinates
        smooth[i] = weighted_pointsum / weightsum;
    }
}

/**
 * @brief Inline function to clear bad stdin contents after a failed fetch of it
 * Clears an unlimited ammount of characters form input stream until a newline is encountered
 */
inline void remove_bad_stdin() { cin.ignore(numeric_limits<streamsize>::max(), '\n'); };

/**
 * @brief Get the number of points from stdin
 * 
 * @param n the variable to deposit number into
 * @return true in case the input fetching succeeded
 * @return false in case the input fetching failed
 */
bool get_num_of_points(int& n) {
    // Uncomment for interactive use
    // cout << "Number of points in the trajectory: ";
    cin >> n;
    // If input fetching fails
    if (cin.fail()) {
        // Safely exiting program in case input stream ended abruptly
        if (cin.eof()) exit(1);
        // Clear locked state of cin
        cin.clear();
        // Remove extraneous input left in buffer to avoid further error
        remove_bad_stdin();
        return false;
    // Number of points has to be positive
    } else if (n <= 0 || n >= 1000000) {
        remove_bad_stdin();
        return false;
    }
    return true;
}

/**
 * @brief Get the coordinates of the points from stdin
 * 
 * @param points the vector to deposit points into
 * @return true in case the input fetching succeeded
 * @return false in case the input fetching failed
 */
bool get_points(vector<Point>& points) {
    for(Point& pt : points) {
        // Uncomment for interactive use
        // cout << "Point x and y coordinate: ";
        cin >> pt;
        // If input fetching fails
        if (cin.fail()) {
            // Safely exiting program in case input stream ended abruptly
            if (cin.eof()) exit(1);
            // Clear locked state of cin
            cin.clear();
            // Remove extraneous input left in buffer to avoid further error
            remove_bad_stdin();
            return false;
        }
    }
    return true;
}

/**
 * @brief Get the type of smoothing to be used later
 * 
 * @param type the variable to deposit type into
 * @return true in case the input fetching succeeded
 * @return false in case the input fetching failed
 */
bool get_smoothing_type(string& type) {
    // Uncomment for interactive use
    // cout << "Type of smoothing (NONE, MOVING_AVG, WEIGHTED): ";
    cin >> type;
    // Safely exiting program in case input stream ends abruptly
    if (cin.fail() && cin.eof()) exit(1);
    // If the input is none of these then fail
    if (type != "NONE" && type != "MOVING_AVG" && type != "WEIGHTED") {
        // Remove extraneous input left in buffer to avoid further error
        remove_bad_stdin();
        return false;
    }
    // No further validation required as string type variable can take anything from stdin
    return true;
}

int main(void) {
    // Get the number of points in the trajectory
    int n;
    while (!get_num_of_points(n)) { cout << "Invalid input, try again!" << endl; }
    
    // Create source and destination point vectors
    vector<Point> points(n);
    vector<Point> smooth(n);
    // Get point coordinates for source vector
    while (!get_points(points)) { cout << "Invalid input, try again (all previous points ignored)" << endl; }

    // Get smoothing type to be used
    string type;
    while (!get_smoothing_type(type)) { cout << "Invalid smoothing type, try again!" << endl; }

    // Use appropriate amoothing algorithm, then output the smoothed vector of points
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
