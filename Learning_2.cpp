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
#include "library/library.h"
//#include <conio.h>

#ifdef _WIN64
#include <Windows.h>
#endif

using namespace std;

mutex mu;

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
//string path = "File.txt";
int fcount = 0;
FILE *MyFileW = fopen("File.txt", "wb");
void prime_gen()
{
	int i = 0;
	while (flag == 0)
	{
		int num = rand() % 100000000;
		if (prime_detect(num))
		{
                lock_guard<mutex> guard(mu);
				if(MyFileW != nullptr)
                {
				    fcount += fwrite(&num, sizeof(int), 1, MyFileW);
                    fflush(MyFileW);
                }
				else
                {
				    printf("error - file not found\n");
                }
			i++;
		}
	}
}

int main()
{
	CTimer timer1;
	timer1.Start();
    vector<int> numbers;
	vector<thread*> threads;
	int size = 20;
//	int total_read = 0;
	while (threads.size() != size)
	{
		thread* th = new thread(prime_gen);
		threads.push_back(th);
	}

	while (flag == 0)
	{
//        int rcount = 0;
//        while(rcount != fcount)
//        {
            FILE *MyFileR = fopen("File.txt", "rb");
            if (MyFileR != nullptr)
            {
                int fnum;
//                fseek(MyFileR, total_read, SEEK_SET);
                lock_guard<mutex> guard(mu);
                {
                    while (!feof(MyFileR))
                    {
                        int ret = fread(&fnum, sizeof(int), 1, MyFileR);

                        if (ret > 0)
                        {
//                            total_read += ret * sizeof(int);
                            printf("%d ", fnum);
                        }
                        else
                            break;
                    }
                    fclose(MyFileR);
                }
            }
            else
            {
                printf("error - file not found\n");
            }
//            rcount++;
//        }
//        total_read = 0;
		cout << endl;
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
    fclose(MyFileW);
}
