#include "tools.h"

int main()
{
	ring_buf<int, 11> buf;
	ring_buf_fast<int, 8> buf_f;

	std::cout << "Capacity: " << buf.capacity() << std::endl;
	
	std::cout << "Printing empty... ";
	for (auto &it : buf) std::cout << it << ' ';

	std::cout << "\nFilling buffer..." << std::endl;
	for (int i = 1; i <= 10; ++i) buf.push_back(i); 

	std::cout << "Printing first time... ";
	for (auto &it : buf) std::cout << it << ' ';

	std::cout << "\nAdding 5 elements over tail... "  << std::endl;
	for (int i = 11; i <= 15; ++i) buf.push_back(i); 	

	std::cout << "Printing second time... ";
	for (auto &it : buf) std::cout << it << ' ';

	std::cout << "\nDeleting 4 elements... "  << std::endl;
	for (int i = 0; i < 4; ++i) buf.pop_front();

	std::cout << "Printing third time... ";
	for (auto &it : buf) std::cout << it << ' ';

	std::cout << "\nAdding 6 elements over tail... "  << std::endl;
	for (int i = 16; i <= 21; ++i) buf.push_back(i); 

	std::cout << "Printing last time... ";
	for (auto &it : buf) std::cout << it << ' ';

	std::cout << std::endl;

	std::cout << measure_time<1000000>(&decltype(buf)::push_back, &buf, 1) << " ns" << std::endl;
	std::cout << measure_time<1000000>(&decltype(buf_f)::push_back, &buf_f, 1) << " ns" <<std::endl;
}