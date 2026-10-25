#include "cube.hpp"

int main (int argc, char* argv[])
{
	string input_binary_file = "";
	string query_binary_file = "";
	string output_text_file = "";
	int k , M, probes, N, R;
	read_command (argc, argv, &k, &M, &probes, &N, &R,
			&input_binary_file, &query_binary_file, &output_text_file);
	k = 16;
	N = 3;

	
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
		output_text_file = "output_k" + to_string(k) + "_probes" + to_string(probes) + "_M" + to_string(M) + "_R"
				+ to_string(R) + "_N" + to_string(N) + ".txt";
	}

	input_binary_file = "train-images.idx3-ubyte";
	//cout << input_binary_file << endl;

	cout << "Reading the files..." << endl;

	vector<Point> dataset = read_binary_file(input_binary_file, 1000);

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
	//check README file on paragraph "Design choices and Explanation Implementation in exersise" 
	//the number 5) on what the h_p_i[k] and f represent in the exersise
	vector<int> h_p_i[k];
	map <int, string> f;

	srand((unsigned) time(NULL));

	//On what I do in this section of code read the same number (i.e. 5)) paragraph as above
	//to understand the below process

	//move is explained in the same paragraph
	double move = 0.0;
	//first calculate hps
	for (int g = 0; g < k; g++)
	{
		for (unsigned int i = 0; i < dataset.size(); i++)
		{
			int hpi = h_p(&dataset.at(i), v.at(g), t.at(g), w);
			if (move > hpi) 
			{
				move = hpi;
			}
			h_p_i[g].push_back(hpi);
			//cout << h_p_i[g].at(i) << endl;
		}
	}

	//then normalize hps and calculate f
	for (int g = 0; g < k; g++)
	{
		for (unsigned int i = 0; i < dataset.size(); i++)
		{
			h_p_i[g].at(i) -= (move - 1);
//			cout << h_p_i[g].at(i) << endl;
			f_i(&dataset.at(i), h_p_i[g].at(i), &f);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	int num_of_buckets = pow(2,k);

	vector<Point> hyper_cube[num_of_buckets];
	int bucket;

	for (unsigned int i = 0; i < dataset.size(); i++)
	{
		string str_bin_bucket = dataset.at(i).get_Point_f_string();
		bucket = stoi(str_bin_bucket, 0, 2);
		//cout << bucket << endl;

		hyper_cube[bucket].push_back(dataset.at(i));
	}
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	query_binary_file = "t10k-images.idx3-ubyte";

	//////////////////////////////////////////////////////////////////////////
	searching_part_cube(query_binary_file, dataset, hyper_cube, k, v, t,
                    w, f, probes, num_of_buckets, N, M, R, output_text_file);
	
	return 0;
}
