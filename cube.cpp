#include "cube.hpp"

float search_cube(string type_of_search, Point &p, vector<Point> *hyper_cube, vector<Point> &dataset,
			int probes, int N, int M, int num_of_buckets, int R, ofstream* file)
{
	clock_t time_req;
    int bucket;
	
	time_req = clock();
	vector<int> neighbours;
	if ((type_of_search == "ANN") || (type_of_search == "Range"))
	{
		string str_bin_bucket = p.get_Point_f_string();
		bucket = stoi(str_bin_bucket, 0, 2);

		neighbours.push_back(bucket);
		int remain_probes = probes - 1;
		int dist;
		int flag = 1;
		int current_distance = 1;
		//process which we calculate and store sorted the hamming distance from the value of the
		//variable bucket compared with the values of zeros and ones that represent every bucket we have
		while (flag == 1)
		{
			for (int g = 0; g < num_of_buckets ; g++)
			{
				dist = hamming_distance(g, bucket);
				if (remain_probes > 0)
				{
					if (dist == current_distance)
					{
						neighbours.push_back(g);
						remain_probes--;
					}
				}
				else
					flag = 0;
//				cout << "hamming_distance = " << i << " - " <<  bucket_x << "=" << dist << endl;
			}
			current_distance++;
		}
	}

	vector<min_point> find_candidates;
	min_point temp_min_point;
	temp_min_point.set_min_point_min_id(-1);
	temp_min_point.set_min_point_min_distance(1000000);
	double temp;

	if ((type_of_search == "ANN") || (type_of_search == "Range"))
	{
		int position = 0;
		int sum_M = 0;
		while (position < probes && sum_M < M )
		{
			unsigned int j = 0;
			int current_index = neighbours.at(position);
			while ((j < hyper_cube[current_index].size()) && (sum_M < M))
			{
				temp = euclidean_distance(p, hyper_cube[current_index].at(j));
				if (((type_of_search == "ANN") && (temp <= temp_min_point.get_min_point_min_distance()))
					|| ((type_of_search == "Range") && (temp < R)) )
				{
					temp_min_point.set_min_point_min_id(hyper_cube[current_index].at(j).get_id());
					temp_min_point.set_min_point_min_distance(temp);
					find_candidates.push_back(temp_min_point);
				}
				j++;
				sum_M++;
			}
			position++;
		}
	}
	else if (type_of_search == "Exhausted")
	{
		for (unsigned int g = 0; g < dataset.size(); g++)
		{
			temp = euclidean_distance(p, dataset.at(g));
			if (temp <= temp_min_point.get_min_point_min_distance())
			{
				temp_min_point.set_min_point_min_id(dataset.at(g).get_id());
				temp_min_point.set_min_point_min_distance(temp);
				find_candidates.push_back(temp_min_point);
			}
		}
	}
	time_req = clock() - time_req;
//	cout << "Using " << type_of_search << " search, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds\n" << endl;

//	cout << "The nearest points from q = " <<endl; p.print_point();
	int y = 0;
	int yy = find_candidates.size() - 1;
	if (type_of_search == "Range")
	{
		sort(find_candidates.begin(), find_candidates.end(), &comparator);
	}
	while ((y < N) && (yy >= 0))
	{
		if (type_of_search == "ANN") 
		{
			//cout << "Nearest neighbor-" << to_string(y) << ": " << find_candidates.at(yy).get_min_point_min_id() << endl;
			*file << "Nearest neighbor-" << to_string(y) << ": " << find_candidates.at(yy).get_min_point_min_id() << endl;

			//cout << "distanceHypercube: " << find_candidates.at(yy).get_min_point_min_distance() << endl;
			*file << "distanceHypercube: " << find_candidates.at(yy).get_min_point_min_distance() << endl;
		}
		if (type_of_search == "Exhausted") 
		{
			//cout << "distanceTrue: " << find_candidates.at(yy).get_min_point_min_distance() << endl;
			*file << "distanceTrue: " << find_candidates.at(yy).get_min_point_min_distance() << endl;
			break;
		}
		if (type_of_search == "Range") 
		{
			//cout << find_candidates.at(yy).get_min_point_min_id() << endl;
			*file << find_candidates.at(yy).get_min_point_min_id() << endl;
		}
//		cout << "----------------------------------------------------------\n" << endl;
//		cout << "min point id " << find_candidates.at(yy).get_min_point_min_id() << endl;
//		cout << "----------------------------------------------------------\n" << endl;
//		cout << "min distance " << find_candidates.at(yy).get_min_point_min_distance() << endl;
//		cout << "----------------------------------------------------------\n" << endl;
		y++;
		yy--;
	}
	return (float)time_req/CLOCKS_PER_SEC;
}

void searching_part_cube(string query_binary_file, vector<Point> &dataset, vector<Point> *hyper_cube, int k, vector<V_random_vector> &v, vector<double> &t,
                    int w, map <int, string> &f, int probes, int num_of_buckets, int N, int M, int R, string output_text_file)                   
{
//	int number;
//	cout << "Give number of points for the query" << endl;
//	cin >> number;
	cout << "Reading the files..." << endl;

	vector<Point> query;
	
//	query = read_binary_file(query_binary_file, number);
	query = read_binary_file(query_binary_file, 20);

	vector<int> h_p_i_for_query[k];

	double move = 0.0;
	for (int g = 0; g < k; g++)
	{
		for (unsigned int i = 0; i < query.size(); i++)
		{
			int hpi = h_p(&query.at(i), v.at(g), t.at(g), w);
			if (move > hpi) 
			{
				move = hpi;
			}
			h_p_i_for_query[g].push_back(hpi);
		}
	}

	for (int g = 0; g < k; g++)
	{
		for (unsigned int i = 0; i < query.size(); i++)
		{
			h_p_i_for_query[g].at(i) -= move + 1;
			f_i(&query.at(i), h_p_i_for_query[g].at(i), &f);
		}
	}

	ofstream file(output_text_file);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	for (unsigned int i = 0; i < query.size(); i++)
    {
		//cout << "Query: " << to_string(i) << endl;
		file << "Query: " << to_string(i) << endl;

//		cout << "ANN search\n" << endl;
		float tHypercube = search_cube("ANN", query.at(i), hyper_cube, dataset, probes, N, M, num_of_buckets, 0, &file);
		
		////////////////////////////////////////////////////////////////////////////////////////////////

//		cout << "Exhausted search\n" << endl;
		float tTrue = search_cube("Exhausted", query.at(i), NULL, dataset, 0, N, 0, 0, 0, &file);

		//cout << "tHypercube: " << tHypercube << endl;
		file << "tHypercube: " << tHypercube << endl;

		//cout << "tTrue: " << tTrue << endl;
		file << "tTrue: " << tTrue << endl;

		///////////////////////////////////////////////////////////////////////////////////////////////

		//cout << "R-near neighbors:" << endl;
		float tRange = search_cube("Range", query.at(i), hyper_cube, dataset, probes, N, M, num_of_buckets, R, &file);

		//cout << "tRange: " << tRange << endl;
		file << "tRange: " << tRange << endl;

		//cout << endl;
		file << endl;
	}
}

