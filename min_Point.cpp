#include "min_Point.hpp"

void min_point::set_min_point_min_id (int i)
{
	min_id = i;
}

int min_point::get_min_point_min_id ()
{
	return min_id;
}

void min_point::set_min_point_min_distance (double temp)
{
	min_distance = temp;
}

double min_point::get_min_point_min_distance ()
{
	return min_distance;
}

bool comparator(min_point& s1, min_point& s2) {
    return ((s1.get_min_point_min_distance()) > (s2.get_min_point_min_distance()));
}
