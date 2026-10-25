#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

class Point 
{
    private:
		unsigned int id;
        //check README file on paragraph "Design choices and Explanation Implementation in exersise" the
        //number 1) on why I use vector<double> and no vector<uint8_t> for the coordinates
		vector<double> coordinates;
		unsigned int dimension;
        string f_string;
        int first_centroid = -1;
        int second_centroid = -1;

        vector<unsigned int> gi;
    public:
        void set_id (unsigned int i);
        void set_dimension (unsigned int i);
        void set_coordinates (vector<double> image);    
        unsigned int get_dimension ();
        unsigned int get_id ();
        double get_i_coordinate (int i);
        void renew_f_string(string temp);
        void point_print();
        string get_Point_f_string();
        void print_point();
        Point get_Point();
        vector<double>  get_coordinates (); 
        void set_first_centeroid(int i);
        void set_second_centeroid(int i);
        int get_first_centeroid();
        int get_second_centeroid();
        void set_i_coordinate(int i, double value);
        void set_g(vector<unsigned int> g);
        unsigned int g(int i);
};
