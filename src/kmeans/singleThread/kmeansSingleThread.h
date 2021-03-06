#ifndef KMEANSSINGLETHREAD
#define KMEANSSINGLETHREAD

#include <iostream>
#include <vector>
#include <utility>
#include <chrono>

#include <generate.h>


namespace singleThread
{
    template<typename T>
    class CKmeans
    {
    public:
        CKmeans( const std::vector<T>& vpnts, unsigned int numGroups ):  m_k( numGroups ), m_vpnts( vpnts )
        {
        }

        std::vector<std::pair<T, std::vector<T>>> Run()
        {
			auto start = std::chrono::high_resolution_clock::now();
            std::vector<T> cent;
            kmeansPP( m_vpnts, m_k, cent );
			auto end = std::chrono::high_resolution_clock::now();
			float durationSec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
			std::cout << "Duration of k-means++: " << durationSec << std::endl;
            
            // Индексы центроидов для каждой точки
            std::vector<unsigned int> indexes;
            std::vector<T> newCenters = kmeans( m_vpnts, cent, indexes );
            std::vector< std::pair<T, std::vector<T>> > result;
			start = std::chrono::high_resolution_clock::now();
			durationSec = std::chrono::duration_cast<std::chrono::microseconds>(start - end).count() / 1000000.0;
			std::cout << "Duration of k-means: " << durationSec << std::endl;

            for ( auto e : newCenters )
                result.push_back( std::pair<T, std::vector<T>>(e, std::vector<T>()) );
            for (unsigned int i = 0; i < m_vpnts.size(); i++)
            {
                auto ind = indexes[i];
                result[ind].second.push_back(m_vpnts[i]);
            }

            return result;
        }

    private:
		
		// Количество групп
        unsigned int m_k;
		
		// Количество всех точек
        const std::vector<T>& m_vpnts;


        /*!
         *  Находит ближайший центроид для заданной точки pnt из уже имеющихся центроидов в centers
         *  Возвращает dist2 - квадрат дистанции до боижайшего центроида и
         *  индекс ближайшего центроида в качестве return value типа unsigned int
         */
        unsigned int findNearest( const T& pnt, const std::vector<T>& centers, float& dist2 )
        {
            unsigned int min_i;
            //dist2 = HUGE_VAL;
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

        /*!
         *  Алгоритм k-means++, он более осмысленно выбирает начальные цендроиды из заданного множества - неслучайно, только первый центроид случайно
         *  Принимает всё множество точек в vpnts, желаемое количество центроидов в n_cent
         *  Возвращает центроиды в centers
         */
        void kmeansPP( const std::vector<T>& vpnts, const unsigned int n_cent, std::vector<T>& centers )
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

        /*!
         *  Кластеризует, т.е. выбирает для каждого центроида точки, входящие в его кластер
         *  путем последовательного нахождения для каждой точки ближайшего к нему центроида
         *  Вход: vpnts - все точки множества, centers - цетроиды
         *  Выход: indexes - индексы для каждой точки, указывающие соответствующий центроид к которому каждая точка принадлежит.
         */
        void makeClusters( const std::vector<T>& vpnts, const std::vector<T>& centers, std::vector<unsigned int>& indexes )
        {
			auto start = std::chrono::high_resolution_clock::now();
			
            if (indexes.size() < vpnts.size())
                indexes.resize(vpnts.size());
            for ( unsigned int i = 0; i < vpnts.size(); i++ )
            {
                float dist2; // Фиктивная переменная, здесь нужна по причине того, что функци его принимает
                indexes[i] = findNearest(vpnts[i], centers, dist2);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
			float durationSec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
			std::cout << "Duration of " << __FUNCTION__ << " :" << durationSec << std::endl;
        }

        //! Возвращает: новые центры
        std::vector<T> kmeans(const std::vector<T> &vpnts, 
							  std::vector<T>& centers, 
							  std::vector<unsigned int>& indexes)
        {
			auto start = std::chrono::high_resolution_clock::now();
			
            std::vector<T> centroids(centers.size());

            // Количество точек в кластерах
            std::vector<unsigned int> numPoints(centers.size());

            // Заводим два указателяь, которые будут указывать
            // попеременно на centers и centroids, в дальнейшем работаем со страыми и новыми центроидами через них
            std::vector<T> *centOld = &centers;   // Сначала так
            std::vector<T> *centNew = &centroids; // инициализируем
            
            // Суммарная разница между точкой и центроидом в кластерах
            float V = 0, oldV = -1;    // Сумма квадратных "отклонений" (всегда будет положительным ненулевым числом)
            int numIterations = 0;
            while( V != oldV )
            {
				auto start = std::chrono::high_resolution_clock::now();
				
				numIterations++;

                oldV = V;
                V = 0;   

                // Перекластеризовать
                makeClusters(vpnts, *centOld, indexes);

                for( unsigned int i = 0; i < vpnts.size(); i++ )
                {
                    unsigned int ind = indexes[i];
                    T pnt = vpnts[i] - centOld->operator[](ind); // участвует старый центроид
                    V += (float)(pnt * pnt);
                    centNew->operator[]( ind ) += vpnts[i]; // кладем в новый центроид
                    numPoints[ind]++;
                }

                // Обнулить, перезаписать нуль векторами
                // поделить на numPoints, обнулить numPoints;
                // Цикл по кол-ву центроидов
                for (unsigned int i = 0; i < centNew->size(); i++)
                {
                    centOld->operator[](i) = T::getZero();
                    if ( numPoints[i] > 0 )
                        centNew->operator[](i) /= (float)numPoints[i];
                    numPoints[i] = 0;
                }

                // Поменять местами Old и New
                std::swap(centNew, centOld);

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



#endif // KMEANS

