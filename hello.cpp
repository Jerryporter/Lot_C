#include "mingw-threads/mingw.thread.h"
#include <chrono>
#include <iostream>
#include "frequentuse.h"
#include "timer.h"

using namespace std;
using namespace chrono;

void f1(int n)
{
    int stateKeepAlive = 0;
	time_t tt;
	system_clock::time_point today = system_clock::now();
	cout << "clock set up!" << endl;
	while (1)
	{
		steady_clock::time_point start = steady_clock::now();
		while (!stateKeepAlive)
		{
			steady_clock::time_point end = steady_clock::now();
			auto time_span = duration_cast<seconds>(end - start).count();
			if (time_span > 5)
			{
				stateKeepAlive = 1;
				tt = system_clock::to_time_t(today);
				cout << ctime(&tt);
			}
		}
		stateKeepAlive = 0;
	}
}
int main()
{
	thread t1(f1);
	t1.detach();
	int i=1;
	while(1){
		cout<<i++<<endl;
	}

	system("pause");
	return 0;
}