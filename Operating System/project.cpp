#include <string>
#include <stdio.h>      
#include <stdlib.h>    
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

#define NUM_CPU 1 //in case there would be multiple CPUs


//*****************************************************
//The process class that represents a process in this simulator
class process
{
public:
	//variables
	char processId;
	int initialArrival;
	int burstTime;
	int numBursts;
	int ioTime;

	//Time variables
	int IOtimeLeft;
	int CPUtime;

	//Special time variables for RR
	int RRtimeLeft;

	//Tracking elements
	bool isLate;
	int numBurstsExecuted;
	int contextWait;

	//Calculation variables
	int RQwaitTime;
	int totalTime;


	//Constructors
	process(char id, int arrival, int burst, int num, int io)
	{
		//Initialzie a process with given variables
		processId = id;
		initialArrival = arrival;
		burstTime = burst;
		numBursts = num;
		ioTime = io;
		isLate = false;
		numBurstsExecuted = 0;
		IOtimeLeft = io;
		RQwaitTime = 0;
		totalTime = 0;
		CPUtime = burst;
		//contextWait = 0;
		RRtimeLeft = -1;


	}

	process()
	{
		//Initialzie a process without variables
		processId = '_';
		initialArrival = 0;
		burstTime = 0;
		numBursts = 0;
		ioTime = 0;
		isLate = false;
		numBurstsExecuted = 0;
		IOtimeLeft = ioTime;
		RQwaitTime = 0;
		totalTime = 0;
		CPUtime = burstTime;
		RRtimeLeft = -1;
		contextWait = 0;


	}

	//Accessors
	char getProcessId(){return processId;}
	int getInitialArrival(){return initialArrival;}
	int getBurstTime(){return burstTime;}
	int getNumBursts(){return numBursts;}
	int getIoTime(){return ioTime;}
	void doneBurst()
	{
		numBursts--;
		numBurstsExecuted++;
	}
	void completeBurst()
	{
		numBursts--;
		numBurstsExecuted++;
	}
	void IOreset()
	{
		IOtimeLeft = ioTime;
	}
	void CPUtimePass()
	{
		//when process is on CPU and time passes
		if(CPUtime>0)
		{
			CPUtime--;
			totalTime++;
		}
	}
	void IOtimePass()
	{
		//when process is on IO and time passes
		IOtimeLeft--;
	}
	void RQtimePass()
	{
		//when process is on Ready Queue(RQ) and time passes
		RQwaitTime++;
		totalTime++;
	}



};


//End of process class
//*****************************************************************************
//The cpu simulator class in this project

class CPU_sim
{
	//variables
	public :

		//The current process in the CPU
		process currentProcess;


		//if there is multiple process
		bool isCPUready;
		int startTime;
		int timeLeft;
		//time before the cpu can start working
		int waitTime;

		//ACCESSORIES AND modifiers
		void setCurrentProcess(process p)
		{
			currentProcess = p;
		}
		bool getCPUstate()
		{
			return isCPUready;
		}
		void setCPUstate(bool state)
		{
			isCPUready = state;
		}
		void timePass()
		{
			currentProcess.CPUtimePass();
			timeLeft--;
		}
		void setTime(int time)
		{
			timeLeft = time;
		}
		int getTimeLeft()
		{
			return timeLeft;
		}

};


//end of CPU class
//***************************************************************

//Helper class

//calulate the output variabls
double compute_average_CPUburst(vector<process> &q)
{
	//The sum of total cpu bursts
	double sum = 0.0;
	//The number of total cpu bursts
	double count = 0.0;
	//calculate sum and count
	//Iterate through the process vector
	for (unsigned int i=0;i<q.size();i++)
	{
		sum+= (double)(q[i].burstTime * q[i].numBursts);
		count += (double)q[i].numBursts;
	}

	return sum/count;

}


//calculate average wait time
double compute_average_waitTime(vector<process> &q)
{
	//The sum of total wait time
	double sum = 0.0;
	//The number of total wait time
	double count = 0.0;
	//calculate sum and count
	//Iterate through the process vector
	for (unsigned int i=0;i<q.size();i++)
	{
		sum+= (double)q[i].RQwaitTime;
		count += (double)q[i].numBursts;
	}

	return sum/count;
}


