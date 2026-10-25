#include "typical_functions.hpp"

void read_command (int argc, char **argv, int *k, int *M, int *probes, int *N, int *R,
		string *input, string *query, string *output)
{
	*k = 14;
	*M = 10;
	*probes = 2;
	*N = 1;
	*R = 10000;

	int i;
	for (i = 1; i < argc; i = i + 2)
	{
		if (strcmp(argv[i], "-k") == 0)
		{
			*k = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-M") == 0)
		{
			*M = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-probes") == 0)
		{
			*probes = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-N") == 0)
		{
			*N = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-R") == 0)
		{
			*R = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			*input = argv[i+1];
		}
		else if (strcmp(argv[i], "-q") == 0)
		{
			*query = argv[i+1];
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			*output = argv[i+1];
		}
		else 
		{
			cout << "Wrong arguments! Flags: -d, -q, -k, -M, -probes, -N, -R\n" << endl;
			exit(0);
		}
	}		
}

void read_command_lsh (int argc, char **argv, int *L, int *k, int *N, int *R,
		string *input, string *query, string* output)
{
	*k = 4;
	*L = 5;
	*N = 1;
	*R = 10000;

	int i;
	for (i = 1; i < argc; i = i + 2)
	{
		if (strcmp(argv[i], "-k") == 0)
		{
			*k = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-L") == 0)
		{
			*L = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-N") == 0)
		{
			*N = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-R") == 0)
		{
			*R = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			*input = argv[i+1];
		}
		else if (strcmp(argv[i], "-q") == 0)
		{
			*query = argv[i+1];
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			*output = argv[i+1];
		}
		else 
		{
			cout << "Wrong arguments! Flags: -d, -q, -k, -M, -probes, -N, -R\n" << endl;
			exit(0);
		}
	}	
}

vector<Point> read_binary_file (string binary_file, int sum)
{
	ifstream myfile (binary_file, ios::binary);

	if (!myfile)
	{
		cout << "Cannot open file " << binary_file << endl;
		exit(0);
	}

	//Because of the high/low endian, the ntohl() function translates a long integer from 
	//network byte order to host byte order, in other words nhtol checks what we have and in  
	//low-endian machines flip the bytes 
	unsigned int magic_number;
	myfile.read((char*)(&magic_number), 4);
	cout << ntohl(magic_number) << endl;

	unsigned int images_num;
	myfile.read((char*)(&images_num), 4);
	images_num = ntohl(images_num);
	cout << images_num << endl;

	unsigned int rows;
	myfile.read((char*)(&rows), 4);
	rows = ntohl(rows);
	cout << rows << endl;

	unsigned int cols;
	myfile.read((char*)(&cols), 4);
	cols = ntohl(cols);
	cout << cols << endl;

	int sum_pointers = 0;
	vector<Point> dataset;
	//Because we read 1 byte every time we dont care about high/low endian
	for (unsigned int i = 0; i < images_num; i++)
	{
		Point p;
		vector<double> image;
		unsigned int pixels_num = rows * cols;
		for(unsigned int j = 0; j < pixels_num; j++)
		{
			uint8_t pixel;
			myfile.read((char*)&pixel, 1);
			//You have to do CAST in UNSGINED INT to print the value
//			cout << (unsigned int)pixel << endl;
			image.push_back((double)pixel);
		} 
		p.set_id (i);
		p.set_dimension (image.size());
		p.set_coordinates (image);
		dataset.push_back(p); 
//		p.print_point();
		if (sum)
		{
			sum_pointers++;
			if (sum == sum_pointers)
			{
				break;
			}
		}
	}

	myfile.close();

	return dataset;
}

unsigned int check_Points_Dimension (vector<Point> *dataset)
{
	unsigned int global_dimension = dataset->at(0).get_dimension();
	for (Point p: *(dataset))
	{
		if (p.get_dimension() != global_dimension)
		{
			cout << "Invalid dimension among given Points" << endl;
			exit(0);
		}
	}
	return global_dimension;
}
