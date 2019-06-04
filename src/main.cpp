// Test of kmeans algorithm

#include <iostream>
#include <atomic>
#include <vector>
#include <string>
#include <cstdint>

#include "src/userData/point.h"
#include "src/generator/generate.h"
#include "src/kmeans/singleThread/kmeansSingleThread.h"

//#include "src/kmeans/multiThread/test_lockfree.h"


using namespace std;

const uint32_t NUM_POINTS = 50000; // Количество точек
const uint32_t NUM_METRICS = 5;    // Метрика-размерность точек
const uint32_t NUM_CLUSTERS = 20;  // Количество кластеров

//int Foo::val = 5;

/*
class Bit
{
public:
	int a1 : 1;
	int a2 : 1;
	int a3 : 1;
	int a4 : 1;
	int a5 : 1;
	int a6 : 1;
	int a7 : 1;
	int a8 : 1;
};
*/

class logger
{
public: 
	//explicit 
	logger(int) {}
};

int main()
{
        //logger log = 1;
	/*
	union
	{
		Bit bit;
		char a;
	};

	a = 12;
	cout << bit.a1 << bit.a2 << bit.a3 << bit.a4 << bit.a5 << bit.a6 << bit.a7 << bit.a8;

	Foo foo;

	cout << foo.getVal() << endl;
	*/
    //std::cin.get();

    //test::test_lockfree();

    typedef CPoint<NUM_METRICS> MYPOINT;
    vector<MYPOINT> vpoints;

    generators::generate<NUM_METRICS>( NUM_POINTS, vpoints );

	singleThread::CKmeans<MYPOINT> kmeans(vpoints, NUM_CLUSTERS);

	vector<pair<MYPOINT, vector<MYPOINT>>> result = kmeans.Run();


	for (auto e : result)
		cout << e.first << " " << endl;

	//getc();
	std::cin.get();

    return 0;
}

