#ifndef __HELPERS__H
#define __HELPERS__H

#include <vector>

namespace multiThread
{
	/*!
	 *  Находит ближайший центроид для заданной точки pnt из уже имеющихся центроидов в centers
	 *  Возвращает dist2 - квадрат дистанции до боижайшего центроида и
	 *  индекс ближайшего центроида в качестве return value типа unsigned int
	 */
	template<typename T>
	unsigned int findNearest(const T& pnt, 
							const std::vector<T>& centers, 
							float& dist2)
	{
		unsigned int min_i;
		T PntCntr = centers[0] - pnt;
		dist2 = PntCntr * PntCntr;
		min_i = 0;
		for( unsigned int i = 1; i < centers.size(); i++ )
		{
			PntCntr = centers[i] - pnt;
			float d2 = PntCntr * PntCntr;
			if ( dist2 > d2 )
			{
				dist2 = d2;
				min_i = i;
			}
		}
		return min_i;
	}
}

#endif
