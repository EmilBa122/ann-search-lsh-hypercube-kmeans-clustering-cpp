#include "common_functions.hpp"

//Code for random generator from
//https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
int random_generator()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist_int(2, 6);

	int w = dist_int(gen);
	return w;
}

double t_random_generator(int w)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> dist(0, w);

	double t = dist(gen);
	return t;
}

int flip_a_coin()
{
	int random = rand() % 2;
//	cout << random << endl;
	return random;
}

//Code for random number from normal distribution
//https://stackoverflow.com/questions/60721093/random-number-from-normal-distribution-in-c
double get_Normal_Distribution()
{
	static unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	static default_random_engine generator(seed);
	
	static normal_distribution<double> normalDist(0, 1);
	
	return normalDist(generator);
}

//Returns dot product of two vectors normalized by w
double dot_product_w (Point p, V_random_vector v, int w)
{
	double sum = 0;                             
	for (unsigned int i = 0; i < p.get_dimension(); i++)
	{
		sum = sum + (unsigned int) p.get_i_coordinate(i) *  v.get_i_coordinate(i) / w;
	}
//	cout << sum << endl;
	return sum;
}

//Returns dot product of two vectors
double dot_product (Point p, V_random_vector v)
{
	double sum = 0;                             
	for (unsigned int i = 0; i < p.get_dimension(); i++)
	{
		sum = sum + (unsigned int) p.get_i_coordinate(i) *  v.get_i_coordinate(i);
	}
//	cout << sum << endl;
	return sum;
}

int h_p(Point *p, V_random_vector v, double t, int w)
{
	//Because the dot product it is very large, I do "/w" below 
	int h_p = floor((dot_product(*p, v) + t ) / w ); 
	return h_p;
}

//check README file on paragraph "Design choices and Explanation Implementation in exersise" 
//the number 4) on how works the below function
void f_i(Point *p, int h_p, map <int, string>* f)
{
	map<int, string>::iterator it;
	it = f->find(h_p);
	if (it == f->end())
	{
		string temp = to_string(flip_a_coin());
		f->insert(pair<int, string>(h_p, temp));
		p->renew_f_string(temp);
	}
	else
	{
		p->renew_f_string(it->second);
	}
}

//Original source but I modified some things
//https://dev.to/ggorantala/hamming-distance-kcm
int hamming_distance(int x, int y)
{
    int x_or = x ^ y; // keep different bits, they will be represented as 1

    int dist = 0; // hamming_distance
    while(x_or > 0)
    { // check all bits
        if((x_or | 1) == x_or) // if true, means the leftmost bit is 1, hence different
            dist++;
        x_or = x_or >> 1;
    }
    return dist;
}

double euclidean_distance(Point x, Point y)
{
	double sum = 0.0;
	for (unsigned  int i = 0; i < x.get_dimension(); i++)
	{
		sum = sum + pow(x.get_i_coordinate(i) - y.get_i_coordinate(i), 2);
	}
	//cout << "sum " << sum << endl;
	return sqrt(sum);
}

void V_random_vector::set_random_vector_coordinates ()
{
	for (unsigned int i = 0; i < dimension; i++)
	{
		v.push_back(get_Normal_Distribution());
//		cout << v.at(i) << endl;
	}
}

double V_random_vector::get_i_coordinate (int i)
{
	return v.at(i);
}

V_random_vector::V_random_vector(unsigned int dimension)
{
	this->dimension = dimension;
	set_random_vector_coordinates();
}

void V_random_vector::print_v()
{
	for (unsigned int i = 0; i < dimension; i++)
	{
		cout << v.at(i) << " " ;
	}
	cout << endl;
}
