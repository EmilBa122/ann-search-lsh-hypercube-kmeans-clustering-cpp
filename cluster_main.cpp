#include "cluster_functions.hpp"


int main (int argc, char* argv[])
{
    string input_binary_file = "";
    string configuration_file = "";
    string output_file = "";
    string method = ""; 
    read_command_cluster (argc, argv, &input_binary_file, &configuration_file, &output_file, &method);
    //input_binary_file = "train-images.idx3-ubyte";
    //configuration_file = "cluster.conf";
    if (input_binary_file == "") 
	{	
		cout << "Please give the complete path of input.dat binary file" << endl;
		cin >> input_binary_file;
	}
	if (configuration_file == "")
	{
		cout << "Please give the complete path of cluster.conf file" << endl;
		cin >> configuration_file;
	}
    if (output_file == "") 
	{
		output_file = "output_Loyds.txt";
	}

    int sum_clusters;
    int L = 3;
    int sum_hash_functions = 4;
    int M_hypercube = 10;
    int dim_hypercube = 3;
    int probes = 2;

    read_cluster_conf(configuration_file, &sum_clusters, &L, &sum_hash_functions, &M_hypercube, &dim_hypercube, &probes);
    cout << sum_clusters << "\n" << L << "\n" << sum_hash_functions << "\n" << M_hypercube << "\n" ; 
    cout << dim_hypercube << "\n" << probes << endl; 

    cout << "Reading the files..." << endl;

    vector<Point> dataset = read_binary_file(input_binary_file, 0);

    //check for negative coordinates
    // for (auto point : dataset)
    // {
    // 	for (auto coordinate : point.get_coordinates())
    // 	{
    // 		if (coordinate < 0)
    // 		{
    // 			cout << "neg: " << coordinate << endl;
    // 		}
    // 	}
    // }

    vector<centroid> centroids;
    //cout << "--------------------" << endl;
    // for (int i = 0; i < sum_clusters; i++)
    // {
    //     cluster temp;
    //     centroid.push_back(temp);
    // }

    clock_t time_req;

    time_req = clock();
    kMeanspp_centroids_initialization(centroids, dataset, sum_clusters);

    //check for negatice mean_points
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

    // cout << "--------------------" << endl;
    // cout << "=================================" << endl;
    MacQueen_update(centroids, dataset);

    time_req = clock() - time_req;

    float time = (float)time_req/CLOCKS_PER_SEC;

    exit_file(centroids, "Lloyds", time, output_file);

    return 0;
}
