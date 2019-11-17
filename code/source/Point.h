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

Point::~Point()
{
}

#endif