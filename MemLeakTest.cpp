#include "MemLeakTest.h"

MemLeak::MemLeak()
{
}

uint MemLeak::read_write_mix1(NVDSim::NVDIMM *NVDimm)
{
    uint num_writes = 1000;

    NVDSim::FlashTransaction t;
    uint cycle = 0;
    uint writes = 0;
    bool result = 0;
    int mode = 0;

    while(cycle < 7000000)
    {
	if(writes < num_writes)
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
			writes++;
			mode = 0;
		    }
		}
	    }
	}
    	
	// we're going to be updating constantly
	(*NVDimm).update();
	//if (NVDimm->numWrites == num_writes)
	    //break;
	cycle++;
    }

    return cycle;
}

uint MemLeak::read_write_mix2(NVDSim::NVDIMM *NVDimm)
{
    uint num_writes = 25;

    NVDSim::FlashTransaction t;
    uint cycle = 0;
    uint writes = 0;
    bool result = 0;
    uint reads = 0;

    while(writes < num_writes)
    {
	// try to add a write
	t= NVDSim::FlashTransaction(NVDSim::DATA_WRITE, writes, (void *)0xdeadbeef);
	result = (*NVDimm).add(t);
	
	// if the queues weren't full, then proceed to adding reads
	if(result == 1)
	{
	    writes++;
	}
    	
	// we're going to be updating constantly
	(*NVDimm).update();
	cycle++;
    }

    // make sure all the writes complete
    while(cycle < 3000000)
    {
	(*NVDimm).update();
	cycle++;
    }

    while(reads < num_writes)
    {
	// try to add a write
	t= NVDSim::FlashTransaction(NVDSim::DATA_READ, reads, (void *)0xdeadbeef);
	result = (*NVDimm).add(t);
	
	// if the queues weren't full, then proceed to adding reads
	if(result == 1)
	{
	    reads++;
	}
    	
	// we're going to be updating constantly
	(*NVDimm).update();
	//if (NVDimm->numWrites == num_writes)
	    //break;
	cycle++;
    }

    // make sure all the reads complete
    while(cycle < 50000000)
    {
	(*NVDimm).update();
	cycle++;
    }

    return cycle;
}

uint MemLeak::just_writes(NVDSim::NVDIMM *NVDimm)
{
    uint num_writes = 100000000;

    NVDSim::FlashTransaction t;
    uint cycle = 0;
    uint writes = 0;
    bool result = 0;
	
    while(writes < num_writes)
    {
	// try to add a write
	t= NVDSim::FlashTransaction(NVDSim::DATA_WRITE, writes, (void *)0xdeadbeef);
	result = (*NVDimm).add(t);
	
	// if the queues weren't full, then proceed to adding reads
	if(result == 1)
	{
	    writes++;
	}

	// we're going to be updating constantly
	(*NVDimm).update();
	if (NVDimm->numWrites == num_writes)
		break;
    }

    return cycle;
}

uint MemLeak::same_write(NVDSim::NVDIMM *NVDimm)
{
    uint num_writes = 80;

    NVDSim::FlashTransaction t;
    uint cycle = 0;
    uint writes = 0;
    bool result = 0;
	
    while(cycle < 5000000)
    {
	// try to add a write
	t= NVDSim::FlashTransaction(NVDSim::DATA_WRITE, writes, (void *)0xdeadbeef);
	result = (*NVDimm).add(t);
	
	// if the queues weren't full, then proceed to adding reads
	if(result == 1)
	{
	    writes++;
	}
	if(writes > num_writes)
	{
	    writes = 0;
	}

	// we're going to be updating constantly
	(*NVDimm).update();
    }

    return cycle;
}
