#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <arpa/inet.h>
#include <vector>

#include "Point.hpp"

using namespace std;

void read_command (int argc, char **argv, int *k, int *M, int *probes, int *N, int *R,
		string *input, string *query, string *output);
vector<Point> read_binary_file (string binary_file, int sum);
unsigned int check_Points_Dimension (vector<Point> *dataset);
void read_command_lsh (int argc, char **argv, int *L, int *k, int *N, int *R,
		string *input, string *query, string *output);
