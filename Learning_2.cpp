#include <iostream>
#include <thread>
#include <time.h>
#include <cmath>
#include <mutex>
#include <vector>
#include <chrono>
#include <sys/select.h>
//#include <conio.h>

#ifdef _WIN64
#include <Windows.h>
#endif

using namespace std;

class CTimer
{
	//LARGE_INTEGER t_start, t_end;
#ifdef linux
	timespec t_start, t_end;
#elif _WIN64
	long long t_start_speed, t_end_speed, t_start_count, t_end_count;
#endif
	long long spent_time;
public:
	void Start()
	{
#ifdef linux
		clock_gettime(CLOCK_REALTIME, &t_start);
#elif _WIN64
		QueryPerformanceFrequency((LARGE_INTEGER*)& t_start_speed);
		QueryPerformanceCounter((LARGE_INTEGER*)& t_start_count);
		//QueryPerformanceFrequency((LARGE_INTEGER*)& t_start);
#endif
		cout << "Counting started" << endl;
	}
	void End()
	{
#ifdef linux
		clock_gettime(CLOCK_REALTIME, &t_end);
#elif _WIN64
		QueryPerformanceFrequency((LARGE_INTEGER*)& t_end_speed);
		QueryPerformanceCounter((LARGE_INTEGER*)& t_end_count);
		//QueryPerformanceFrequency((LARGE_INTEGER*)& t_end);
#endif
		cout << "Counting finished" << endl;
	}
	void Count()
	{
#ifdef linux
		spent_time = 1000 * (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_nsec - t_start.tv_nsec) / 1000000;
#elif _WIN64
		spent_time = (1000 * t_end_count / t_end_speed) - (1000 * t_start_count / t_start_speed);
#endif
		//spent_time = t_end.tv_nsec - t_start.tv_nsec;
		cout << "Spent " << spent_time << " miliseconds" << endl;
	}

};
mutex mu;
vector<int> vect;
#ifdef linux
int kbhit()
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);
    if(select(1, &read_fd, NULL, NULL, &tv) == -1)
        return 0;
    if(FD_ISSET(0, &read_fd))
        return 1;

    return 0;
}
#endif
bool prime_detect(int n)
{
	for (int i = 2; i < n; ++i) {
		if (n % i == 0)
		{
			return false;
		}
	}
	return true;
}

void prime_gen()
{
	int num, i = 0;

	while (!kbhit())
	{

		num = rand() % 10000;

		if (prime_detect(num))
		{
			{
				lock_guard<mutex> guard(mu);
				//mu.lock();
				vect.push_back(num);

				cout << "Thread ID: " << this_thread::get_id() << " Num =\t" << num << endl;
				//mu.unlock();
			}
			this_thread::sleep_for(chrono::milliseconds(1000));
			i++;
		}

	}
}

int main()
{
	CTimer timer1, timer2, timer4, timer8;
	timer1.Start();

	vector<thread*> threads;
	//vector<thread&> threads;
	//for (int i = 0; i < 100; i++)
	while (threads.size() != 100)
	{
		thread* th = new thread(prime_gen);
		threads.push_back(th);
		//threads.push_back(thread(prime_gen));
	}


	while (!kbhit())
	{
		mu.lock();
		for (auto it = vect.begin(); it != vect.end(); ++it)
		{
			cout << *it << " ";
		}
		cout << endl;
		mu.unlock();
		this_thread::sleep_for(chrono::seconds(2));
	}

	for (int i = 0; i < 100; ++i)
	{
		threads[i]->join();
	}
	timer1.End();

	cout << endl;
	timer1.Count();
	cout << endl;
	cin.get();

}
