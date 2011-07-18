#ifndef NVTBS_H
#define NVTBS_H

#include <iostream>
#include <time.h>

#include <NVDIMM.h>

#include "MemLeakTest.h"

enum TestType{
    READ_WRITE_MIX1,
    READ_WRITE_MIX2
};

class tester{
	public:
		void read_cb(uint, uint64_t, uint64_t);
		void write_cb(uint, uint64_t, uint64_t);
		void power_cb(uint, vector<vector<double>>, uint64_t);
		void run_test(TestType);
};
#endif
