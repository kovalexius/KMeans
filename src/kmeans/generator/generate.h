#ifndef GENERATE
#define GENERATE

#include <vector>
#include "../userData/point.h"

namespace generators
{
    double randf(double m)
    {
        return m * rand() / ( RAND_MAX - 1 );
    }

    template<unsigned int Metric>
    void generate( unsigned int num_points, std::vector<CPoint<Metric>>& vpoints )
    {
        for( auto i = 0u; i < num_points; i++ )
        {
            CPoint<Metric> pnt;
            for( auto j = 0u; j < Metric; j ++ )
                pnt.setCoord( j, (float)randf(10) );
            vpoints.push_back( std::move(pnt) );
        }
    }
}


#endif // GENERATE

