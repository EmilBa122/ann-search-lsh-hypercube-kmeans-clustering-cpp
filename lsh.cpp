#include "lsh.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////
//							Before you do anything for the lsh part read and follow 
//		 the instructions I give on the README file on paragraph "Design choices and Explanation 
//		 Implementation in exersise" the number 6) to understand more easily the part of the lsh code
//////////////////////////////////////////////////////////////////////////////////////////////////////
float search_lsh(string type_of_search, Point &p, vector<Point>** lsh, vector<Point> &dataset,
			int N, int M, int R, int L, ofstream* file)
{
	clock_t time_req;
	time_req = clock();

	vector<min_point> find_candidates;
	min_point temp_min_point;
	temp_min_point.set_min_point_min_id(-1);
	temp_min_point.set_min_point_min_distance(1000000);
	double temp;

	if ((type_of_search == "ANN") || (type_of_search == "Range"))
	{
		for (int l = 0; l < L; l++)
		{
			unsigned int bucket = p.g(l);

			unsigned int size = lsh[l][bucket].size();
			for (unsigned int j = 0; j < size; j++)
			{
				temp = euclidean_distance(p, lsh[l][bucket].at(j));
				if (((type_of_search == "ANN") && (temp <= temp_min_point.get_min_point_min_distance()))
					|| ((type_of_search == "Range") && (temp < R)) )
				{
					temp_min_point.set_min_point_min_id(lsh[l][bucket].at(j).get_id());
					temp_min_point.set_min_point_min_distance(temp);
					find_candidates.push_back(temp_min_point);
				}
			}
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
		if (find_candidates.size() == (unsigned int)y)
		{
			cout << "No Neighbours" << endl;
			break;
		}
		if (type_of_search == "ANN") 
		{
		//	cout << "Nearest neighbor-" << to_string(y) << ": " << find_candidates.at(yy).get_min_point_min_id() << endl;
			*file << "Nearest neighbor-" << to_string(y) << ": " << find_candidates.at(yy).get_min_point_min_id() << endl;

		//	cout << "distanceLSH: " << find_candidates.at(yy).get_min_point_min_distance() << endl;
			*file << "distanceLSH: " << find_candidates.at(yy).get_min_point_min_distance() << endl;
		}
		if (type_of_search == "Exhausted") 
		{
			//cout << "distanceTrue: " << find_candidates.at(yy).get_min_point_min_distance() << endl;
			*file << "distanceTrue: " << find_candidates.at(yy).get_min_point_min_distance() << endl;
			break;
		}
		if (type_of_search == "Range") 
		{
		//	cout << find_candidates.at(yy).get_min_point_min_id() << endl;
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

void searching_part_lsh(string query_binary_file, vector<Point> &dataset, vector<Point>** lsh, int k,
		vector<V_random_vector> &v, vector<double> &t, int w, int L, int N, int M, int R, string output_text_file)
{
//	int number;
//	cout << "Give number of points for the query" << endl;
//	cin >> number;
	cout << "Reading the files..." << endl;

	vector<Point> query;
	
//	query = read_binary_file(query_binary_file, number);
	query = read_binary_file(query_binary_file, 0);

	vector<int> h_p_i_for_query[k];

	//first calculate hps
	double max = 1;
	double move = 0.0;
	for (int g = 0; g < k; g++)
	{
		for (unsigned int i = 0; i < query.size(); i++)
		{
			int hpi = h_p(&query.at(i), v.at(g), t.at(g), w);
			if (move > hpi) {
				move = hpi;
			}
			if (max < hpi) {
				max = hpi;
			}
			h_p_i_for_query[g].push_back(hpi);
			//cout << h_p_i[g].at(i) << endl;
		}
	}
	max = max + move;
	unsigned int max_rand = (INT32_MAX - 5) / max - 5;
	cout << max_rand << endl;

	for (unsigned int i = 0; i < query.size(); i++)
	{
		vector<unsigned int> g;
		for (int l = 0; l < L; l++)
		{
			unsigned int sum = 0;
			for (int j = 0; j < k; j++)
			{
				h_p_i_for_query[j].at(i) -= (move - 1);
				sum = sum + ((rand() % max_rand) * h_p_i_for_query[j].at(i)) % M;
			}
			sum = sum % M;
			g.push_back(sum);
		}
		query.at(i).set_g(g);
	}

	ofstream file(output_text_file);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	for (unsigned int i = 0; i < query.size(); i++)
    {
		//cout << "Query: " << to_string(i) << endl;
		file << "Query: " << to_string(i) << endl;

//		cout << "ANN search\n" << endl;
		float tLSH = search_lsh("ANN", query.at(i), lsh, dataset, N, M, 0, L, &file);
		
		////////////////////////////////////////////////////////////////////////////////////////////////

//		cout << "Exhausted search\n" << endl;
		float tTrue = search_lsh("Exhausted", query.at(i), NULL, dataset, N, 0, 0, L, &file);

//		cout << "tLSH: " << tLSH << endl;
		file << "tLSH: " << tLSH << endl;

//		cout << "tTrue: " << tTrue << endl;
		file << "tTrue: " << tTrue << endl;

		///////////////////////////////////////////////////////////////////////////////////////////////

//		cout << "R-near neighbors:" << endl;
		float tRange = search_lsh("Range", query.at(i), lsh, dataset, N, M, R, L, &file);

//		cout << "tRange: " << tRange << endl;
		file << "tRange: " << tRange << endl;

//		cout << endl;
		file << endl;
	}
}

