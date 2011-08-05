#include "MappedTest.h"

MappedTest::MappedTest()
{
}

uint MappedTest::write_everywhere(NVDSim::NVDIMM *NVDimm)
{
    NVDSim::FlashTransaction t;
    uint cycle = 0;
    uint packages = 0;
    uint dies = 0;
    uint planes = 0;
    uint blocks = 0;
    uint pages = 0;
    bool result = 0;

    while(cycle < 10000000)
    {
	while(packages < NUM_PACKAGES)
	{
	    while(dies < DIES_PER_PACKAGE)
	    {
		while(planes < PLANES_PER_DIE)
		{
		    while(blocks < VIRTUAL_BLOCKS_PER_PLANE)
		    {
			while(pages < PAGES_PER_BLOCK)
			{
			    uint64_t addr = NV_PAGE_SIZE * (pages + BLOCK_SIZE * (blocks + VIRTUAL_BLOCKS_PER_PLANE * (planes + PLANES_PER_DIE * (dies + NUM_PACKAGES * packages))));
			    // try to add a write
			    t= NVDSim::FlashTransaction(NVDSim::DATA_WRITE, addr, (void *)0xdeadbeef);
			    result = (*NVDimm).add(t);
	
			    // if the queues weren't full, then proceed to adding reads
			    if(result == 1)
			    {				
				pages++;
			    }

			    // we're going to be updating constantly
			    (*NVDimm).update();
			    //if (NVDimm->numWrites == num_writes)
			    //break;
			    cycle++;
			}
			blocks++;
			pages = 0;
		    }
		    planes++;
		    blocks = 0;
		}
		dies++;
		planes = 0;
	    }
	    packages++;
	    dies = 0;
	}
    	
	// we're going to be updating constantly
	(*NVDimm).update();
	//if (NVDimm->numWrites == num_writes)
	    //break;
	cycle++;
    }

    return cycle;
}

uint MappedTest::read_everywhere(NVDSim::NVDIMM *NVDimm)
{
    NVDSim::FlashTransaction t;
    uint cycle = 0;
    uint packages = 0;
    uint dies = 0;
    uint planes = 0;
    uint blocks = 0;
    uint pages = 0;
    bool result = 0;

    while(cycle < 10000000)
    {
	while(packages < NUM_PACKAGES)
	{
	    while(dies < DIES_PER_PACKAGE)
	    {
		while(planes < PLANES_PER_DIE)
		{
		    while(blocks < VIRTUAL_BLOCKS_PER_PLANE)
		    {
			while(pages < PAGES_PER_BLOCK)
			{
			    uint64_t addr = NV_PAGE_SIZE * (pages + BLOCK_SIZE * (blocks + VIRTUAL_BLOCKS_PER_PLANE * (planes + PLANES_PER_DIE * (dies + NUM_PACKAGES * packages))));
			    // try to add a write
			    t= NVDSim::FlashTransaction(NVDSim::DATA_READ, addr, (void *)0xdeadbeef);
			    result = (*NVDimm).add(t);
	
			    // if the queues weren't full, then proceed to adding reads
			    if(result == 1)
			    {				
				pages++;
			    }

			    // we're going to be updating constantly
			    (*NVDimm).update();
			    //if (NVDimm->numWrites == num_writes)
			    //break;
			    cycle++;
			}
			blocks++;
			pages = 0;
		    }
		    planes++;
		    blocks = 0;
		}
		dies++;
		planes = 0;
	    }
	    packages++;
	    dies = 0;
	}
    	
	// we're going to be updating constantly
	(*NVDimm).update();
	//if (NVDimm->numWrites == num_writes)
	    //break;
	cycle++;
    }

    return cycle;
}
