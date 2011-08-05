#ifndef NVMLT_H
#define NVMLT_H

#include <NVDIMM.h>

class MemLeak{
    public:
    MemLeak(void);
    uint read_write_mix1(NVDSim::NVDIMM*);
    uint read_write_mix2(NVDSim::NVDIMM*);
    uint just_writes(NVDSim::NVDIMM*);
    uint same_write(NVDSim::NVDIMM*);
};

#endif
