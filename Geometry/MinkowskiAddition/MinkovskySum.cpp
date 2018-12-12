#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>

struct Point {
    double x;
    double y;

    Point(double x, double y) :
            x(x),
            y(y) {}

    Point() :
            x(0),
            y(0) {}
};

Point operator+(const Point &left, const Point &right) {
    Point result = Point(left.x + right.x, left.y + right.y);
    return result;
}

Point operator-(const Point &left, const Point &right) {
    Point result = Point(left.x - right.x, left.y - right.y);
    return result;
}

Point operator/(const Point &left, int right) {
    Point result = Point(left.x / right, left.y / right);
    return result;
}


double angle(const Point &first, const Point &second) {
    double dotProduct = first.x * second.x + first.y * second.y;
    double determinant = first.x * second.y - first.y * second.x;
    double angle = atan2(determinant, dotProduct);
    return angle;
}

struct Polygon {
    std::vector<Point> vertices;

    explicit Polygon(const std::vector<Point> &givenVertices) {
        assert(givenVertices.size() >= 3);
        vertices = givenVertices;
        size_t leftmost = 0;
        double leftmostY = vertices[0].y;
        for (size_t i = 1; i < vertices.size(); ++i) {
            if (vertices[i].x < vertices[leftmost].x ||
                (vertices[i].x == vertices[leftmost].x && vertices[i].y < leftmostY)) {
                leftmost = i;
                leftmostY = vertices[i].y;
            }
        }
        std::vector<Point> buff = vertices;
        for (size_t i = leftmost; i < vertices.size() + leftmost; ++i) {
            buff[i - leftmost] = vertices[i % vertices.size()];
        }
        vertices = buff;
    }
};


Polygon MinkovskySum(const Polygon &first, const Polygon &second) {
    std::vector<Point> ans;
    size_t firstIndex = 0, secondIndex = 0;
    std::vector<Point> firstPolygon = first.vertices;
    std::vector<Point> secondPolygon = second.vertices;
    firstPolygon.push_back(first.vertices[0]);
    firstPolygon.push_back(first.vertices[1]);
    secondPolygon.push_back(second.vertices[0]);
    secondPolygon.push_back(second.vertices[1]);
    while (secondIndex < secondPolygon.size() - 2 || firstIndex < firstPolygon.size() - 2) {
        ans.push_back(firstPolygon[firstIndex] + secondPolygon[secondIndex]);
        double angleBetweenVectors = angle(firstPolygon[firstIndex + 1] - firstPolygon[firstIndex],
                                           secondPolygon[secondIndex + 1] - secondPolygon[secondIndex]);
        if (angleBetweenVectors < 0) {
            ++firstIndex;
        } else if (angleBetweenVectors > 0) {
            ++secondIndex;
        } else {
            ++firstIndex;
            ++secondIndex;
        }
    }
    Polygon minkovskySum(ans);
    return minkovskySum;
}

double triangleArea(const Point &a, const Point &b, const Point &c) {
    double s = ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) / 2;
    return fabs(s);
}

bool isPointInsidePolygon(const Point &point, const Polygon &polygon) {
    double polygonArea = 0, triangledByPointArea = 0;
    for (size_t i = 1; i < polygon.vertices.size() - 1; ++i) {
        polygonArea += triangleArea(polygon.vertices[0], polygon.vertices[i], polygon.vertices[i + 1]);
    }
    for (size_t i = 0; i < polygon.vertices.size(); ++i) {
        triangledByPointArea += triangleArea(point, polygon.vertices[i],
                                             polygon.vertices[(i + 1) % polygon.vertices.size()]);
    }

    return fabs(polygonArea - triangledByPointArea) <
           std::numeric_limits<double>::epsilon() * std::max(fabs(polygonArea), fabs(triangledByPointArea));
}

int main() {
    int n, m;
    std::vector<Point> points;
    std::cin >> n;
    double x, y;
    while (n--) {
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }
    Polygon a(points);
    points.clear();
    std::cin >> m;
    while (m--) {
        std::cin >> x >> y;
        points.emplace_back(-x, -y);
    }
    Polygon b(points);
    std::cout << (isPointInsidePolygon(Point(0, 0), MinkovskySum(a, b)) ? "YES" : "NO");
}