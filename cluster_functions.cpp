#include "cluster_functions.hpp"

void read_command_cluster (int argc,char **argv, string *input_binary_file, string *configuration_file, string *output_file, string *method)
{
    int i;
	if (argc > 11)
	{
		cout << "Wrong arguments! Flags: -i, -c, -o, -m\n" << endl;
		exit(0);
	}
	for (i = 1; i < argc; i = i + 2)
	{
		if (strcmp(argv[i], "-i") == 0)
		{
			*input_binary_file = argv[i+1];
		}
		else if (strcmp(argv[i], "-c") == 0)
		{
			*configuration_file = argv[i+1];
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			*output_file = argv[i+1];
		}
		else if (strcmp(argv[i], "-m") == 0)
		{
			*method = argv[i+1];
		}
		else 
		{
			cout << "Wrong arguments! Flags: -i, -c, -o, -m\n" << endl;
			exit(0);
		}
	}
}

void read_cluster_conf(string configuration_file, int *sum_clusters, int *L, int *sum_hash_functions,
                int *M_hypercube, int *dim_hypercube, int *probes)
{
    ifstream myfile(configuration_file);

    if (!myfile)
	{
		cout << "Cannot open file!" << endl;
		exit(0);
	}

    string line;

    while (getline(myfile, line))
    {
        char* buff = new char[line.length() + 1];
        strcpy(buff, line.c_str());

        char* variable = strtok(buff, " ");
        char* value = strtok(NULL, " ");

        if (strcmp(variable, "number_of_clusters:") == 0)
		{
            *sum_clusters = atoi(value);
		}	
        else if (strcmp(variable, "number_of_vector_hash_tables:") == 0)
		{
            *L = atoi(value);
		}	
        else if (strcmp(variable, "number_of_vector_hash_functions:") == 0)
		{
            *sum_hash_functions = atoi(value);
		}	
        else if (strcmp(variable, "max_number_M_hypercube:") == 0)
		{
            *M_hypercube = atoi(value);
		}		
        else if (strcmp(variable, "number_of_hypercube_dimensions:") == 0)
		{
            *dim_hypercube = atoi(value);
		}		
        else if (strcmp(variable, "number_of_probes:") == 0)
		{
            *probes = atoi(value);
		}

        delete[] buff;
    }
	myfile.close();
}

void centroid::set_centroid (Point temp)
{
	point = temp;
	mean_point = temp;
}

Point centroid::get_centroid ()
{
	return point.get_Point();
}

unsigned int centroid::get_centroid_point_id ()
{
	return point.get_id();
}

//On what the below function does check README file on paragraph "Design choices and Explanation Implementation in exersise" 
//the number 7) I explain there
void my_method_centroids_initialization(vector<centroid> &centroids, vector<Point> &dataset, int k)
{
	srand((unsigned) time(NULL));
	int i = 0;
	//For every k centroid we do the below
	while (1)
	{
		//we take a random point from the dataset
		int random = rand() % dataset.size();
		double distance;
		unsigned int sum_far_centroids = 0;
		int j = 0;
		//We make a point centroid if it is far away (not necessarly the farthest point)
		//from the rest centroids
		while (1)
		{
			//.i.e not to check the cendroid with himself
			if(j == i) 
			{
				j++;
				if (j == k)
				{
					j = 0;
					break;
				}
				continue;
			}
			//if we dont have j centroids
			if (centroids.size() <= (unsigned int)j)
			{
				break;
			} 
			distance = euclidean_distance(dataset.at(random), centroids.at(j).get_centroid());
			cout << distance << endl;
			if (distance >= min_distance)// min_distance defined in cluster_functions.hpp
			{
				sum_far_centroids++;
			}
			j++;
			if (j == k)
			{
				j = 0;
				break;
			}	
		}
		//i.e. if the random point we chose is far away from the rest centroids we make it centroid
		//else choose another ranmdom point 
		if (sum_far_centroids == (centroids.size() - 1))
		{
			centroid temp;
			temp.set_centroid(dataset.at(random));
			centroids.push_back(temp);
			i++;
		}
		//if we have k cendroids we exit from the loop
		if (i == k)
			break;
	}
}

