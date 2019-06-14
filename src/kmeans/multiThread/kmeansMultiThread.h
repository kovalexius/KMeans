#ifndef KMEANSMULTITHREAD
#define KMEANSMULTITHREAD

#include <iostream>
#include <vector>
#include <chrono>

#include <memory>


#include <unistd.h>

#include "threadpool.h"

#include "kmeans_pp.h"
#include "helpers.h"

namespace multiThread
{
    template<typename T>
    class CKmeans
    {
		class CKMTask : public mtp::CTask
		{
		public:
			CKMTask(const int _begin,
					const int _end,
					const std::vector<T> &_vpnts,
					const std::vector<T> &_centers,
					std::vector<unsigned int> &_indexes) : m_begin(_begin),
														m_end(_end),
														m_vpnts(_vpnts),
														m_centers(_centers),
														m_indexes(_indexes)
			{
				
			}

			virtual void Execute() override
			{
				for ( unsigned int i = m_begin; i < m_end; i++)
				{
					float dist2; 
					m_indexes[i] = findNearest(m_vpnts[i], m_centers, dist2);
				}
			}
			
			
		private:
			const int m_begin, m_end;
			const std::vector<T> &m_vpnts;
			const std::vector<T> &m_centers;
			std::vector<unsigned int> &m_indexes;
		};

    public:
        CKmeans(const std::vector<T>& vpnts, unsigned int numGroups): m_vpnts(vpnts), 
																	m_numClusters(numGroups)
																	
        {
			m_pool.Init(8);
        }

        std::vector<std::pair<T, std::vector<T>>> Run()
        {
            std::vector<T> cent;
            kmeansPP(m_vpnts, m_numClusters, cent);
			
            // Индексы центроидов для каждой точки
            std::vector<T> newCenters = kmeans(cent);
			
            std::vector<std::pair<T, std::vector<T>>> result;
            for (auto e : newCenters)
                result.push_back(std::pair<T, std::vector<T>>(e, std::vector<T>()));
            for (unsigned int i = 0; i < m_vpnts.size(); i++)
            {
                auto ind = m_indexes[i];
                result[ind].second.push_back(m_vpnts[i]);
            }

            return result;
        }

    private:
        unsigned int m_numClusters;
        const std::vector<T>& m_vpnts;
		std::vector<unsigned int> m_indexes;
		mtp::CThreadPool m_pool;

        /*!
         *  Кластеризует, т.е. выбирает для каждого центроида точки, входящие в его кластер
         *  путем последовательного нахождения для каждой точки ближайшего к нему центроида
         *  Вход: vpnts - все точки множества, centers - цетроиды
         *  Выход: m_indexes - индексы для каждой точки, указывающие соответствующий центроид к которому каждая точка принадлежит.
         */
        void makeClusters(const std::vector<T>& centers)
        {
			auto start = std::chrono::high_resolution_clock::now();
			
            if (m_indexes.size() < m_vpnts.size())
                m_indexes.resize(m_vpnts.size());
			
			auto curNums = m_vpnts.size()/8;
			auto additionly = m_vpnts.size()%8;
			CKMTask *tasks[9];
			for(int i = 0; i < 8; i++)
			{
				tasks[i] = new CKMTask(i*curNums, i*curNums + curNums, m_vpnts, centers, m_indexes);
			}
			tasks[8] = new CKMTask(8*curNums, 8*curNums + additionly, m_vpnts, centers, m_indexes);			
			for(int i = 0; i < 9; i++)
			{
				m_pool.AddTask(tasks[i]);
			}
			for(int i = 0; i < 9; i++)
			{
				tasks[i]->Wait();
				delete tasks[i];
			}
			
            auto end = std::chrono::high_resolution_clock::now();
			float durationSec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
			std::cout << "Duration of " << __FUNCTION__ << " :" << durationSec << std::endl;
        }

        /*! Возвращает: новые центры
		 * 	Вход: vpnts - все точки множества, centers - цетроиды
		 *  Выход: m_indexes - индексы для каждой точки, указывающие соответствующий центроид к которому каждая точка принадлежит.
		 *  return: набор центроидов
		 */
        std::vector<T> kmeans(std::vector<T> &centers)
        {
			auto start = std::chrono::high_resolution_clock::now();
			
            std::vector<T> centroids(centers.size());

            // Количество точек в кластерах
            std::vector<unsigned int> numPoints(centers.size());

            // Заводим два указателяь, которые будут указывать
            // попеременно на centers и centroids, в дальнейшем работаем со страыми и новыми центроидами через них
            std::vector<T> *centOld = &centers;   // Сначала так
            std::vector<T> *centNew = &centroids; // инициализируем

            // Суммарная разница между точкой и центроидом в кластерах - признак
            float V = 0.0, oldV = -1;
			int numIterations = 0;
            while(V != oldV)
            {
				auto start = std::chrono::high_resolution_clock::now();
				
				numIterations++;
				
				
                oldV = V;
                V = 0.0;

                // Перекластеризовать
                makeClusters(*centOld);

                for( unsigned int i = 0; i < m_vpnts.size(); i++ )
                {
                    auto ind = m_indexes[i];
                    T pnt = m_vpnts[i] - centOld->operator[](ind); // участвует старый центроид
                    V += pnt*pnt;
                    centNew->operator[](ind) += m_vpnts[i]; // вычисления вместе с выделением массива под центроиды
                    numPoints[ind]++;
                }

                // Пересчитывание новых центроидов:
                // сложить все точки кластера в ячейку для центроида,
                // поделить на количество точек - получится новый центроид;
                // Цикл по кол-ву центроидов
                for (unsigned int i = 0; i < centNew->size(); i++)
                {
                    centOld->operator[](i) = T::getZero();
                    if ( numPoints[i] > 0 )
                        centNew->operator[](i) /= (float)numPoints[i];
                    numPoints[i] = 0;
                }

                // Поменять местами Old и New
                swap(centNew, centOld);

                std::cout << "V: " << V << std::endl;
				
				auto end = std::chrono::high_resolution_clock::now();
				float durationSec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
				std::cout << "Duration of iteration :" << durationSec << std::endl;
            }
            
            auto end = std::chrono::high_resolution_clock::now();
			float durationSec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
			std::cout << "Duration of " << __FUNCTION__ << " :" << durationSec << 
				" num of iterations: " << numIterations << std::endl;

            return *centOld;
        }
    };

}

#endif // KMEANSMULTIRHREAD

