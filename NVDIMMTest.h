#ifndef NVTBS_H
#define NVTBS_H

#include <iostream>
#include <time.h>

#include <NVDIMM.h>

#include "MemLeakTest.h"
#include "MappedTest.h"

enum TestType{
    READ_WRITE_MIX1,
    READ_WRITE_MIX2,
    JUST_WRITES,
    SAME_WRITE,
    WRITE_EVERYWHERE,
    READ_EVERYWHERE
};

class tester{
	public:
		void read_cb(uint, uint64_t, uint64_t);
		void unmapped_cb(uint, uint64_t, uint64_t);
		void crit_cb(uint, uint64_t, uint64_t);
		void write_cb(uint, uint64_t, uint64_t);
		void power_cb(uint, vector<vector<double>>, uint64_t);
		void run_test(TestType);
};
#endif