//calculate average turn around time
double compute_average_turnAroundTime(vector<process> &q)
{
	//The sum of total turn around time
	double sum = 0.0;
	//The number of total turn around time
	double count = 0.0;
	//calculate sum and count
	//Iterate through the process vector
	for (unsigned int i=0;i<q.size();i++)
	{
		sum+= (double)q[i].totalTime;
		count += (double)q[i].numBursts;
	}

	return sum/count;
}


//helper functions for print
//a X style function to print the ready queue
void printQueue(vector<process>* readyQueue)
{
	printf("[Q");
	if (readyQueue->size() == 0)
	{
		printf(" empty]\n");
	}
	else
	{
		for (unsigned int i =0;i<readyQueue->size();i++)
		{
			printf(" %c",(*readyQueue)[i].getProcessId() );
		}
		printf("]\n");
	}
}



//******************************************************************
//Start of sorting methods




//*******************************************************************
//FCFS
void FCFS_place_process(vector<process> *p, process currentProcess)
{
    if((p->size() == 0) || (currentProcess.getInitialArrival() > (*p)[p->size()-1].getInitialArrival()))
	{
		p->push_back(currentProcess);
	}
	else
	{
		for(vector<process>::iterator itr = p->begin();itr!=p->end();itr++)
		{
			if(currentProcess.getInitialArrival()<itr->getInitialArrival())
			{
				p->insert(itr,currentProcess);
				break;
			}
		}
	}
}

void FCFS(vector<process> *p, unsigned int numProcess, FILE * outFile)
{
	// p stores all the processes to be sorted
	//Define the three queues of three different states: ready,wait,done
	vector<process> readyQueue;
	vector<process> waitQueue;
	vector<process> doneQueue;

	//define the current process the method is working on
	process currentProcess;
	//count how many context switches in this process
	int contextSwitches = 0;
	//Current time is the time counter 
	int currentTime = 0;
	int contextSwitchTime = 8;
	//initialize the CPU vector in case there are multiple CPUs
	vector<CPU_sim> CPU(NUM_CPU);

	//
	for(int i =0;i<NUM_CPU;i++)
	{
		CPU[i].setTime(contextSwitchTime/2);
	}
	//Print the header
	printf("time %dms: Simulator started for FCFS", currentTime);
	printQueue(&readyQueue);
	//Current time is added every ms
	for (currentTime = 0; doneQueue.size()!=p->size();currentTime++)
	{
		//store each process into ready queue
		for(unsigned int i=0;i<numProcess;i++)
		{
			currentProcess = (*p)[i];
			if(currentProcess.getInitialArrival() == currentTime)
			{
				FCFS_place_process(&readyQueue,currentProcess);
				printf("time %dms: Process %c arrived", currentTime, currentProcess.getProcessId());
				printQueue(&readyQueue);
			}

		}
		//record time passing by ms
		for(unsigned int i=0;i<readyQueue.size();i++)
		{
			readyQueue[i].RQtimePass();
		}

		//Main body of FCFS
		for(unsigned int i =0;i<NUM_CPU;i++)
		{
			if(CPU[i].getTimeLeft() == 0 && CPU[i].currentProcess.getProcessId()!='_')
			{
				//if a burst has finished, remove it from CPU and make a context switch
				CPU[i].currentProcess.doneBurst();
				//after a burst is done, check for the rest
				if(CPU[i].currentProcess.getNumBursts()>0)
				{
					//print out process information
					printf("time %dms: Process %c completed a CPU burst; %d to go", currentTime, CPU[i].currentProcess.getProcessId(), CPU[i].currentProcess.getNumBursts());
					printQueue(&readyQueue);
					printf("time %dms: Process %c blocked on I/O until time %dms", currentTime, CPU[i].currentProcess.getProcessId(), currentTime + CPU[i].currentProcess.getIoTime());
					printQueue(&readyQueue);
					waitQueue.push_back(CPU[i].currentProcess);
				}
				else
				{
					//If the current process has no other bursts
					printf("time %dms: Process %c terminated", currentTime, CPU[i].currentProcess.getProcessId());
					printQueue(&readyQueue);
					doneQueue.push_back(CPU[i].currentProcess);
				}
				//then clean the current process and count the contxt switch
				process empty;
				CPU[i].setCurrentProcess(empty);
				CPU[i].setTime(contextSwitchTime/2);//as what we did before, reset the context switch time
				contextSwitches++;
			}
			if(CPU[i].currentProcess.getProcessId()=='_'&&readyQueue.size() ==0)
			{
				//take the first process in ready queue
				CPU[i].setTime(contextSwitchTime/2);
				readyQueue.erase(readyQueue.begin());
				printf("time %dms: Process %c started using the CPU", currentTime, CPU[i].currentProcess.getProcessId());
				printQueue(&readyQueue);
				CPU[i].setTime(CPU[i].currentProcess.getBurstTime());
			}
			CPU[i].timePass();
		}

		for(unsigned int i =0;i<waitQueue.size();i++)
		{
			if(waitQueue[i].getProcessId()!='_'&&waitQueue[i].IOtimeLeft == 0)
			{
				//check if the process is valid,and ready to get into running
				waitQueue[i].IOreset();
				FCFS_place_process(&readyQueue,waitQueue[i]);
				printf("time %dms: Process %c completed I/O", currentTime, waitQueue[i].getProcessId());
				printQueue(&readyQueue);
				waitQueue.erase(waitQueue.begin()+i);
				i--;//adpat to the removal
			}
			else
			{
				waitQueue[i].IOtimePass();
			}
		}

	}
	//currentTime+=3;
	for(unsigned int i =0;i<doneQueue.size();i++)
	{
		doneQueue[i].numBursts = doneQueue[i].numBurstsExecuted;
	}

	//outfile print
	printf("time %dms: Simulator ended for FCFS\n\n", currentTime);
	fprintf(outFile, "Algorithm FCFS\n");
	fprintf(outFile, "-- average CPU burst time: %.2f ms\n", compute_average_CPUburst(*p));
	fprintf(outFile, "-- average wait time: %.2f ms\n", compute_average_waitTime(doneQueue));
	fprintf(outFile, "-- average turnaround time: %.2f ms\n", compute_average_turnAroundTime(doneQueue));
	fprintf(outFile, "-- total number of context switches: %d\n", contextSwitches);
	fprintf(outFile, "-- total number of preemptions: %d\n", 0);
}

