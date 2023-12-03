#include <vector>
#include <iostream>
#include <atomic>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> 

#include "headers/GlobalVars.h"
#include "headers/loadData.h"
#include "headers/cost.h"
#include "headers/super.h"

using namespace std;

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// VARIVEIS PARA CARREGAR DADOS
// ------------------------------------------------------------------------------------------------------------------------------------------------------

map<int, vector<int>>  mapToolSets;
vector<int> JobToolsIndex;
vector<vector<int>> JobTools;

vector<int> operation;		 	
vector<int> job;			  
vector<int> processingTime;	 
vector<int> machine;		 
vector<int> priority;		 

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// VARIVEIS RESOLVER O PROBLEMA
// ------------------------------------------------------------------------------------------------------------------------------------------------------

int planingHorizon;  		 
int unsupervised; 			 
int numberMachines;		   
int numberTools;      	    
int numberJobs; 	   	   
int capacityMagazine;     

vector<Job> jobsType;
vector<Job> jobsTypeDeletados;
vector<ToolSet> ToolSetsType;
vector<ToolSet> ToolSetsTypeDeletados;

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// MAIN
// ------------------------------------------------------------------------------------------------------------------------------------------------------

int main(){ 

    // ---------------------------------------------------------------------

	// laodToolSet("/home/mateus/WSL/IC/data/Exemplo/ToolSets.csv");
	// laodRealInstance("/home/mateus/WSL/IC/data/Exemplo/Jobs.csv");

    // vector<int> sol = {2,5,6,3,4,7,8,0,1,9};
    // machine         = {1,1,0,0,0,0,0,1,1,1};
    // priority        = {1,1,0,0,0,1,1,0,0,1};
    // numberMachines  = 2;

    // loadDataTypes();

    // for (int i = 0; i < numberJobs; ++i) {
    //     jobsType[i].indexMachine = machine[i];
    // }

    // ---------------------------------------------------------------------

	laodToolSet("/home/mateus/WSL/IC/data/Exemplo/ToolSets2.csv");
	laodRealInstance("/home/mateus/WSL/IC/data/Exemplo/Jobs2.csv");

    vector<int> sol = {2,5,6,3,4,7,8,0,1,8};
    machine         = {1,1,0,0,0,0,0,1,1,1,1};
    priority        = {1,1,0,0,0,1,1,0,0,1,1};
    numberMachines  = 2;

    loadDataTypes();

    for (int i = 0; i < numberJobs; ++i) {
        jobsType[i].indexMachine = machine[i];
    }

    printDataReport();
    removeSubSets();
    printDataReport();

    // ---------------------------------------------------------------------

    // laodToolSet("/home/mateus/WSL/IC/data/1000ToolSet.csv");
	// laodRealInstance("/home/mateus/WSL/IC/data/1000.csv");

    // priority.resize(numberJobs);
    // for (int i = 0; i < numberJobs; ++i) {
    //     priority[i] = 1;
    // }
    // machine.resize(numberJobs);
    // for (int i = 0; i < numberJobs; ++i) {
    //     machine[i] = 0;
    // }
    // numberMachines = 1;
    
    // loadDataTypes();

    // priority.resize(numberJobs);
    // for (int i = 0; i < numberJobs; ++i) {
    //     jobsType[i].priority = 1;
    // }
    // machine.resize(numberJobs);
    // for (int i = 0; i < numberJobs; ++i) {
    //     jobsType[i].indexMachine = 0;
    // }
    // numberMachines = 1;

    // removeSubSets();

    // vector<int> sol(numberJobs);
    // for (int i = 0; i < numberJobs; ++i) {
    //     sol[i] = i;
    // }

    // ---------------------------------------------------------------------

    printDataReport();
    printSolutionReport(sol);

	return 0;
}