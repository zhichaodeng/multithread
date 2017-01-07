#include <iostream>
#include <thread>
#include <future>
#include <atomic>
#include <chrono>
#include <vector>
using namespace std;

using timeT = chrono::steady_clock::time_point; 
inline uint64_t square(uint32_t i) {
	return i * i;
	//cout<<"inputID="<<ftid<<", thread#"<<tid<<": enqueue "<<i<<endl;
}

void asyncSum(uint32_t num) {
	vector<future<uint64_t>> res;
	uint64_t sum = 0;
	timeT start= std::chrono::steady_clock::now();
	for (uint32_t i = 1; i<=num; ++i) {
		res.push_back(async(launch::async, square, i));
	}

	//!! MUST use &: copy ctor thread(thread&)=deleted
	for ( future<uint64_t> &t : res ) {
		sum += t.get();
	}
	timeT end= std::chrono::steady_clock::now();
	cout<<"parallel async time="<<chrono::duration_cast<chrono::nanoseconds>(end-start).count()<<" ns"<<endl;
	cout<<"sum=SUM(x^2), x=[1,20] == "<<sum<<endl;
}

atomic<uint64_t> asum(0);
inline void atomicSquare(uint32_t x) {
	asum += x*x;
}

void atomicSum(uint32_t num) {
	vector<thread> vt;
	timeT start= std::chrono::steady_clock::now();
	for (uint32_t i = 1; i<=num; ++i) {
		vt.push_back(thread(atomicSquare, i));
	}

	for (auto &t : vt) if (t.joinable()) t.join();
	timeT end= std::chrono::steady_clock::now();
	cout<<"parallel async time="<<chrono::duration_cast<chrono::nanoseconds>(end-start).count()<<" ns"<<endl;
	cout<<"sum=SUM(x^2), x=[1,20] == "<<asum<<endl;
}

void singleSum(uint32_t num) {
	uint32_t ssum = 0;
	timeT start= std::chrono::steady_clock::now();
	for (uint32_t i = 1; i<=num; ++i) {
		ssum += i*i;
	}

	timeT end= std::chrono::steady_clock::now();
	cout<<"single threadtime="<<chrono::duration_cast<chrono::nanoseconds>(end-start).count()<<" ns"<<endl;
	cout<<"sum=SUM(x^2), x=[1,20] == "<<ssum<<endl;
}

void testSuite(uint32_t low, uint32_t high) {
	for (uint32_t num=low; num<=high; num*=10) {
		cout<<"===== Test for num="<<num<<endl;
		asyncSum(num);

		atomicSum(num);
	
		singleSum(num);
	}
}

int main() {
    uint32_t lowN = 20;
    uint32_t highN= 20000000;
	testSuite(lowN, highN);
	return 0;
}
