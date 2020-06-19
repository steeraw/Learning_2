#include <iostream>
#include <thread>
#include <time.h>
#include <cmath>
#include <mutex>
#include <vector>
#include <chrono>
#include <sys/select.h>
#include <fstream>
#include <string>
#include <stdio.h>
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
//vector<int> vect;
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
int flag = 0;
fstream fs;
string path = "File.txt";
//FILE *MyFile;
void prime_gen()
{
	int num, i = 0;

	while (flag == 0)
	{

		num = rand() % 100000000;

		if (prime_detect(num))
		{
			{
				lock_guard<mutex> guard(mu);
				//mu.lock();
				//vect.push_back(num);

				/*MyFile = fopen("File.txt", "a");
				if(MyFile != NULL)
                {
				    fscanf(MyFile, "%d", num);

                }
				else
                {
				    printf("error - file not found\n");
                }*/

				fs.open(path, fstream::out | fstream::app);
				if(!fs.is_open())
                {

                    cout << "file not found" << endl;
                    break;
                }
				else
                {
				    //cout << "opened" << endl;
                    fs << num << ' ';
				}
				fs.close();


				//mu.unlock();
			}

			i++;
		}

	}
}

int main()
{
	CTimer timer1, timer2, timer4, timer8;
	timer1.Start();
    char str[10000];

	vector<thread*> threads;
	//vector<thread&> threads;
	//for (int i = 0; i < 100; i++)
	int size = 20;
	while (threads.size() != size)
	{
		thread* th = new thread(prime_gen);
		threads.push_back(th);
		//threads.push_back(thread(prime_gen));
	}


	while (flag == 0)
	{
		mu.lock();
		/*for (auto it = vect.begin(); it != vect.end(); ++it)
		{
			cout << *it << " ";
		}*/
		fs.open(path, fstream::in);
		if(fs.is_open())
        {
		    char ch;
		    while (fs.get(ch))
            {
		        cout << ch;
            }
        }
		else
        {
		    cout << "error: file not found" << endl;
            break;
		}
		fs.close();
        /*MyFile = fopen("File.txt", "a");
        if(MyFile != NULL)
        {
            while(fgets(str, 10000, MyFile))
                fprintf(stdout, "%d", str);


        }
        else
        {
            printf("error - file not found\n");
        }*/


		cout << endl;
		mu.unlock();
		this_thread::sleep_for(chrono::seconds(2));
		if(kbhit())
        {
		    flag = 1;
        }
	}

	for (int i = 0; i < size; ++i)
	{
		threads[i]->join();
	}
	timer1.End();

	cout << endl;
	timer1.Count();
	cout << endl;

	//cin.get();
    fs.open(path, fstream::out);
    fs.clear();
    fs.close();

}
