#ifndef __KMEANS_PP__H
#define __KMEANS_PP__H

#include <vector>

#include "helpers.h"
#include "../generator/generate.h"

namespace multiThread
{
	/*!
	*  Алгоритм k-means++, он более осмысленно выбирает начальные цендроиды из заданного множества - неслучайно, только первый центроид случайно
	*  Принимает всё множество точек в vpnts, желаемое количество центроидов в n_cent
	*  Возвращает центроиды в centers
	*/
	template<typename T>
	void kmeansPP(const std::vector<T>& vpnts,
				  const unsigned int n_cent,
				  std::vector<T>& centers)
	{
		std::vector<float> dist2;
		dist2.resize( vpnts.size() );

		if ( centers.size() < n_cent )
			centers.resize( n_cent );

		// Первый центроид выбираем случайно
		centers[0] = vpnts[ rand() % vpnts.size() ];

		// Суммируем минимальное расстояние от каждой точки до ближайшего уже имеющегося центроида,
		// затем находим случайную величину от 0 до этой sum (суммы)
		// затем выбираем следующий центроид путем последовательного вычитания расстояния от каждой точки до ближайшего центроида из этой суммы
		for ( unsigned int n_cluster = 1; n_cluster < n_cent; n_cluster++ )
		{
			float sum = 0;
			for( unsigned int j = 0; j < vpnts.size(); j++ )
			{
				findNearest( vpnts[j], centers, dist2[j] );
				sum += dist2[j];
			}
			sum = generators::randf(sum);
			for( unsigned int j = 0; j < vpnts.size(); j++ )
			{
				sum -= dist2[j];
				if( sum <= 0 )
				{
					centers[n_cluster] = vpnts[j];
					break;
				}
			}
		}
	}
}

#endif
