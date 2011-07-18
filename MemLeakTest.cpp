#include "MemLeakTest.h"

MemLeak::MemLeak()
{
}

uint MemLeak::read_write_mix1(NVDSim::NVDIMM *NVDimm)
{
    uint num_writes = 100000000;

    NVDSim::FlashTransaction t;
    uint cycle = 0;
    uint writes = 0;
    bool result = 0;
    int mode = 0;
	
    while(writes < num_writes)
    {
	// write mode
	if(mode == 0)
	{
	    // try to add a write
	    t= NVDSim::FlashTransaction(NVDSim::DATA_WRITE, writes, (void *)0xdeadbeef);
	    result = (*NVDimm).add(t);
	
	    // if the queues weren't full, then proceed to adding reads
	    if(result == 1)
	    {
		writes++;
		mode = 1;
	    }
	}
	// read mode (add 10 reads for every write)
	else
	{
	    // try to add a read
	    t= NVDSim::FlashTransaction(NVDSim::DATA_READ, writes, (void *)0xdeadbeef);
	    result = (*NVDimm).add(t);
	
	    // if the queues weren't full
	    if(result == 1)
	    {
		// count that read
		mode++;
		// if we're done adding reads go back to adding writes
		if(mode > 10)
		{
		    mode = 0;
		}
	    }
	}

	// we're going to be updating constantly
	(*NVDimm).update();
	if (NVDimm->numWrites == num_writes)
		break;
    }

    return cycle;
}