void kMeanspp_centroids_initialization(vector<centroid> &centroids, vector<Point> &dataset, int k)
{
	srand((unsigned) time(NULL));
	int random = rand() % dataset.size();

	centroid temp;
	temp.set_centroid(dataset.at(random));

	centroids.push_back(temp);

	int size = dataset.size();
	for (int t = 1; t < k; t++)
	{
		//cout << "eimai mesa" << endl;
		double max_Di = -1;
		double D[size - t];
		int g = 0;
		for (int i = 0; i < size; i++)
		{
			double min = 1000000;
			for (int ii = 0; ii < t; ii++)
			{
				if (centroids.at(ii).get_centroid_point_id() == dataset.at(i).get_id())
				{
					min = 0;
					break;
				}
				double distance = euclidean_distance(centroids.at(ii).get_centroid(), dataset.at(i));
				if (min > distance)
				{
					min = distance;
				}
			}
			if (min == 0)
			{
				g++;
				continue;
			}
			if (max_Di < min)
			{
				max_Di = min;
			}
			D[g] = min;
			g++;
		}
		double P[size - t + 1];
		P[0] = 0;
		double result = 0;
		int i = 0;
		for (int r = 1; r <= (size - t); r++)
		{
			for (int ii = 0; ii < t; ii++)
			{
				if (centroids.at(ii).get_centroid_point_id() == dataset.at(i).get_id())
				{
					i++;
					ii = 0;
				}
			}
			double new_Di = D[i] / max_Di;
			result = result + pow(new_Di, 2);
			i++;
			P[r] = result;
		}
		default_random_engine gen;
    	uniform_real_distribution<double> distribution(0.0, P[size -t]);

		float x = distribution(gen);

		int left = 0;
 		int right = size - t;
		while (1)
		{
			int mid = (left + right) / 2;
			if (mid == 0)
			{
				centroid temp;
				temp.set_centroid(dataset.at(mid));

				centroids.push_back(temp);
				break;
			}
			if ((P[mid - 1] < x) && (P[mid] >= x))  
			{
				centroid temp;
				temp.set_centroid(dataset.at(mid));

				centroids.push_back(temp);
				break;
			}
			if ( x <= P[mid -1])
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
			if (left > right)
			{
				cout << "fatal error" << endl;
				exit(0);
			}
		}
	}
	for (int t = 0; t < k; t++)
	{
		Point temp;
		temp.set_dimension(dataset.at(0).get_dimension());
		vector<double> zero;
		for (unsigned int i = 0; i < dataset.at(0).get_dimension(); i++)
		{
			zero.push_back(0);
		}
		temp.set_coordinates(zero);
		centroids.at(t).set_mean_point(temp);
	}
}

void centroid::add_point_to_centroid(Point temp)
{
	P.insert(pair<unsigned int, Point>(temp.get_id(), temp));
}

map<unsigned int, Point>& centroid::get_centroid_points()
{
	return P;
}

unsigned int centroid::get_centroid_dimension()
{
	return point.get_dimension();
}

void centroid::set_centroid_coordinates(Point mean_point)
{
	point.set_coordinates(mean_point.get_coordinates());
	point.set_dimension(mean_point.get_dimension());
}

void centroid::set_mean_point(Point temp)
{
	mean_point = temp;
}

Point centroid::get_mean_point()
{
	return mean_point;
}

int assigment_Lloyd_method (vector<centroid> &centroids, Point &p, int k)
{
		double min1 = 10000000;
		double min2 = 100000000;
		int new_position1;
		int new_position2;
		//we find the new nearest centroid of the point p
		for (int t = 0; t < k; t++)
		{
			double distance = euclidean_distance(p, centroids.at(t).get_centroid());
			if (min1 > distance)
			{
				min2 = min1;
				new_position2 = new_position1;
				min1 = distance;
				new_position1 = t;
			}
			if (min1 < distance && min2 > distance)
			{
				min2 = distance;
				new_position2 = t;
			}
		}
		//We erase the point p from the old centroid and we add it to the new one
		if (p.get_first_centeroid() != -1)
		{
//			cout << "centroid " << p.get_first_centeroid() << " id: " << p.get_id() << endl;
//			cout << "before: " << centroids.at(p.get_first_centeroid()).get_centroid_points().size() << endl;

//			map<unsigned int, Point>::iterator it;
//			it = centroids.at(p.get_first_centeroid()).get_centroid_points().find(p.get_id());
//			if (it == centroids.at(p.get_first_centeroid()).get_centroid_points().end()) {
//				cout << " error " << endl;
//				exit(0);
//			}

			centroids.at(p.get_first_centeroid()).get_centroid_points().erase(p.get_id());
//			cout << "after: " << centroids.at(p.get_first_centeroid()).get_centroid_points().size() << endl;
			int dimension = p.get_dimension();
			Point mean_point = centroids.at(p.get_first_centeroid()).get_mean_point();
			int number = centroids.at(p.get_first_centeroid()).get_centroid_points().size();
			for (int i = 0; i < dimension; i++)
			{
				mean_point.set_i_coordinate(i, (ceil(mean_point.get_i_coordinate(i) *
						(number+1)) - p.get_i_coordinate(i)) / number);
			}
			centroids.at(p.get_first_centeroid()).set_centroid(mean_point);
		}
		p.set_first_centeroid(new_position1);
		p.set_second_centeroid(new_position2);
		centroids.at(new_position1).add_point_to_centroid(p);
		return new_position1;
}

