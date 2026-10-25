#pragma once

class min_point
{
    private:
        int min_id;
        double min_distance;
    public:
        void set_min_point_min_id (int i);
        int get_min_point_min_id ();
        void set_min_point_min_distance (double temp);
        double get_min_point_min_distance ();
};

bool comparator(min_point& s1, min_point& s2);
