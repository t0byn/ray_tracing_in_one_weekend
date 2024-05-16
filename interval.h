#ifndef INTERVAL_H
#define INTERVAL_H

class Interval
{
public:
    double min;
    double max;

    inline Interval() : min(+infinity), max(-infinity) {};
    inline Interval(double min, double max) : min(min), max(max) {};

    inline bool contains(const double x) const
    {
        return (x >= min) && (x <= max);
    }

    inline bool surrounds(const double x) const
    {
        return (x > min) && (x < max);
    }

    inline double clamp(const double x) const
    {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif