#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <ctime>

#include "typical_functions.hpp"
#include "common_functions.hpp"

#define min_distance 3000
#define little_change_of_centroid 1000

using namespace std;

//check README file on paragraph "Design choices and Explanation Implementation in exersise" 
//the number 7) on what class centroid represent and do in the exersise
class centroid
{
    private:
        //coordinates of the point centroid
        Point point;
        //has a number of points near it
        //on what P do check the same number of paragraph (i.e. 7)) I explain there 
        map<unsigned int, Point> P;
        Point mean_point;
    public:
        void set_centroid (Point p);
        Point get_centroid ();
        unsigned int get_centroid_point_id ();
        void add_point_to_centroid(Point p);
        map<unsigned int, Point> &get_centroid_points();
        unsigned int get_centroid_dimension();
        void set_centroid_coordinates(Point mean_point);
        void set_mean_point(Point temp);
        Point get_mean_point();
};

void read_command_cluster (int argc,char **argv, string *input_binary_file, string *configuration_file, string *output_file, string *method);
void read_cluster_conf(string configuration_file, int *sum_clusters, int *L, int *sum_hash_functions, int *M_hypercube, int *dim_hypercube, int *probes); 
void kMeanspp_centroids_initialization(vector<centroid> &centroids, vector<Point> &dataset, int k); 
void MacQueen_update(vector<centroid> &centroids, vector<Point> &dataset);
void exit_file(vector<centroid> &centroids, string method, float time, string output_file);