//*******************************************************************
//SJF

void SJF_place_process(vector<process> *p, process currentProcess)
{
	//Place a process into the process vector aat its correct entry
	//There are two cases where push back can be directly used
	//first, it is the only process
	//second, it is the largest
	if((p->size() == 0) || (currentProcess.getBurstTime() > (*p)[p->size()-1].getBurstTime()))
	{
		p->push_back(currentProcess);
	}
	else
	{
		//Else, iterate through the vector and find the proper place to insert
		
		for(vector<process>::iterator itr = p->begin();itr!=p->end();itr++)
		{
			if(currentProcess.getBurstTime()<itr->getBurstTime())
			{
				p->insert(itr,currentProcess);
				break;
			}
		}
	}
}



void SJf(vector<process> *p, unsigned int numProcess, FILE * outFile)
{
	// p stores all the processes to be sorted
	//Define the three queues of three different states: ready,wait,done
	vector<process> readyQueue;
	vector<process> waitQueue;
	vector<process> doneQueue;

	//define the current process the method is working on
	process currentProcess;
	//count how many context switches in this process
	int contextSwitches = 0;
	//Current time is the time counter 
	int currentTime = 0;
	int contextSwitchTime = 8;
	//initialize the CPU vector in case there are multiple CPUs
	vector<CPU_sim> CPU(NUM_CPU);

	//
	for(int i =0;i<NUM_CPU;i++)
	{
		CPU[i].setTime(contextSwitchTime/2);
	}
	//Print the header
	printf("time %dms: Simulator started for SJF", currentTime);
	printQueue(&readyQueue);
	//Current time is added every ms
	for (currentTime = 0; doneQueue.size()!=p->size();currentTime++)
	{
		//store each process into ready queue
		for(unsigned int i=0;i<numProcess;i++)
		{
			currentProcess = (*p)[i];
			if(currentProcess.getInitialArrival() == currentTime)
			{
				SJF_place_process(&readyQueue,currentProcess);
				printf("time %dms: Process %c arrived", currentTime, currentProcess.getProcessId());
				printQueue(&readyQueue);
			}

		}
		//record time passing by ms
		for(unsigned int i=0;i<readyQueue.size();i++)
		{
			readyQueue[i].RQtimePass();
		}

		//Main body of SJF
		for(unsigned int i =0;i<NUM_CPU;i++)
		{
			if(CPU[i].getTimeLeft() == 0 && CPU[i].currentProcess.getProcessId()!='_')
			{
				//if a burst has finished, remove it from CPU and make a context switch
				CPU[i].currentProcess.doneBurst();
				//after a burst is done, check for the rest
				if(CPU[i].currentProcess.getNumBursts()>0)
				{
					//print out process information
					printf("time %dms: Process %c completed a CPU burst; %d to go", currentTime, CPU[i].currentProcess.getProcessId(), CPU[i].currentProcess.getNumBursts());
					printQueue(&readyQueue);
					printf("time %dms: Process %c blocked on I/O until time %dms", currentTime, CPU[i].currentProcess.getProcessId(), currentTime + CPU[i].currentProcess.getIoTime());
					printQueue(&readyQueue);
					waitQueue.push_back(CPU[i].currentProcess);
				}
				else
				{
					//If the current process has no other bursts
					printf("time %dms: Process %c terminated", currentTime, CPU[i].currentProcess.getProcessId());
					printQueue(&readyQueue);
					doneQueue.push_back(CPU[i].currentProcess);
				}
				//then clean the current process and count the contxt switch
				process empty;
				CPU[i].setCurrentProcess(empty);
				CPU[i].setTime(contextSwitchTime/2);//as what we did before, reset the context switch time
				contextSwitches++;
			}
			if(CPU[i].currentProcess.getProcessId()=='_'&&readyQueue.size() ==0)
			{
				//take the first process in ready queue
				CPU[i].setTime(contextSwitchTime/2);
				readyQueue.erase(readyQueue.begin());
				printf("time %dms: Process %c started using the CPU", currentTime, CPU[i].currentProcess.getProcessId());
				printQueue(&readyQueue);
				CPU[i].setTime(CPU[i].currentProcess.getBurstTime());
			}
			CPU[i].timePass();
		}

		for(unsigned int i =0;i<waitQueue.size();i++)
		{
			if(waitQueue[i].getProcessId()!='_'&&waitQueue[i].IOtimeLeft == 0)
			{
				//check if the process is valid,and ready to get into running
				waitQueue[i].IOreset();
				SJF_place_process(&readyQueue,waitQueue[i]);
				printf("time %dms: Process %c completed I/O", currentTime, waitQueue[i].getProcessId());
				printQueue(&readyQueue);
				waitQueue.erase(waitQueue.begin()+i);
				i--;//adpat to the removal
			}
			else
			{
				waitQueue[i].IOtimePass();
			}
		}

	}
	//currentTime+=3;

	for(unsigned int i =0;i<doneQueue.size();i++)
	{
		doneQueue[i].numBursts = doneQueue[i].numBurstsExecuted;
	}

	//outfile print

	printf("time %dms: Simulator ended for SJF\n\n", currentTime);
	fprintf(outFile, "Algorithm SJF\n");
	fprintf(outFile, "-- average CPU burst time: %.2f ms\n", compute_average_CPUburst(*p));
	fprintf(outFile, "-- average wait time: %.2f ms\n", compute_average_waitTime(doneQueue));
	fprintf(outFile, "-- average turnaround time: %.2f ms\n", compute_average_turnAroundTime(doneQueue));
	fprintf(outFile, "-- total number of context switches: %d\n", contextSwitches);
	fprintf(outFile, "-- total number of preemptions: %d\n", 0);

}
//******************************************************************
//Start of RR
//it's basically the same as FCFS
void RR_place_process(vector<process> *p, process currentProcess)
{
    if((p->size() == 0) || (currentProcess.getInitialArrival() > (*p)[p->size()-1].getInitialArrival()))
	{
		p->push_back(currentProcess);
	}
	else
	{
		for(vector<process>::iterator itr = p->begin();itr!=p->end();itr++)
		{
			if(currentProcess.getInitialArrival()<itr->getInitialArrival())
			{
				p->insert(itr,currentProcess);
				break;
			}
		}
	}
}
//The main body of RR
void RR(vector<process> *p, unsigned int numProcess, FILE * outFile)
{

	// p stores all the processes to be sorted
	//Define the three queues of three different states: ready,wait,done
	vector<process> readyQueue;
	vector<process> waitQueue;
	vector<process> doneQueue;

	//define the current process the method is working on
	process currentProcess;
	//count how many context switches in this process
	int contextSwitches = 0;
	//Current time is the time counter 
	int currentTime = 0;
	int contextSwitchTime = 8;
	//initialize the CPU vector in case there are multiple CPUs
	//In RR we need to track the preemptions
	int preEmptions = 0;
	printf("time %dms: Simulator started for RR", currentTime);
	printQueue(&readyQueue);
	//NUM_CPU == 1 in this case
	vector<CPU_sim> CPU(NUM_CPU);

	//processess start arriving CPU at a certain wait time
	for(int i =0;i<NUM_CPU;i++)
	{
		CPU[i].setTime(contextSwitchTime/2);
	}

	//Unlike in other methods where we push back to wait Queue in main body, in RR
	//We implement wait queue first

	for (currentTime = 0; doneQueue.size()!=p->size();currentTime++)
	{
		//Check wait queue
		for(unsigned int i =0;i<waitQueue.size();i++)
		{
			if(waitQueue[i].getProcessId()!='_'&&waitQueue[i].IOtimeLeft == 0)
			{
				//check if the process is valid,and ready to get into running
				waitQueue[i].IOreset();
				RR_place_process(&readyQueue,waitQueue[i]);
				printf("time %dms: Process %c completed I/O", currentTime, waitQueue[i].getProcessId());
				printQueue(&readyQueue);
				waitQueue.erase(waitQueue.begin()+i);
				i--;//adpat to the removal
			}
			else
			{
				waitQueue[i].IOtimePass();
			}
		}
		//store each process into ready queue
		for(unsigned int i=0;i<numProcess;i++)
		{
			currentProcess = (*p)[i];
			if(currentProcess.getInitialArrival() == currentTime)
			{
				RR_place_process(&readyQueue,currentProcess);
				printf("time %dms: Process %c arrived", currentTime, currentProcess.getProcessId());
				printQueue(&readyQueue);
			}

		}
		//record time passing by ms
		for(unsigned int i=0;i<readyQueue.size();i++)
		{
			readyQueue[i].RQtimePass();
		}

		//Main body of SJF
		for(unsigned int i =0;i<NUM_CPU;i++)
		{
			if(CPU[i].getTimeLeft() == 0 && CPU[i].currentProcess.getProcessId()!='_')
			{
				//if a burst has finished, remove it from CPU and make a context switch
				CPU[i].currentProcess.doneBurst();
				//after a burst is done, check for the rest
				if(CPU[i].currentProcess.getNumBursts()>0)
				{
					//print out process information
					printf("time %dms: Process %c completed a CPU burst; %d to go", currentTime, CPU[i].currentProcess.getProcessId(), CPU[i].currentProcess.getNumBursts());
					printQueue(&readyQueue);
					printf("time %dms: Process %c blocked on I/O until time %dms", currentTime, CPU[i].currentProcess.getProcessId(), currentTime + CPU[i].currentProcess.getIoTime());
					printQueue(&readyQueue);
					waitQueue.push_back(CPU[i].currentProcess);
				}
				else
				{
					//If the current process has no other bursts
					printf("time %dms: Process %c terminated", currentTime, CPU[i].currentProcess.getProcessId());
					printQueue(&readyQueue);
					doneQueue.push_back(CPU[i].currentProcess);
				}
				//then clean the current process and count the contxt switch
				process empty;
				CPU[i].setCurrentProcess(empty);
				CPU[i].setTime(contextSwitchTime/2);//as what we did before, reset the context switch time
				contextSwitches++;
			}
			if(CPU[i].currentProcess.getProcessId()=='_'&&readyQueue.size() ==0)
			{
				//take the first process in ready queue
				CPU[i].setTime(contextSwitchTime/2);
				readyQueue.erase(readyQueue.begin());
				printf("time %dms: Process %c started using the CPU", currentTime, CPU[i].currentProcess.getProcessId());
				printQueue(&readyQueue);
				CPU[i].setTime(CPU[i].currentProcess.getBurstTime());
			}
			CPU[i].timePass();
		}
		//Count pre emptiions 
		for(unsigned int i =0;i<NUM_CPU;i++)
		{
			//If finished a process and ready queue is empty
			if(CPU[i].currentProcess.RRtimeLeft!=0&&readyQueue.empty())
			{
				printf("time %dms: Time slice expired; no preemption because ready queue is empty\n", currentTime);
				printQueue(&readyQueue);

			}
			else if(CPU[i].currentProcess.RRtimeLeft!=0&&!readyQueue.empty())
			{
				//finished a process but theres some in ready queue
				process temp;
				readyQueue.push_back(CPU[i].currentProcess);
				CPU[i].currentProcess = temp;
				printf("time %dms: Time slice expired; process %c preempted with %dms to go\n", currentTime,readyQueue.back().getProcessId(),readyQueue.back().RRtimeLeft);
				printQueue(&readyQueue);
				preEmptions++;
				CPU[i].waitTime+=8;
				contextSwitches++;
				readyQueue[i].RQwaitTime-=8;

			}

            for(unsigned int i =0;i<waitQueue.size();i++)
            {
                if(waitQueue[i].getProcessId()!='_'&&waitQueue[i].IOtimeLeft == 0)
                {
                    //check if the process is valid,and ready to get into running
                    waitQueue[i].IOreset();
                    RR_place_process(&readyQueue,waitQueue[i]);
                    printf("time %dms: Process %c completed I/O", currentTime, waitQueue[i].getProcessId());
                    printQueue(&readyQueue);
                    waitQueue.erase(waitQueue.begin()+i);
                    i--;//adpat to the removal
                }
                else
                {
                    waitQueue[i].IOtimePass();
                }
            }

		

	    }

        for(unsigned int i =0;i<doneQueue.size();i++)
        {
            doneQueue[i].numBursts = doneQueue[i].numBurstsExecuted;
        }

        //outfile print

        printf("time %dms: Simulator ended for RR\n\n", currentTime);
        fprintf(outFile, "Algorithm RR\n");
        fprintf(outFile, "-- average CPU burst time: %.2f ms\n", compute_average_CPUburst(*p));
        fprintf(outFile, "-- average wait time: %.2f ms\n", compute_average_waitTime(doneQueue));
        fprintf(outFile, "-- average turnaround time: %.2f ms\n", compute_average_turnAroundTime(doneQueue));
        fprintf(outFile, "-- total number of context switches: %d\n", contextSwitches);
        fprintf(outFile, "-- total number of preemptions: %d\n", 0);

    }
}


