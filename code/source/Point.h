#ifndef POINT_H
#define POINT_H

#include <string>

class Point
{
private:
    float x;
    float y;
    float z;
public:
    Point() {};
    Point(float x, float y, float z);
    float GetX() {return x;};
    float GetY() {return y;};
    float GetZ() {return z;};
    void SetX(float x) {this->x = x;};
    void SetY(float y) {this->y = y;};
    void SetZ(float z) {this->z = z;};
    std::string To_string();
    void Clone(Point &p);
    bool Is_equal(Point &p);
    float Distance_from(Point &p);
    ~Point();

};

Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

std::string Point::To_string()
{
    std::string result = "X= " + std::to_string(x) + " Y= " + std::to_string(y) + " Z= " + std::to_string(z);
    return(result);
}

void Point::Clone(Point &p)
{
    this->x = p.GetX();
    this->y = p.GetY();
    this->z = p.GetZ();
}

bool Point::Is_equal(Point &p)
{
    if ((p.GetX() == x) && (p.GetY() == y) && (p.GetZ() == z)) return true;
    return false;
}

float Point::Distance_from(Point &p){
    float distance = sqrt(pow(x-p.GetX(), 2) +
            pow(y-p.GetY(), 2) +
            pow(z-p.GetZ(), 2));
    return distance;
}

Point::~Point()
{
}

#endif