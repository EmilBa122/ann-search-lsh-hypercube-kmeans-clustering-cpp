#include "Point.hpp"

void Point::set_id (unsigned int i)
{
	id = i;
}

void Point::set_dimension (unsigned int i)
{
	dimension = i;
}

void Point::set_coordinates (vector<double> image)
{
    for (unsigned int i = 0; i < image.size(); i++)
    {
        coordinates.push_back(image.at(i));
    }
}

unsigned int Point::get_dimension ()
{
    return dimension;
}

unsigned int Point::get_id ()
{
    return id;
}

double Point::get_i_coordinate (int i)
{
    return coordinates.at(i);
}

void Point::renew_f_string(string temp)
{
    f_string = f_string + temp;
}

void Point::point_print()
{
    cout << f_string << endl;
}

string Point::get_Point_f_string()
{
    return f_string;
}

void Point::print_point()
{
    for (unsigned int i = 0; i < dimension; i++)
    {
        cout << round(coordinates.at(i)) << " ";
    }
    cout << endl;
}

Point Point::get_Point()
{
    return *this;
}

vector<double>  Point::get_coordinates ()
{
    return coordinates;
} 

void Point::set_first_centeroid(int i)
{
    first_centroid = i;
}

void Point::set_second_centeroid(int i)
{
    second_centroid = i;
}

int Point::get_first_centeroid()
{
    return first_centroid;
}

int Point::get_second_centeroid()
{
    return second_centroid;
}

void Point::set_i_coordinate(int i, double value)
{
    coordinates.at(i) = value;
}

void Point::set_g(vector<unsigned int> gi)
{
	this->gi = gi;
}

unsigned int Point::g(int i)
{
	return gi.at(i);
}