int main(int argc, char* argv[]){

    // Collecting parameters
    string::size_type sz;
    long num_process = stol(argv[1],&sz);
    double seed = stod(argv[2],&sz);
    srand48(seed);
    int lambda = atoi(argv[3]);
    int upper_bound = atoi(argv[4]);
    int t_cs = atoi(argv[5]);
	if (t_cs==1){}
    double a = stod(argv[6], &sz);
	if (a==0){}
    int t_slice = atoi(argv[7]);
	if (t_slice==1){}
    if (argc == 9) {
      bool isEnd = (argv[8] == string("END"));
	  if (isEnd){}
    } else {
      bool isEnd = true;
	  if (isEnd){}
    }

    // Declare containers
    vector<int> arrival(num_process, -1); // arrival time
    vector<int> nburst(num_process, -1); // number of bursts
    vector<vector<int> > cpuburst(num_process); // actual time for cpu bursts
    vector<vector<int> > ioburst(num_process); // actual time for io bursts
    vector<string> IDs;
    IDs = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

    // initialize data
    int random_num;
    for (int i=0;i<num_process;i++){
        while (true){
            random_num = floor(-(log(drand48())) / lambda);
            if (random_num<=upper_bound) {break;}
        }
        arrival[i]=random_num;
        nburst[i] = floor(drand48() * 100) + 1;
        cpuburst[i].resize(nburst[i]);
        ioburst[i].resize(nburst[i]);
        for (int j=0;j<nburst[i]-1;j++){
            while (true){
                random_num = floor(-(log(drand48())) / lambda);
                if (random_num<=upper_bound) {break;}
            }
            cpuburst[i][j]=random_num;
            while (true){
                random_num = floor(-(log(drand48())) / lambda);
                if (random_num<=upper_bound) {break;}
            }
            ioburst[i][j]=random_num;
        }
        while (true){
            random_num = floor(-(log(drand48())) / lambda);
            if (random_num<=upper_bound) {break;}
        }
        cpuburst[i][nburst[i] - 1] = random_num;
        ioburst[i][nburst[i] - 1] = 0;
    }

    // start intput data
    ofstream outfile;
    //outfile.open("simout.txt");
    //vector<process> processes;

    

    //FCFS(&processes,num_process,outfile);
    //SJF(&processes,num_process,outfile);
    //SRT();
    //RR(&processes,num_process,outfile);
    return 0;
}