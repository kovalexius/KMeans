#ifndef KMEANSMULTITHREAD
#define KMEANSMULTITHREAD

#include <iostream>
#include <vector>
#include <chrono>

#include <memory>
#include <atomic>
#include <thread>

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
					std::vector<unsigned int> &_indexes,
					std::vector<unsigned int> &_numPoints,
					std::vector<T> &_accumulator,
					AtomicFloat &_V) : m_begin(_begin),
													m_end(_end),
													m_vpnts(_vpnts),
													m_centers(_centers),
													m_indexes(_indexes),
													m_numPoints(_numPoints),
													m_accumulator(_accumulator),
													m_V(_V)
			{
			}
			
			~CKMTask()
			{
				//std::cout << __FUNCTION__ << " tid: " << std::this_thread::get_id() << std::endl;
			}

			virtual void Execute() override
			{
				for (unsigned int i = m_begin; i < m_end; i++)
				{
					float dist2; 
					m_indexes[i] = findNearest(m_vpnts[i], m_centers, dist2);
					
					/*
					auto ind = m_indexes[i];
					T pnt = m_vpnts[i] - m_centers[ind]; // участвует старый центроид
					m_V += pnt*pnt;
					m_accumulator[ind] += m_vpnts[i]; // вычисления вместе с выделением массива под центроиды
					m_numPoints[ind]++;
					*/
				}
			}
			
			
		private:
			const int m_begin, m_end;
			const std::vector<T> &m_vpnts;
			const std::vector<T> &m_centers;
			std::vector<unsigned int> &m_indexes;
			
			std::vector<unsigned int> &m_numPoints;
			std::vector<T> &m_accumulator;
			AtomicFloat &m_V;
		};

    public:
        CKmeans(const std::vector<T>& vpnts, unsigned int numGroups): m_vpnts(vpnts), 
																	m_numClusters(numGroups)
																	
        {
			m_pool.Init(8);
        }

        std::vector<std::pair<T, std::vector<T>>> Run()
        {
			kmeansPP(m_vpnts, m_numClusters, m_centers);
			
			kmeans();
			
            std::vector<std::pair<T, std::vector<T>>> result;
            for (auto e : m_centers)
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
		std::vector<T> m_centers;

		std::vector<std::shared_ptr<CKMTask>> m_tasks;
		mtp::CThreadPool m_pool;

        /*!
         *  Кластеризует, т.е. выбирает для каждого центроида точки, входящие в его кластер
         *  путем последовательного нахождения для каждой точки ближайшего к нему центроида
         *  Вход: vpnts - все точки множества, centers - цетроиды
         *  Выход: m_indexes - индексы для каждой точки, указывающие соответствующий центроид к которому каждая точка принадлежит.
         */
        void makeClusters()
        {
			//auto start = std::chrono::high_resolution_clock::now();
			
            if (m_indexes.size() < m_vpnts.size())
                m_indexes.resize(m_vpnts.size());
			
			
			for(int i = 0; i < m_tasks.size(); i++)
			{
				m_pool.AddTask(m_tasks[i].get());
			}
			for(int i = 0; i < m_tasks.size(); i++)
			{
				m_tasks[i]->Wait();
			}
			
			/*
            auto end = std::chrono::high_resolution_clock::now();
			float durationSec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
			std::cout << "Duration of " << __FUNCTION__ << " :" << durationSec << std::endl;
			*/
        }

        /*! Возвращает: новые центры
		 * 	Вход: vpnts - все точки множества, centers - цетроиды
		 *  Выход: m_indexes - индексы для каждой точки, указывающие соответствующий центроид к которому каждая точка принадлежит.
		 *  return: набор центроидов
		 */
		/*
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
        /**/
        
        
		void accumulate(std::vector<unsigned int> &_numPoints,
						std::vector<T> &_accumulator,
						AtomicFloat &_V)
		{
			for(unsigned int i = 0; i < m_vpnts.size(); i++)
			{
				auto ind = m_indexes[i];
				T pnt = m_vpnts[i] - m_centers[ind]; // участвует старый центроид
				_V += pnt*pnt;
				_accumulator[ind] += m_vpnts[i]; // вычисления вместе с выделением массива под центроиды
				_numPoints[ind]++;
			}
		}
		
		/*! Возвращает: новые центры
		 * 	Вход: vpnts - все точки множества, centers - цетроиды
		 *  Выход: m_indexes - индексы для каждой точки, указывающие соответствующий центроид к которому каждая точка принадлежит.
		 *  return: набор центроидов
		 */
		//*
        void kmeans()
        {
			auto start = std::chrono::high_resolution_clock::now();

			const int numTasks = 12;
			
            // Количество точек в кластерах
            std::vector<unsigned int> numPoints(m_centers.size());
			std::vector<T> accumulator(m_centers.size());

            // Суммарная разница между точкой и центроидом в кластерах - признак
            AtomicFloat V(0.0);
			float oldV = -1.0;
			
			
			auto curNums = m_vpnts.size()/(numTasks-1);
			auto additionly = m_vpnts.size()%(numTasks-1);
			
			
			int numIterations = 0;
            while(V != oldV)
            {
				auto start = std::chrono::high_resolution_clock::now();
				
				numIterations++;
				
				
                oldV = V;
                V = 0.0;

				for(int i = 0; i < (numTasks-1); i++)
				{
					m_tasks.push_back(std::make_shared<CKMTask>(i*curNums, 
																i*curNums + curNums, 
																m_vpnts, 
																m_centers, 
																m_indexes, 
																numPoints, 
																accumulator,
																V
																));
				}
				m_tasks.push_back(std::make_shared<CKMTask>((numTasks-1)*curNums, 
															(numTasks-1)*curNums + additionly, 
															m_vpnts, 
															m_centers, 
															m_indexes, 
															numPoints, 
															accumulator,
															V
															));
				
                // Перекластеризовать
                makeClusters();

				m_tasks.clear();
				
                accumulate(numPoints, accumulator, V);

                // Пересчитывание новых центроидов:
                // сложить все точки кластера в ячейку для центроида,
                // поделить на количество точек - получится новый центроид;
                // Цикл по кол-ву центроидов
                for (unsigned int i = 0; i < m_centers.size(); i++)
                {
                    if ( numPoints[i] > 0 )
                        m_centers[i] = accumulator[i]/(float)numPoints[i];
                    numPoints[i] = 0;
					accumulator[i] = T::getZero();
                }

                std::cout << "V: " << V << std::endl;
				
				//*
				auto end = std::chrono::high_resolution_clock::now();
				float durationSec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
				std::cout << "Duration of iteration :" << durationSec << std::endl;
				/**/
            }
            
            auto end = std::chrono::high_resolution_clock::now();
			float durationSec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
			std::cout << "Duration of " << __FUNCTION__ << " :" << durationSec << 
				" num of iterations: " << numIterations << std::endl;
        }
        /**/
    };

}

#endif // KMEANSMULTIRHREAD

