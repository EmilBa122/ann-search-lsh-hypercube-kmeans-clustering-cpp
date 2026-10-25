#pragma once

#include <iostream>
#include <ctime>
#include <algorithm>
#include <bits/stdc++.h>
#include <map>
#include <cstdlib>

#include "common_functions.hpp"
#include "typical_functions.hpp"
#include "min_Point.hpp"

using namespace std;

void searching_part_cube(string query_binary_file, vector<Point> &dataset, vector<Point> *hyper_cube, int k, vector<V_random_vector> &v, vector<double> &t,
                    int w, map <int, string> &f, int probes, int num_of_buckets, int N, int M, int R, string output_text_file);

