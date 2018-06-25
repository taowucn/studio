#include <iostream>
#include <cassert>
#include <atomic>
#include <vector>
#include <unistd.h>
#include <thread>
#include <sstream>

std::atomic <int> G_num(0);
std::atomic_flag lock = ATOMIC_FLAG_INIT;

void add_foo()
{
	int i = 1000;
	while (i--) {
		G_num++;
	}
}

int main()
{
	std::thread th1 = std::thread(add_foo);
	std::thread th2 = std::thread(add_foo);

	th1.join();
	th2.join();

	std::cout <<"Result: " << G_num << std::endl;

	return 0;
}
