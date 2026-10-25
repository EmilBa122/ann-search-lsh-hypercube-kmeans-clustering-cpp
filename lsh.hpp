#pragma once

#include <iostream>
#include <ctime>
#include <algorithm>
#include <bits/stdc++.h>

#include "common_functions.hpp"
#include "typical_functions.hpp"
#include "min_Point.hpp"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////
//							Before you do anything for the lsh part read and follow 
//		 the instructions I give on the README file on paragraph "Design choices and Explanation 
//		 Implementation in exersise" the number 6) to understand more easily the part of the lsh code
//////////////////////////////////////////////////////////////////////////////////////////////////////

void searching_part_lsh(string query_binary_file, vector<Point> &dataset, vector<Point>** lsh, int k,
		vector<V_random_vector> &v, vector<double> &t, int w, int L, int N, int M, int R, string output_text_file);

