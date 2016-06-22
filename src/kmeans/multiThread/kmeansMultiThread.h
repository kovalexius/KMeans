#ifndef KMEANSMULTITHREAD
#define KMEANSMULTITHREAD

#include "threadpool.h"

namespace multiThread
{
    template<typename T>
    class CKmeans
    {
		class CKMTask : public mtp::CTask
		{
			CKMTask()
			{

			}

			virtual void Execute() override
			{

			}
		};

    public:
        CKmeans( const vector<T>& vpnts, unsigned int numGroups ): m_vpnts( vpnts ), m_k( numGroups )
        {
        }

        vector< pair< T, vector<T> > > Run()
        {
            vector<T> cent;
            kmeansPP( m_vpnts, m_k, cent );
            // Индексы центроидов для каждой точки
            vector<unsigned int> indexes;
            vector<T> newCenters = kmeans( m_vpnts, cent, indexes );
            vector< pair<T, vector<T>> > result;

            for ( auto e : newCenters )
                result.push_back( pair<T, vector<T>>( e, vector<T>()) );
            for (unsigned int i = 0; i < m_vpnts.size(); i++)
            {
                auto ind = indexes[i];
                result[ind].second.push_back(m_vpnts[i]);
            }

            return result;
        }

    private:
        unsigned int m_k;
        const vector<T>& m_vpnts;


        /*!
         *  Находит ближайший центроид для заданной точки pnt из уже имеющихся центроидов в centers
         *  Возвращает dist2 - квадрат дистанции до боижайшего центроида и
         *  индекс ближайшего центроида в качестве return value типа unsigned int
         */
        unsigned int findNearest( const T& pnt, const vector<T>& centers, float& dist2 )
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
        void kmeansPP( const vector<T>& vpnts, const unsigned int n_cent, vector<T>& centers )
        {
            vector<float> dist2;
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
        void makeClusters( const vector<T>& vpnts, const vector<T>& centers, vector<unsigned int>& indexes )
        {
            if (indexes.size() < vpnts.size())
                indexes.resize(vpnts.size());
            for ( unsigned int i = 0; i < vpnts.size(); i++ )
            {
                float dist2; // Фиктивная переменная, здесь нужна по причине того, что функци его принимает
                indexes[i] = findNearest( vpnts[i], centers, dist2 );
            }
        }

        //! Возвращает: новые центры
        vector<T> kmeans( const vector<T> &vpnts, vector<T>& centers, vector<unsigned int>& indexes )
        {
            vector<T> centroids;
            centroids.resize( centers.size() );

            // Количество точек в кластерах
            vector<unsigned int> numPoints;
            numPoints.resize(centers.size());

            // Суммарная разница между точкой и центроидом в кластерах
            //vector<T> sumVec;
            //sumVec.resize(centers.size());

            // Заводим два указателяь, которые будут указывать
            // попеременно на centers и centroids, в дальнейшем работаем со страыми и новыми центроидами через них
            vector<T> *centOld;
            vector<T> *centNew;
            centOld = &centers;   // Сначала так
            centNew = &centroids; // инициализируем
            float V = 0, oldV = -1;
            while( V != oldV )
            {
                oldV = V;
                V = 0;

                // Перекластеризовать
                makeClusters(vpnts, *centOld, indexes);

                for( unsigned int i = 0; i < vpnts.size(); i++ )
                {
                    unsigned int ind = indexes[i];
                    T pnt = vpnts[i] - centOld->operator[](ind); // участвует старый центроид
                    V += pnt*pnt;
                    centNew->operator[]( ind ) += vpnts[i]; // кладем в новый центроид
                    numPoints[ind]++;
                }

                // Обнулить, перезаписать нуль векторами
                // поделить на numPoints, обнулить numPoints;
                // Цикл по кол-ву центроидов
                for (unsigned int i = 0; i < centNew->size(); i++)
                {
                    //V += sumVec[i] * sumVec[i];
                    //sumVec[i] = T::getZero();
                    centOld->operator[](i) = T::getZero();
                    if ( numPoints[i] > 0 )
                        centNew->operator[](i) /= (float)numPoints[i];
                    numPoints[i] = 0;
                }

                // Поменять местами Old и New
                swap(centNew, centOld);

                cout << "V: " << V << endl;
            }

            return *centOld;
        }
    };

}

#endif // KMEANSMULTIRHREAD

