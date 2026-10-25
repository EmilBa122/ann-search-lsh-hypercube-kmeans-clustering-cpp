#include <map>
#include <cstdlib>
#include "lsh.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////
//							Before you do anything for the lsh part read and follow 
//		 the instructions I give on the README file on paragraph "Design choices and Explanation 
//		 Implementation in exersise" the number 6) to understand more easily the part of the lsh code
//////////////////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
	string input_binary_file = "";
	string query_binary_file = "";
	string output_text_file = "";
	int k = 4, L, N, R, M;
	read_command_lsh (argc, argv, &L, &k, &N, &R,
			&input_binary_file, &query_binary_file, &output_text_file);
	if (input_binary_file == "") 
	{	
		cout << "Please give the complete path of input.dat binary file" << endl;
		cin >> input_binary_file;
	}
	if (query_binary_file == "")
	{
		cout << "Please give the complete path of query.dat binary file" << endl;
		cin >> query_binary_file;
	}
	if (output_text_file == "") 
	{
		output_text_file = "output_k" + to_string(k) + "_L" + to_string(L) + "_R"
				+ to_string(R) + "_N" + to_string(N) + ".txt";
	}

	//input_binary_file = "train-images.idx3-ubyte";
	cout << input_binary_file << endl;

	cout << "Reading the files..." << endl;

	vector<Point> dataset = read_binary_file(input_binary_file, 0);

	unsigned int dimension = check_Points_Dimension(&dataset);

	int w = 6;//random_generator();

	vector<V_random_vector> v;
	vector<double> t;

	for (int i = 0; i < k; i++)
	{
		v.push_back(V_random_vector(dimension));
		t.push_back(t_random_generator(w));
		//v.at(i).print_v();
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
	vector<int> h_p_i[k];
	map <int, string> f;

	srand((unsigned) time(NULL));

	//first calculate hps
	double max = 1;
	double move = 0.0;
	for (int g = 0; g < k; g++)
	{
		for (unsigned int i = 0; i < dataset.size(); i++)
		{
			int hpi = h_p(&dataset.at(i), v.at(g), t.at(g), w);
			if (move > hpi) 
			{
				move = hpi;
			}
			if (max < hpi) 
			{
				max = hpi;
			}
			h_p_i[g].push_back(hpi);
			//cout << h_p_i[g].at(i) << endl;
		}
	}
	max = max + move;
	unsigned int max_rand = (INT32_MAX - 5) / max - 5;
//	cout << max_rand << endl;

	M = dataset.size() / 16;
	for (unsigned int i = 0; i < dataset.size(); i++)
	{
		vector<unsigned int> g;
		for (int l = 0; l < L; l++)
		{
			unsigned int sum = 0;
			for (int j = 0; j < k; j++)
			{
				h_p_i[j].at(i) -= (move - 1);
				sum = sum + ((rand() % max_rand) * h_p_i[j].at(i)) % M;
			}
			sum = sum % M;
			g.push_back(sum);
		}
		dataset.at(i).set_g(g);
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	vector<Point> **lsh;
	lsh = new vector<Point>*[L];
	for (int l = 0; l < L; l++)
	{
		lsh[l] = new vector<Point>[M];
	}
	for (int l = 0; l < L; l++)
	{
		for (unsigned int i = 0; i < dataset.size(); i++)
		{
			lsh[l][dataset.at(i).g(l)].push_back(dataset.at(i));
		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	//query_binary_file = "t10k-images.idx3-ubyte";

	//////////////////////////////////////////////////////////////////////////
	searching_part_lsh(query_binary_file, dataset, lsh, k,
			v, t, w, L, N, M, R, output_text_file);
	
//	for (int l = 0; l < L; l++)
//	{
//		delete lsh[l];
//	}
	delete [] lsh;

	return 0;
}
