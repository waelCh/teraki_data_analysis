#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "Point.h"
using namespace std;

#define END_HEADER_MARKER       "end_header"
#define UNCOMP_FILE             "../../input/Car_XYZI_uncompressed_ASCII.ply"
#define DECOMP_FILE_A           "../../input/Car_XYZI_decompressed_ASCII_A.ply"
#define DECOMP_FILE_B           "../../input/Car_XYZI_decompressed_ASCII_B.ply"
#define DECOMP_FILE_C           "../../input/Car_XYZI_decompressed_ASCII_C.ply"
#define MAX_DISTANCE_VALUE      999999

void skip_header(istream &stream) 
{
    bool keep_looping = true;
    string line;
    while (keep_looping)
    {
        getline(stream, line);
        if (line.compare(END_HEADER_MARKER) == 0)
        {
            keep_looping = false;
        }
    }    
}

void split_line(string text, vector<string> &result)
{
    istringstream iss(text);
    result.clear();
    for(string s; iss>>s;)
        result.push_back(s);
}

Point create_point(vector<string> &vector)
{
    Point p(-1,-1,-1);
    if (vector.size() != 4) cout <<"Invalid vector size (!= 4)" <<"\n";
    else
    {
        p.SetX(stof(vector[0]));
        p.SetY(stof(vector[1]));
        p.SetZ(stof(vector[2]));
    }
    return p;
}

int get_distance(Point &p1, Point &p2)
{
    return sqrt(pow(p1.GetX()-p2.GetX(), 2) +
                pow(p1.GetY()-p2.GetY(), 2) +
                pow(p1.GetZ()-p2.GetZ(), 2));
}

int find_point_in_decomp_file(Point &p, string file_name, Point &closest_p, float &distance)
{
    string line;
    int min_distance = MAX_DISTANCE_VALUE;
    int x_distance = 0;
    ifstream file(UNCOMP_FILE);
    vector<string> splitted_line;
    if (file.is_open())
    {
        while(getline(file, line))
        {
            split_line(line, splitted_line);
            Point px = create_point(splitted_line);
            if (px.GetX() == -1) cout <<"ERROR at:" <<line <<"\n";
            x_distance = get_distance(p, px);
            if (x_distance < min_distance)
            {
                min_distance = x_distance;
                closest_p = px;
            }
            
        }
    }
    
    file.close();
    // return 0 on success
    return 0;
}

int main() 
{
    string line;
    ifstream org_file(UNCOMP_FILE);
    if (org_file.is_open()) 
    {
        skip_header(org_file);
        vector<string> splitted_line;
        while(getline(org_file, line))
        {
            split_line(line, splitted_line);
            Point org_p = create_point(splitted_line);
            if (org_p.GetX() == -1) cout <<"ERROR at:" <<line <<"\n";
            float distance;
            Point closest_point;
            find_point_in_decomp_file(org_p, DECOMP_FILE_B, closest_point, distance);
            cout <<distance <<"\n";
        }
        org_file.close();
    }

    else cout << "Unable to open files";
     return 0;
}