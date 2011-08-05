#ifndef NVMT_H
#define NVMT_H

#include <NVDIMM.h>

class MappedTest{
    public:
    MappedTest(void);
    uint write_everywhere(NVDSim::NVDIMM*);
    uint read_everywhere(NVDSim::NVDIMM*);
};

#endif
