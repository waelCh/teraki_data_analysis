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
#define OUTPUT_FILE_A           "../../output/distances_decomp_A.txt"
#define OUTPUT_FILE_B           "../../output/distances_decomp_B.txt"
#define OUTPUT_FILE_C           "../../output/distances_decomp_C.txt"
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

int find_point_in_decomp_file_array(Point &p, string (&file_names)[3], Point (&closest_p)[3], float (&distance)[3])
{
    float min_distance_0 = MAX_DISTANCE_VALUE;
    float min_distance_1 = MAX_DISTANCE_VALUE;
    float min_distance_2 = MAX_DISTANCE_VALUE;
    float x_distance_0 = 0;
    float x_distance_1 = 0;
    float x_distance_2 = 0;
    ifstream decomp_file_0(file_names[0]);
    ifstream decomp_file_1(file_names[1]);
    ifstream decomp_file_2(file_names[2]);
    string line_file_0;
    string line_file_1;
    string line_file_2;
    vector<string> splitted_line_0;
    vector<string> splitted_line_1;
    vector<string> splitted_line_2;
    if ((decomp_file_0.is_open()) 
        && (decomp_file_1.is_open()) 
        && (decomp_file_2.is_open()))
    {
        skip_header(decomp_file_0);
        skip_header(decomp_file_1);
        skip_header(decomp_file_2);
        while((getline(decomp_file_0, line_file_0)) 
            && (getline(decomp_file_1, line_file_1)) 
            && (getline(decomp_file_2, line_file_2)))
        {
            split_line(line_file_0, splitted_line_0);
            split_line(line_file_1, splitted_line_1);
            split_line(line_file_2, splitted_line_2);
            Point px0 = create_point(splitted_line_0);
            Point px1 = create_point(splitted_line_1);
            Point px2 = create_point(splitted_line_2);
            x_distance_0 = get_distance(p, px0);
            x_distance_1 = get_distance(p, px1);
            x_distance_2 = get_distance(p, px2);
            if (x_distance_0 < min_distance_0)
            {
                min_distance_0 = x_distance_0;
                closest_p[0].Clone(px0);
            }
            if (x_distance_1 < min_distance_1)
            {
                min_distance_1 = x_distance_1;
                closest_p[1].Clone(px1);
            }
            if (x_distance_2 < min_distance_2)
            {
                min_distance_2 = x_distance_2;
                closest_p[2].Clone(px2);
            }
        }
        distance[0] = min_distance_0;
        distance[1] = min_distance_1;
        distance[2] = min_distance_2;
    }
    decomp_file_0.close();
    decomp_file_1.close();
    decomp_file_2.close();
    // return 0 on success
    return 0;
}

int main() 
{
    string line;
    Point p_error(-1, -1, -1);
    ifstream org_file(UNCOMP_FILE);
    ofstream out_file_A(OUTPUT_FILE_A);
    ofstream out_file_B(OUTPUT_FILE_B);
    ofstream out_file_C(OUTPUT_FILE_C);
    if (org_file.is_open() && out_file_A.is_open() && out_file_B.is_open() && out_file_C.is_open()) 
    {
        skip_header(org_file);
        vector<string> splitted_line;
        while(getline(org_file, line))
        {
            split_line(line, splitted_line);
            Point org_p = create_point(splitted_line);
            if (!org_p.Is_equal(p_error))
            {
                float distances[3];
                Point closest_points[3];
                string decomp_files[3] = {DECOMP_FILE_A, DECOMP_FILE_B, DECOMP_FILE_C};
                find_point_in_decomp_file_array(org_p, decomp_files, closest_points, distances);
                //File A
                out_file_A <<org_p.GetX() <<" " <<org_p.GetY() <<" " <<org_p.GetZ() \
                    <<" " <<closest_points[0].GetZ() <<" " <<closest_points[0].GetY() <<" " <<closest_points[0].GetX() \
                    <<" " <<distances[0] <<"\n";
                // File B
                out_file_B <<org_p.GetX() <<" " <<org_p.GetY() <<" " <<org_p.GetZ() \
                    <<" " <<closest_points[1].GetZ() <<" " <<closest_points[1].GetY() <<" " <<closest_points[1].GetX() \
                    <<" " <<distances[1] <<"\n";
                // File C
                out_file_C <<org_p.GetX() <<" " <<org_p.GetY() <<" " <<org_p.GetZ() \
                    <<" " <<closest_points[2].GetZ() <<" " <<closest_points[2].GetY() <<" " <<closest_points[2].GetX() \
                    <<" " <<distances[2] <<"\n";
            }
        }
        out_file_A.close();
        out_file_B.close();
        out_file_C.close();
        org_file.close();
    }
    else cout << "Unable to open files";
    return 0;
}