#include <iostream>
#include <cmath>
#include <functional>
#include <iomanip>

const double EPS = 1e-9;

struct Point {
    double x;
    double y;
    double z;
    Point(double x, double y, double z) :
            x(x),
            y(y),
            z(z)
    {}
};

Point operator +(Point left, Point right) {
    Point result = Point(left.x + right.x, left.y + right.y, left.z + right.z);
    return result;
}

Point operator -(Point left, Point right) {
    Point result = Point(left.x - right.x, left.y - right.y, left.z - right.z);
    return result;
}

Point operator /(Point left, int right) {
    Point result = Point(left.x / right, left.y / right, left.z / right);
    return result;
}

double distance(const Point& first, const Point& second) {
    double distanceBetweenPoints = (first.x - second.x) * (first.x - second.x) +
                                   (first.y - second.y) * (first.y - second.y) +
                                   (first.z - second.z) * (first.z - second.z);
    distanceBetweenPoints = sqrt(distanceBetweenPoints);
    return distanceBetweenPoints;
}

template <typename T, typename F>
double ternarySearch(T segmentStart, T segmentFinish, const std::function <F(T)> f) {
    while (distance(segmentStart, segmentFinish) > EPS) {
        Point leftPoint = segmentStart + (segmentFinish - segmentStart) / 3;
        Point rightPoint = segmentFinish - (segmentFinish - segmentStart) / 3;
        if (f(leftPoint) < f(rightPoint)) {
            segmentFinish = rightPoint;
        }
        else {
            segmentStart = leftPoint;
        }
    }
    return f(segmentStart);
}

double Solution(Point& firstStart, Point& firstFinish, Point& secondStart, Point& secondFinish) {
    auto g = [=](Point b) {
        return ternarySearch<Point, double>(secondStart, secondFinish, [b](Point c) {
            return distance(b, c);
        });
    };
    double ans = ternarySearch<Point, double>(firstStart, firstFinish, g);
    return ans;
}


int main() {
    double x, y, z;
    std::cin >> x >> y >> z;
    Point firstStart(x, y, z);
    std::cin >> x >> y >> z;
    Point firstFinish(x, y, z);
    std::cin >> x >> y >> z;
    Point secondStart(x, y, z);
    std::cin >> x >> y >> z;
    Point secondFinish(x, y, z);

    double ans = Solution(firstStart, firstFinish, secondStart, secondFinish);

    std::cout << std::fixed << std::setprecision(10);
    std::cout << ans;
    return 0;
}