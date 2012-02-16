#include "MappedTest.h"

MappedTest::MappedTest()
{
}

uint MappedTest::write_everywhere(NVDSim::NVDIMM *NVDimm)
{
    NVDSim::FlashTransaction t;
    uint64_t cycle = 0;
    uint64_t packages = 0;
    uint64_t dies = 0;
    uint64_t planes = 0;
    uint64_t blocks = 0;
    uint64_t pages = 0;
    bool result = 0;

    while(cycle < 10000000)
    {
	while(packages < NVDSim::NUM_PACKAGES)
	{
	    while(dies < NVDSim::DIES_PER_PACKAGE)
	    {
		while(planes < NVDSim::PLANES_PER_DIE)
		{
		    while(blocks < NVDSim::VIRTUAL_BLOCKS_PER_PLANE)
		    {
			while(pages < NVDSim::PAGES_PER_BLOCK)
			{
			    uint64_t temp_address = NVDSim::NV_PAGE_SIZE; // * (pages + BLOCK_SIZE * (blocks + NVDSim::VIRTUAL_BLOCKS_PER_PLANE * (planes + NVDSim::PLANES_PER_DIE * (dies + NVDSim::NUM_PACKAGES * packages))));
			    // try to add a write
			    t= NVDSim::FlashTransaction(NVDSim::DATA_WRITE, temp_address, (void *)0xdeadbeef);
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
    uint64_t cycle = 0;
    uint64_t packages = 0;
    uint64_t dies = 0;
    uint64_t planes = 0;
    uint64_t blocks = 0;
    uint64_t pages = 0;
    bool result = 0;

    while(cycle < 10000000)
    {
	while(packages < NVDSim::NUM_PACKAGES)
	{
	    while(dies < NVDSim::DIES_PER_PACKAGE)
	    {
		while(planes < NVDSim::PLANES_PER_DIE)
		{
		    while(blocks < NVDSim::VIRTUAL_BLOCKS_PER_PLANE)
		    {
			while(pages < NVDSim::PAGES_PER_BLOCK)
			{
			    uint64_t addr = NVDSim::NV_PAGE_SIZE; // * (pages + BLOCK_SIZE * (blocks + NVDSim::VIRTUAL_BLOCKS_PER_PLANE * (planes + NVDSim::PLANES_PER_DIE * (dies + NVDSim::NUM_PACKAGES * packages))));
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
