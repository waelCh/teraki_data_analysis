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
#define OUTPUT_FILE             "./output.txt"
#define MAX_DISTANCE_VALUE      999999
#define DEBUG_MODE              false

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
    if (vector.size() == 4)
    {
        p.SetX(stof(vector[0]));
        p.SetY(stof(vector[1]));
        p.SetZ(stof(vector[2]));
    }
    return p;
}

float get_distance(Point &p1, Point &p2)
{  
    float distance = sqrt(pow(p1.GetX()-p2.GetX(), 2) +
                        pow(p1.GetY()-p2.GetY(), 2) +
                        pow(p1.GetZ()-p2.GetZ(), 2));
    return distance;
}

int find_point_in_decomp_file(Point &p, string file_name, Point &closest_p, float &distance)
{
    string line;
    float min_distance = MAX_DISTANCE_VALUE;
    float x_distance = 0;
    ifstream file(file_name);
    vector<string> splitted_line;
    if (file.is_open())
    {
        skip_header(file);
        while(getline(file, line))
        {
            split_line(line, splitted_line);
            Point px = create_point(splitted_line);
            x_distance = get_distance(p, px);
            if (x_distance < min_distance)
            {
                min_distance = x_distance;
                closest_p.Clone(px);
            }
        }
        distance = min_distance;
    }
    file.close();
    // return 0 on success
    return 0;
}

int main() 
{
    string line;
    Point p_error(-1, -1, -1);
    ifstream org_file(UNCOMP_FILE);
    ofstream out_file(OUTPUT_FILE);
    if (org_file.is_open() && out_file.is_open()) 
    {
        skip_header(org_file);
        vector<string> splitted_line;
        while(getline(org_file, line))
        {
            split_line(line, splitted_line);
            Point org_p = create_point(splitted_line);
            if (!org_p.Is_equal(p_error))
            {
                float distance;
                Point closest_point;
                find_point_in_decomp_file(org_p, DECOMP_FILE_B, closest_point, distance);
                out_file <<org_p.GetX() <<" " <<org_p.GetY() <<" " <<org_p.GetZ() \
                    <<" " <<closest_point.GetZ() <<" " <<closest_point.GetY() <<" " <<closest_point.GetX() \
                    <<" " <<distance <<"\n";
            }
        }
        out_file.close();
        org_file.close();
    }
    else cout << "Unable to open files";
     return 0;
}