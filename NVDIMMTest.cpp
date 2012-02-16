/*TraceBasedSim.cpp
 *
 * This will eventually run traces. Right now the name is a little misleading...
 * It adds a certain amount (NUM_WRITES) of write transactions to the flash dimm
 * linearly starting at address 0 and then simulates a certain number (SIM_CYCLES)
 * of cycles before exiting.
 *
 * The output should be fairly straightforward. If you would like to see the writes
 * as they take place, change OUTPUT= 0; to OUTPUT= 1;
 */

#include "NVDIMMTest.h"

uint OUTPUT= 1;

using namespace std;

int main(int argc, char *argv[]){
	tester t;
	for(int i = 0; i < argc; i++)
	{
	    std::string str = argv[i];
	    if(str.compare("READ_WRITE_MIX1") == 0)
	    {
		t.run_test(READ_WRITE_MIX1);
	    }
	    if(str.compare("READ_WRITE_MIX2") == 0)
	    {
		t.run_test(READ_WRITE_MIX2);
	    }
	    else if(str.compare("JUST_WRITES") == 0)
	    {
		t.run_test(JUST_WRITES);
	    }
	    else if(str.compare("SAME_WRITE") == 0)
	    {
		t.run_test(SAME_WRITE);
	    }
	    else if(str.compare("WRITE_EVERYWHERE") == 0)
	    {
		t.run_test(WRITE_EVERYWHERE);
	    }
	    else if(str.compare("READ_EVERYWHERE") == 0)
	    {
		t.run_test(READ_EVERYWHERE);
	    }
	}
	return 0;
}

void tester::read_cb(uint id, uint64_t address, uint64_t cycle, bool mapped){
	cout<<"[Callback] read complete: "<<id<<" "<<address<<" cycle="<<cycle<<" mapped="<<mapped<<"\n";
}

void tester::unmapped_cb(uint id, uint64_t address, uint64_t cycle, bool mapped){
	cout<<"[Callback] unmapped read complete: "<<id<<" "<<address<<" cycle="<<cycle<<"\n";
}

void tester::crit_cb(uint id, uint64_t address, uint64_t cycle, bool mapped){
	cout<<"[Callback] crit line done: "<<id<<" "<<address<<" cycle="<<cycle<<"\n";
}

void tester::write_cb(uint id, uint64_t address, uint64_t cycle, bool mapped){
	cout<<"[Callback] write complete: "<<id<<" "<<address<<" cycle="<<cycle<<"\n";
}

void tester::power_cb(uint id, vector<vector<double>> data, uint64_t cycle, bool mapped){
        cout<<"[Callback] Power Data for cycle: "<<cycle<<"\n";
	for(uint i = 0; i < NVDSim::NUM_PACKAGES; i++){
	  for(uint j = 0; j < data.size(); j++){
	      if(NVDSim::DEVICE_TYPE.compare("PCM") == 0){
	      if(j == 0){
		cout<<"    Package: "<<i<<" Idle Energy: "<<data[0][i]<<"\n";
	      }else if(j == 1){
		cout<<"    Package: "<<i<<" Access Energy: "<<data[1][i]<<"\n";
	      }
	      if(NVDSim::GARBAGE_COLLECT == 1){
		if(j == 2){
		  cout<<"    Package: "<<i<<" Erase Energy: "<<data[2][i]<<"\n";
		}else if(j == 3){
		  cout<<"    Package: "<<i<<" VPP Idle Energy: "<<data[3][i]<<"\n";
		}else if(j == 4){
		  cout<<"    Package: "<<i<<" VPP Access Energy: "<<data[4][i]<<"\n";
		}else if(j == 5){
		  cout<<"    Package: "<<i<<" VPP Erase Energy: "<<data[5][i]<<"\n";
		}
	      }else{
		if(j == 2){
		  cout<<"    Package: "<<i<<" VPP Idle Energy: "<<data[2][i]<<"\n";
		}else if(j == 3){
		  cout<<"    Package: "<<i<<" VPP Access Energy: "<<data[3][i]<<"\n";
		}
	      }
	    }else{
	      if(j == 0){
		cout<<"    Package: "<<i<<" Idle Energy: "<<data[0][i]<<"\n";
	      }else if(j == 1){
		cout<<"    Package: "<<i<<" Access Energy: "<<data[1][i]<<"\n";
	      }else if(j == 2){
		cout<<"    Package: "<<i<<" Erase Energy: "<<data[2][i]<<"\n";
	      }
	    }
	  }
	}
}

void tester::run_test(TestType test){
	clock_t start= clock(), end;
	uint cycle = 0;

	NVDSim::NVDIMM *NVDimm = new NVDSim::NVDIMM(1,"ini/samsung_K9XXG08UXM_gc_test.ini","ini/def_system.ini","../NVDIMMTest","");

	// Set up the callbacks
	typedef NVDSim::CallbackBase<void,uint,uint64_t,uint64_t,bool> Callback_t;
	NVDSim::Callback_t *r = new NVDSim::Callback<tester, void, uint, uint64_t, uint64_t, bool>(this, &tester::read_cb);
	//NVDSim::Callback_t *u = new NVDSim::Callback<tester, void, uint, uint64_t, uint64_t, bool>(this, &tester::unmapped_cb);
	//NVDSim::Callback_t *c = new NVDSim::Callback<tester, void, uint, uint64_t, uint64_t, bool>(this, &tester::crit_cb);
	NVDSim::Callback_t *w = new NVDSim::Callback<tester, void, uint, uint64_t, uint64_t, bool>(this, &tester::write_cb);
	NVDSim::Callback_v *p = new NVDSim::Callback<tester, void, uint, vector<vector<double>>, uint64_t, bool>(this, &tester::power_cb);
	NVDimm->RegisterCallbacks(r, w, p);
	
	if(test == READ_WRITE_MIX1)
	{
	    MemLeak *temp_test = new MemLeak();
	    cycle = temp_test->read_write_mix1(NVDimm);
	}
	else if(test == READ_WRITE_MIX2)
	{
	    MemLeak *temp_test = new MemLeak();
	    cycle = temp_test->read_write_mix2(NVDimm);
	}
	else if(test == JUST_WRITES)
	{
	    MemLeak *temp_test = new MemLeak();
	    cycle = temp_test->just_writes(NVDimm);
	}
	else if(test == SAME_WRITE)
	{
	    MemLeak *temp_test = new MemLeak();
	    cycle = temp_test->same_write(NVDimm);
	}
	else if(test == WRITE_EVERYWHERE)
	{
	    MappedTest *temp_test = new MappedTest();
	    cycle = temp_test->write_everywhere(NVDimm);
	}
	else if(test == READ_EVERYWHERE)
	{
	    MappedTest *temp_test = new MappedTest();
	    cycle = temp_test->read_everywhere(NVDimm);
	}

	end= clock();
	cout<<"Simulation Results:\n";
	cout<<"Cycles simulated: "<<cycle<<endl;
	NVDimm->printStats();
	NVDimm->saveStats();
	cout<<"Execution time: "<<(end-start)<<" cycles. "<<(double)(end-start)/CLOCKS_PER_SEC<<" seconds.\n";
}
