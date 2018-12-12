#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <unordered_set>

const int HASH_VALUE = 10000;

struct Point {
    int number;
    double x;
    double y;
    double z;

    Point(int number, double x, double y, double z) :
            number(number),
            x(x),
            y(y),
            z(z) {}

    Point(double x, double y, double z) :
            number(-1),
            x(x),
            y(y),
            z(z) {}

    Point() = default;

};

Point operator+(const Point &left, const Point &right) {
    Point result = Point(left.x + right.x, left.y + right.y, left.z + right.z);
    return result;
}

Point operator-(const Point &left, const Point &right) {
    Point result = Point(left.x - right.x, left.y - right.y, left.z - right.z);
    return result;
}

Point operator/(const Point &left, double right) {
    Point result = Point(left.x / right, left.y / right, left.z / right);
    return result;
}

double scalarProduct(const Point &first, const Point &scond) {
    double ans = first.x * scond.x + first.y * scond.y + first.z * scond.z;
    return ans;
}

Point vectorProduct(const Point &first, const Point &scond) {
    double x = first.y * scond.z - first.z * scond.y;
    double y = scond.x * first.z - scond.z * first.x;
    double z = first.x * scond.y - scond.x * first.y;
    return {x, y, z};
}

class Face {
public:
    Point first;
    Point second;
    Point third;
    Point normal;

    Face(Point A, Point B, Point C) { //A, B, C - вершины треугольника
        if (A.number > B.number) {
            std::swap(A, B);
        }
        if (A.number > C.number) {
            std::swap(A, C);
        }
        first = A;
        second = B;
        third = C;
    }

    void getNormal(const Point &in) {
        normal = vectorProduct(second - first, third - first);
        if (scalarProduct(normal, in - first) > 0) {
            normal = normal / -1;
        }
        if (scalarProduct(normal, vectorProduct(second - first, third - first)) < 0) {
            std::swap(second, third);
        }
    }

    bool isVisible(const Point &from) {
        return scalarProduct(normal, from - first) > 0;
    }
};

struct Edge {
    Point first;
    Point second;
    long long hash;

    Edge(const Point &first_, const Point &second_) {
        if (first_.number > second_.number) {
            first = second_;
            second = first_;
        } else {
            first = first_;
            second = second_;
        }
        hash = first.number * HASH_VALUE + second.number;
    }
};

class ConvexHull {
public:
    explicit ConvexHull(const std::vector<Point> &points_) : points_(points_) {}

    void makeTetrahedron(const Point &A, const Point &B, const Point &C, const Point &D) { // A, B, C, D - вершины тетраэдра
        faces_.clear();
        faces_.emplace_back(Face(A, B, C));
        faces_.emplace_back(Face(A, B, D));
        faces_.emplace_back(Face(A, C, D));
        faces_.emplace_back(Face(B, C, D));
        pointIn_ = Point((A + B + C + D) / 4);
        for (auto &face : faces_) {
            face.getNormal(pointIn_);
        }
    }

    void addPoint(Point point) {
        std::unordered_set<int> current;
        for (auto facePointer = faces_.begin(); facePointer != faces_.end(); ++facePointer) {
            if ((*facePointer).isVisible(point)) {
                Edge first((*facePointer).first, (*facePointer).second);
                Edge second((*facePointer).first, (*facePointer).third);
                Edge third((*facePointer).third, (*facePointer).second);
                auto isFirst = current.find(first.hash);
                if (isFirst == current.end()) {
                    current.insert(first.hash);
                } else {
                    current.erase(isFirst);
                }
                auto isSecond = current.find(second.hash);
                if (isSecond == current.end()) {
                    current.insert(second.hash);
                } else {
                    current.erase(isSecond);
                }
                auto isThird = current.find(third.hash);
                if (isThird == current.end()) {
                    current.insert(third.hash);
                } else {
                    current.erase(isThird);
                }
                facePointer = faces_.erase(facePointer);
                --facePointer;
            }
        }
        for (int edgeHash : current) {
            Face face(point, points_[edgeHash / HASH_VALUE], points_[edgeHash % HASH_VALUE]);
            face.getNormal(pointIn_);
            faces_.emplace_back(face);
        }
    }

    std::list<Face> getFaces() {
        return faces_;
    }

private:
    std::list<Face> faces_;
    std::vector<Point> points_;
    Point pointIn_;


};

void Solve(const std::vector<Point> &points) {
    ConvexHull convexHull(points);
    convexHull.makeTetrahedron(points[0], points[1], points[2], points[3]);
    for (int i = 4; i < points.size(); ++i) {
        convexHull.addPoint(points[i]);
    }
    auto ans = convexHull.getFaces();
    std::vector<Face> vectorAns;
    std::cout << ans.size() << std::endl;
    for (auto &face : ans) {
        vectorAns.push_back(face);
    }

    std::sort(vectorAns.begin(), vectorAns.end(), [](const Face &first, const Face &second) {
        if (first.first.number != second.first.number) {
            return first.first.number < second.first.number;
        }
        if (first.second.number != second.second.number) {
            return first.second.number < second.second.number;
        }
        return first.third.number < second.third.number;
    });

    for (auto &face : vectorAns) {
        std::cout << 3 << " " << face.first.number << " " << face.second.number << " " << face.third.number << '\n';
    }
}

int main() {
    int testNumber, pointNumber;
    double x, y, z;
    std::cin >> testNumber;
    std::vector<Point> points;
    while (testNumber--) {
        points.clear();
        std::cin >> pointNumber;
        for (int i = 0; i < pointNumber; i++) {
            std::cin >> x >> y >> z;
            auto newPoint = Point(i, x, y, z);
            points.push_back(newPoint);
        }
        Solve(points);
    }
    return 0;
}