void MacQueen_update(vector<centroid> &centroids, vector<Point> &dataset)
{
	int k = centroids.size();
	int times = 4;
	vector<Point> temp_cendroids;
	int sum = 0;
	do
	{
		sum = 0;
		for (int f = 0; f < k; f++)
		{
			temp_cendroids.clear();
		}
		
		for (int f = 0; f < k; f++)
		{
			temp_cendroids.push_back(centroids.at(f).get_centroid());
		}
	
		for (unsigned int iii = 0; iii < dataset.size(); iii++)
		{
			if (iii % 1000 == 0)
			{
				cout << "[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]] " << dataset.at(iii).get_id() << endl;
			}
			////////////////////////////////////////////////////////////////////////////////
			int new_position1 = assigment_Lloyd_method(centroids, dataset.at(iii), k);
			/////////////////////////////////////////////////////////////////////////////////
			
			int dimension = dataset.at(0).get_dimension();
			Point mean_point = centroids.at(new_position1).get_mean_point(); 
			int number = centroids.at(new_position1).get_centroid_points().size();
			for (int i = 0; i < dimension; i++)
			{
				mean_point.set_i_coordinate(i, (mean_point.get_i_coordinate(i) * 
						(number -1) + dataset.at(iii).get_i_coordinate(i)) / number);
			}
			centroids.at(new_position1).set_centroid(mean_point);
			
		}
		double mysum = 0.0;
		//cout << "edw1" << endl;
		for (int i = 0; i < k; i++)
		{
			//cout << i <<"dddddd " << centroids.at(i).get_centroid_points().size() << endl;
			//centroids.at(i).get_mean_point().print_point();
			//cout << "--------------------" << endl;
			mysum += centroids.at(i).get_centroid_points().size();
			
		 	if (euclidean_distance(temp_cendroids.at(i), centroids.at(i).get_centroid())
		 												<= little_change_of_centroid)
		 	{
		 		sum++;
		 	}
		 }

		// //check for negatice mean_points
		// for (auto centroid : centroids)
		// {
		// 	for (auto coordinate : centroid.get_mean_point().get_coordinates())
		// 	{
		// 		if (coordinate < 0)
		// 		{
		// 			cout << "neg: " << coordinate << endl;
		// 		}
		// 	}
		// }

		//cout << "sum: " << mysum << endl;
		times--;
	} while ((sum < (3.0/4)*k) && (times > 0));
}

void exit_file(vector<centroid> &centroids, string method, float time, string output_file)
{
	ofstream file(output_file);

	if (output_file == "Classic")
	{
		file << "Algorithm: Lloyds" << endl;
	}	
	for (unsigned int i = 0; i < centroids.size(); i++)
	{
		file << "Cluster-" << to_string(i + 1) << " ";
		file <<"{size: " << centroids.at(i).get_centroid_points().size() << ", centroid: [";
		int size = centroids.at(i).get_centroid().get_dimension();
		for (int j = 0; j < size; j++)
		{
			file << " " << floor(centroids.at(i).get_centroid().get_i_coordinate(j));
			if (j != (size -1))
			{
				file << ",";
			}
		}
		file << "]" << endl;
		
	}
	file << "clustering_time: " << time << endl;
	file.close();
}