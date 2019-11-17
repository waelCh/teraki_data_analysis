#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "Point.h"
using namespace std;

// The text line that seperate header metadata from LIDAR points.
#define END_HEADER_MARKER       "end_header"

// The input and output files locations.
#define UNCOMP_FILE             "../../input/Car_XYZI_uncompressed_ASCII.ply"
#define DECOMP_FILE_A           "../../input/Car_XYZI_decompressed_ASCII_A.ply"
#define DECOMP_FILE_B           "../../input/Car_XYZI_decompressed_ASCII_B.ply"
#define DECOMP_FILE_C           "../../input/Car_XYZI_decompressed_ASCII_C.ply"
#define OUTPUT_FILE_A           "../../output/distances_decomp_A.txt"
#define OUTPUT_FILE_B           "../../output/distances_decomp_B.txt"
#define OUTPUT_FILE_C           "../../output/distances_decomp_C.txt"

// The number of decompressed files existing in the input folder.
#define DECOMP_FILES_COUNT      3

#define MAX_DISTANCE_VALUE      999999
#define DEBUG_MODE              false

/**
 * Skip header lines of the input file.
 *
 * @param stream references the file we want to skip some lines.
 * @return void.
 */
void skip_header(istream &stream) 
{
    bool keep_looping = true;
    string line;
    // Keep looping and skipping lines until we reach END_HEADER_MARKER
    while (keep_looping)
    {
        getline(stream, line);
        if (line.compare(END_HEADER_MARKER) == 0)
        {
            keep_looping = false;
        }
    }    
}

/**
 * Given a space separated string, splits it into a vector
 * of sub-strings.
 *
 * @param text string to be splitted.
 * @param result vector containing the obtained sub-strings.
 * @return void.
 */
void split_line(string text, vector<string> &result)
{
    istringstream iss(text);
    result.clear();
    for(string s; iss>>s;)
        result.push_back(s);
}

/**
 * Create a Point object from a vector
 *
 * @param vector contains the coordinates of a point.
 * @return new point containing 'vector' passed coordinates, or 
 * a point with coordiantes (-1,-1,-1) 
 * if the number of 'vector' elements is not equal to 4.
 */
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

/**
 * Save the min disance and its associated point
 */
void update_min_point(float &old_dist, float new_dist, Point &old_closest_point, Point &new_point)
{
    if (new_dist < old_dist)
    {
        old_dist = new_dist;
        old_closest_point.Clone(new_point);
    }
}

/**
 * Get through the list of record files and retrieve
 * a point from each file that satisfy the least distance with point 'p'
 * 
 * @param p reference point to measure distance
 * @param file_names URI of data files
 * @param closest_p list of closest points to point 'p'
 * @param distance min recorded distances between 'p' and all records in each file
 * @return 0 on sucess. -1 if an error occured
 */
int find_point_in_decomp_file_array(
        Point &p, 
        string (&file_names)[DECOMP_FILES_COUNT], 
        Point (&closest_p)[DECOMP_FILES_COUNT], 
        float (&distance)[DECOMP_FILES_COUNT])
{
    float min_distance[DECOMP_FILES_COUNT] = {MAX_DISTANCE_VALUE, MAX_DISTANCE_VALUE, MAX_DISTANCE_VALUE};
    float x_distance[DECOMP_FILES_COUNT] = {0, 0, 0};
    string line_files[DECOMP_FILES_COUNT];
    vector<string> splitted_line[DECOMP_FILES_COUNT];
    Point px[DECOMP_FILES_COUNT];
    ifstream decomp_file_0(file_names[0]);
    ifstream decomp_file_1(file_names[1]);
    ifstream decomp_file_2(file_names[2]);
    if ((decomp_file_0.is_open()) 
        && (decomp_file_1.is_open()) 
        && (decomp_file_2.is_open()))
    {
        skip_header(decomp_file_0);
        skip_header(decomp_file_1);
        skip_header(decomp_file_2);
        // go through all the decomp files simultaneously, 
        // compare distances with 'p' and save the least record
        while((getline(decomp_file_0, line_files[0])) 
            && (getline(decomp_file_1, line_files[1])) 
            && (getline(decomp_file_2, line_files[2])))
        {
            split_line(line_files[0], splitted_line[0]);
            split_line(line_files[1], splitted_line[1]);
            split_line(line_files[2], splitted_line[2]);
            px[0] = create_point(splitted_line[0]);
            px[1] = create_point(splitted_line[1]);
            px[2] = create_point(splitted_line[2]);
            x_distance[0] = p.Distance_from(px[0]);
            x_distance[1] = p.Distance_from(px[1]);
            x_distance[2] = p.Distance_from(px[2]);
            update_min_point(min_distance[0], x_distance[0], closest_p[0], px[0]);
            update_min_point(min_distance[1], x_distance[1], closest_p[1], px[1]);
            update_min_point(min_distance[2], x_distance[2], closest_p[2], px[2]);
        }
        distance[0] = min_distance[0];
        distance[1] = min_distance[1];
        distance[2] = min_distance[2];
    }
    decomp_file_0.close();
    decomp_file_1.close();
    decomp_file_2.close();
    // return 0 on success
    return 0;
}

void log_to_dist_file(ofstream &file, Point &org_p, Point &found_p, float distance)
{
    file <<org_p.GetX() <<" " <<org_p.GetY() <<" " <<org_p.GetZ() \
            <<" " <<found_p.GetZ() <<" " <<found_p.GetY() <<" " <<found_p.GetX() \
            <<" " <<distance <<"\n";
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
                float distances[DECOMP_FILES_COUNT];
                Point closest_points[DECOMP_FILES_COUNT];
                string decomp_files[DECOMP_FILES_COUNT] = {DECOMP_FILE_A, DECOMP_FILE_B, DECOMP_FILE_C};
                // find the closest point in each decomp file
                find_point_in_decomp_file_array(org_p, decomp_files, closest_points, distances);
                // log distances to corresponding files
                log_to_dist_file(out_file_A, org_p, closest_points[0], distances[0]);
                log_to_dist_file(out_file_B, org_p, closest_points[1], distances[1]);
                log_to_dist_file(out_file_C, org_p, closest_points[2], distances[2]);
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