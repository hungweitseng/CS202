#include <iostream>
#include <thread>
#include <sys/time.h>

int main(int argc, char** argv)
{
    struct timeval time_start, time_end, program_start, program_end;
    gettimeofday(&program_start, NULL);	
    	gettimeofday(&time_start, NULL);
  for (volatile int i = 0; i < 500000; i++)
    std::thread([](){}).detach();
   	gettimeofday(&time_end, NULL);
   	std::cout << "Total time for creating 500,000 threads "<< ((time_end.tv_sec * 1000000 + time_end.tv_usec) - (time_start.tv_sec * 1000000 + time_start.tv_usec)) << " (us)\n";
   	std::cout << "average " <<((time_end.tv_sec * 1000000 + time_end.tv_usec) - (time_start.tv_sec * 1000000 + time_start.tv_usec))/500000.0 << " (us)\n";
  return 0;
}