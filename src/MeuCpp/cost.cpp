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

using namespace std;

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// KTNS
// ------------------------------------------------------------------------------------------------------------------------------------------------------

unsigned int KTNSReport(vector<int> s){
    // Variaveis
	vector<bool> magazineL(numberTools, true);	
	unsigned int evalSol = 0; 
	int jL;

	int sumChanges = 0; // Conta quantas trocas de instancia foram feitas, quando pelo menos uma troca de ferramenta foi trocada do magazine
	int currantSwitch = 0; // Conta quantas trocas de ferramenta foram feitas, no job atual
	int currantProcessingTime = 0; 

	// int currantMinute = 0; // Conta quantas horas ja foram usadas no dia atual  		       
	// int currantDay = 0; // Conta quantos dias ja foram usados no horizonte de planejamento
	int inicioJob = 0; // Conta quantas horas ja foram usadas no dia atual  		       
	int fimJob = 0; // Conta quantos dias ja foram usados no horizonte de planejamento 

	vector<int> fineshedPriority; // Jobs prioritarios que foram feitos
	vector<int> unfinesedPriority; // Jobs prioritarios que foram feitos

	numberJobs = s.size();

	// Percorre todas as ferramentas
	for(jL= 0; jL < numberJobs; ++jL){
		
		currantSwitch = 0;

		vector<bool> magazineCL(numberTools);		
		int left = jL;
		int cmL = 0;

		while((cmL < capacityMagazine) && (left < numberJobs)){
			for (auto it=jobsType[s[left]].JobTools.begin(); ((it!=jobsType[s[left]].JobTools.end()) && (cmL < capacityMagazine)); ++it){
				if((magazineL[*it]) && (!magazineCL[*it])){
					magazineCL[*it] = true;
					++cmL;
				}else if((jL == left) && (!magazineCL[*it])){
					magazineCL[*it] = true;
					++cmL;
					++currantSwitch;
				}
			}
			++left;
		}
		
		for(int t=0; ((t < numberTools) && (cmL < capacityMagazine)); t++){
			if((magazineL[t]) && (!magazineCL[t])){
				magazineCL[t] = true;
				++cmL;			
			}
		}

		magazineL = magazineCL;

		if (jL == 0) currantSwitch = capacityMagazine;

		// ---------------------------------------------------------------------------

		currantProcessingTime = jobsType[s[jL]].processingTime;
		fimJob = inicioJob + currantProcessingTime;

		// verifica se a hora é sem supervisao e se houve troca de ferramenta
		if ((inicioJob % 1440 >= unsupervised) && (currantSwitch > 0)){
			
			//Verificar de consigo acabar essa tarefa antes de exceder o horizonte de planejamento
			if(fimJob + 1440 > planingHorizon * 1440){
				// Contar quantar tarefas prioritarias faltaram
				for(unsigned int v = jL; v < numberJobs; ++v){
					if(jobsType[s[v]].priority) unfinesedPriority.push_back(s[v]);
				}
				// Pode sair do loop 
				break;
			}
			else{
				inicioJob += 1440 - (inicioJob % 1440);
				fimJob = inicioJob + currantProcessingTime;

			}

		}

		//Tarefa vai vazar para o proximo dia
		if ((inicioJob % 1440) + currantProcessingTime >= 1440 ){
			
			//Verificar de consigo acabar essa tarefa antes de exceder o horizonte de planejamento
			if(fimJob + 1440 > planingHorizon * 1440){
				// Contar quantar tarefas prioritarias faltaram
				for(unsigned int v = jL; v < numberJobs; ++v){
					if(jobsType[s[v]].priority) unfinesedPriority.push_back(s[v]);
				}
				// Pode sair do loop 
				break;
			}
			else{
				inicioJob = fimJob;
				fimJob = 0;
			}
		}
		//Tarefa pode continuar no mesmo dia
		else{
			inicioJob = fimJob;
		}

		// ---------------------------------------------------------------------------

		cout << "Job Processado: " << s[jL] << " | " << "Processing Time: "  << currantProcessingTime  << " | " << "Priority: " << jobsType[s[jL]].priority << " | " << "Count Mudanças Magazine: " << currantSwitch << endl;
		printf("Minuto do Dia: %i/1440 | Hora: %d/24\n", inicioJob%1440, (inicioJob%1440)/60);
		printf("Dia Atual: %i/%i\n", inicioJob/1440, planingHorizon);
		
		cout << "\n";
		
		cout << "Magazine: ";
		for(unsigned int t = 0; t < magazineCL.size(); ++t){
			if(magazineCL[t]) 
				cout << t << " ";
		}

		cout << "\n\n";

		
		// ---------------------------------------------------------------------------

		evalSol += currantSwitch;
		if(currantSwitch > 0) ++sumChanges;
		if (jobsType[s[jL]].priority) fineshedPriority.push_back(s[jL]);

		// ---------------------------------------------------------------------------

	}

	int fineshedPriorityCount = fineshedPriority.size();
	int unfinesedPriorityCount = unfinesedPriority.size();

	cout << "DONE\n";

	printf("Fineshed Priority (%i) : ", fineshedPriorityCount);
	for(unsigned int t = 0; t < fineshedPriorityCount; ++t){
		cout << fineshedPriority[t] << " ";
	}
	cout << "\n";

	printf("Unfineshed Priority (%i) : ", unfinesedPriorityCount);
	for(unsigned int t = 0; t < unfinesedPriorityCount; ++t){
		cout << unfinesedPriority[t] << " ";
	}
	cout << "\n";

	cout << "Number of Switchs: " << evalSol << "\n";
	cout << "Number of Switchs Instances: " << sumChanges << "\n";

	int cost = (PROFITYPRIORITY * fineshedPriorityCount) - (COSTSWITCH * evalSol) - (COSTSWITCHINSTANCE * sumChanges) - (COSTPRIORITY * unfinesedPriorityCount);
	printf("(%i * %i) - (%i * %i) - (%i * %i) - (%i * %i) = %i\n", PROFITYPRIORITY, fineshedPriorityCount, COSTSWITCH, evalSol, COSTSWITCHINSTANCE, sumChanges, COSTPRIORITY, unfinesedPriorityCount, cost);

	return cost;
}

unsigned int KTNSReportIfDef(vector<int> s){
    // Variaveis
	vector<bool> magazineL(numberTools, true);	
	unsigned int evalSol = 0; 
	int jL;

	int sumChanges = 0; // Conta quantas trocas de instancia foram feitas, quando pelo menos uma troca de ferramenta foi trocada do magazine
	int currantSwitch = 0; // Conta quantas trocas de ferramenta foram feitas, no job atual
	int currantMinute = 0; // Conta quantas horas ja foram usadas no dia atual  		       
	int currantDay = 0; // Conta quantos dias ja foram usados no horizonte de planejamento 

	#ifdef DEBUG
	vector<int> fineshedPriority; // Jobs prioritarios que foram feitos
	vector<int> unfinesedPriority; // Jobs prioritarios que foram feitos
	#else
	int fineshedPriority = 0; // Jobs prioritarios que foram feitos
	int unfinesedPriority = 0; // Jobs prioritarios que foram feitos
	#endif

	numberJobs = s.size();

	// Percorre todas as ferramentas
	for(jL= 0; jL < numberJobs; ++jL){
		
		currantSwitch = 0;

		vector<bool> magazineCL(numberTools);		
		int left = jL;
		int cmL = 0;

		while((cmL < capacityMagazine) && (left < numberJobs)){
			for (auto it=jobsType[s[left]].JobTools.begin(); ((it!=jobsType[s[left]].JobTools.end()) && (cmL < capacityMagazine)); ++it){
				if((magazineL[*it]) && (!magazineCL[*it])){
					magazineCL[*it] = true;
					++cmL;
				}else if((jL == left) && (!magazineCL[*it])){
					magazineCL[*it] = true;
					++cmL;
					++currantSwitch;
				}
			}
			++left;
		}
		
		for(int t=0; ((t < numberTools) && (cmL < capacityMagazine)); t++){
			if((magazineL[t]) && (!magazineCL[t])){
				magazineCL[t] = true;
				++cmL;			
			}
		}

		magazineL = magazineCL;

		if (jL == 0) currantSwitch = capacityMagazine;

		// ---------------------------------------------------------------------------

		// verifica se a hora é sem supervisao e se houve troca de ferramenta
		if ((currantMinute >= unsupervised) && (currantSwitch > 0)){
			//Verificar de consigo acabar essa tarefa antes de exceder o horizonte de planejamento
			if(currantDay + 1 > planingHorizon){
				// Contar quantar tarefas prioritarias faltaram
				for(unsigned int v = jL; v < numberJobs; ++v){
					#ifdef DEBUG
					if(jobsType[s[v]].priority) unfinesedPriority.push_back(s[v]);
					#else
					if(jobsType[s[v]].priority) ++unfinesedPriority;
					#endif
				}
				// Pode sair do loop 
				break;
			}
			else{
				currantDay++;
				currantMinute = 0;
			}
		}

		//Tarefa vai vazar para o proximo dia
		if (currantMinute + jobsType[s[jL]].processingTime > 1440){
			//Verificar de consigo acabar essa tarefa antes de exceder o horizonte de planejamento
			if(currantDay + 1 > planingHorizon){
				// Contar quantar tarefas prioritarias faltaram
				for(unsigned int v = jL; v < numberJobs; ++v){
					#ifdef DEBUG
					if(jobsType[s[v]].priority) unfinesedPriority.push_back(s[v]);
					#else
					if(jobsType[s[v]].priority) ++unfinesedPriority;
					#endif
				}
				// Pode sair do loop 
				break;
			}
			else{
				currantDay++;
				// Acumular as horas que vazaram para o dia seguinte
				currantMinute += (currantMinute + jobsType[s[jL]].processingTime)-1440;
			}
		}
		//Tarefa pode continuar no mesmo dia
		else{
			currantMinute += jobsType[s[jL]].processingTime;
		}

		// ---------------------------------------------------------------------------

		#ifdef DEBUG
		cout << "Job Processado: " << s[jL] << " | " << "Processing Time: "  << jobsType[s[jL]].processingTime  << " | " << "Priority: " << jobsType[s[jL]].priority << " | " << "Count Mudanças Magazine: " << currantSwitch << endl;
		printf("Minuto do Dia: %i/1440 | Hora: %d/24\n", currantMinute, currantMinute/60);
		printf("Dia Atual: %i/%i\n", currantDay, planingHorizon);
		
		cout << "\n";
		
		cout << "Magazine: ";
		for(unsigned int t = 0; t < magazineCL.size(); ++t){
			if(magazineCL[t]) 
				cout << t << " ";
		}

		cout << "\n\n";
		#endif

		
		// ---------------------------------------------------------------------------

		evalSol += currantSwitch;
		if(currantSwitch > 0) ++sumChanges;
		#ifdef DEBUG
		if (jobsType[s[jL]].priority) fineshedPriority.push_back(s[jL]);
		#else
		if (jobsType[s[jL]].priority) ++fineshedPriority;
		#endif

		// ---------------------------------------------------------------------------

	}

	#ifdef DEBUG
	int fineshedPriorityCount = fineshedPriority.size();
	int unfinesedPriorityCount = unfinesedPriority.size();

	cout << "DONE\n";

	printf("Fineshed Priority (%i) : ", fineshedPriorityCount);
	for(unsigned int t = 0; t < fineshedPriorityCount; ++t){
		cout << fineshedPriority[t] << " ";
	}
	cout << "\n";

	printf("Unfineshed Priority (%i) : ", unfinesedPriorityCount);
	for(unsigned int t = 0; t < unfinesedPriorityCount; ++t){
		cout << unfinesedPriority[t] << " ";
	}
	cout << "\n";

	cout << "Number of Switchs: " << evalSol << "\n";
	cout << "Number of Switchs Instances: " << sumChanges << "\n";

	int cost = (PROFITYPRIORITY * fineshedPriorityCount) - (COSTSWITCH * evalSol) - (COSTSWITCHINSTANCE * sumChanges) - (COSTPRIORITY * unfinesedPriorityCount);
	printf("(%i * %i) - (%i * %i) - (%i * %i) - (%i * %i) = %i\n", PROFITYPRIORITY, fineshedPriorityCount, COSTSWITCH, evalSol, COSTSWITCHINSTANCE, sumChanges, COSTPRIORITY, unfinesedPriorityCount, cost);
	#else
	int cost = (PROFITYPRIORITY * fineshedPriority) - (COSTSWITCH * evalSol) - (COSTSWITCHINSTANCE * sumChanges) - (COSTPRIORITY * unfinesedPriority);
	#endif

	return cost;
}

unsigned int KTNS(vector<int> s){
	
	// Variaveis
	vector<bool> magazineL(numberTools, true);	
	unsigned int evalSol = 0; 
	int jL;

	int sumChanges = 0; // Conta quantas trocas de instancia foram feitas, quando pelo menos uma troca de ferramenta foi trocada do magazine
	int currantSwitch = 0; // Conta quantas trocas de ferramenta foram feitas, no job atual
	int currantMinute = 0; // Conta quantas horas ja foram usadas no dia atual  		       
	int currantDay = 0; // Conta quantos dias ja foram usados no horizonte de planejamento 

	int fineshedPriority = 0; // Jobs prioritarios que foram feitos
	int unfinesedPriority = 0; // Jobs prioritarios que foram feitos

	numberJobs = s.size();

	// Percorre todas as ferramentas
	for(jL= 0; jL < numberJobs; ++jL){
		
		currantSwitch = 0;

		vector<bool> magazineCL(numberTools);		
		int left = jL;
		int cmL = 0;

		while((cmL < capacityMagazine) && (left < numberJobs)){
			for (auto it=jobsType[s[left]].JobTools.begin(); ((it!=jobsType[s[left]].JobTools.end()) && (cmL < capacityMagazine)); ++it){
				if((magazineL[*it]) && (!magazineCL[*it])){
					magazineCL[*it] = true;
					++cmL;
				}else if((jL == left) && (!magazineCL[*it])){
					magazineCL[*it] = true;
					++cmL;
					++currantSwitch;
				}
			}
			++left;
		}
		
		for(int t=0; ((t < numberTools) && (cmL < capacityMagazine)); t++){
			if((magazineL[t]) && (!magazineCL[t])){
				magazineCL[t] = true;
				++cmL;			
			}
		}

		magazineL = magazineCL;

		if (jL == 0) currantSwitch = capacityMagazine;

		// ---------------------------------------------------------------------------

		// verifica se a hora é sem supervisao e se houve troca de ferramenta
		if ((currantMinute >= unsupervised) && (currantSwitch > 0)){
			//Verificar de consigo acabar essa tarefa antes de exceder o horizonte de planejamento
			if(currantDay + 1 > planingHorizon){
				// Contar quantar tarefas prioritarias faltaram
				for(unsigned int v = jL; v < numberJobs; ++v){
					unfinesedPriority += jobsType[s[v]].priority;
				}
				// Pode sair do loop 
				break;
			}
			else{
				currantDay++;
				currantMinute = 0;
			}
		}

		//Tarefa vai vazar para o proximo dia
		if (currantMinute + jobsType[s[jL]].processingTime > 1440){
			//Verificar de consigo acabar essa tarefa antes de exceder o horizonte de planejamento
			if(currantDay + 1 > planingHorizon){
				// Contar quantar tarefas prioritarias faltaram
				for(unsigned int v = jL; v < numberJobs; ++v){
					unfinesedPriority += jobsType[s[v]].priority;
				}
				// Pode sair do loop 
				break;
			}
			else{
				currantDay++;
				// Acumular as horas que vazaram para o dia seguinte
				currantMinute += (currantMinute + jobsType[s[jL]].processingTime)-1440;
			}
		}
		//Tarefa pode continuar no mesmo dia
		else{
			currantMinute += jobsType[s[jL]].processingTime;
		}

		// ---------------------------------------------------------------------------

		evalSol += currantSwitch;
		if(currantSwitch > 0) ++sumChanges;
		fineshedPriority += jobsType[s[jL]].priority;

		// ---------------------------------------------------------------------------

	}

	int cost = (PROFITYPRIORITY * fineshedPriority) - (COSTSWITCH * evalSol) - (COSTSWITCHINSTANCE * sumChanges) - (COSTPRIORITY * unfinesedPriority);

	return cost;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// COST
// ------------------------------------------------------------------------------------------------------------------------------------------------------

unsigned int printSolutionReport(vector<int> sol){
	
	vector<vector<int>> sols(numberMachines);
	int totalCost = 0;

	cout << "\n------------------------------------------------------------------------------------------\n";
	cout << "SOLUTION";
	cout << "\n------------------------------------------------------------------------------------------\n\n";

	cout << "Solution: " << endl;
    for (const auto &p : sol)
        cout << p << " ";

	cout << endl;

	cout << "Machine: " << endl;
    for (const auto &p : jobsType)
        cout << p.indexMachine << " ";

	cout << endl;

    for (int i = 0; i < sol.size(); ++i){
        // sols[machine[sol[i]]].push_back(sol[i]);
        sols[jobsType[sol[i]].indexMachine].push_back(sol[i]);
    }

    for (int i = 0; i < sols.size(); ++i) {
        cout << "\n------------------------------------------------------------------------------------------\n";
        cout << "Machine: " << i << " = ";
        for (int j = 0; j < sols[i].size(); ++j)
            cout << sols[i][j] << " ";
        cout << "\n------------------------------------------------------------------------------------------\n\n";

        totalCost += KTNSReport(sols[i]);
    }

	cout << "\n------------------------------------------------------------------------------------------\n";
	cout << "ALL DONE, TOTAL COST: " << totalCost;
	cout << "\n------------------------------------------------------------------------------------------\n";

	return totalCost;

}

unsigned int printSolutionReportIfDef(vector<int> sol){
	
	vector<vector<int>> sols(numberMachines);
	int totalCost = 0;

	#ifdef DEBUG
	cout << "\n------------------------------------------------------------------------------------------\n";
	cout << "SOLUTION";
	cout << "\n------------------------------------------------------------------------------------------\n\n";

	cout << "Solution: " << endl;
    for (const auto &p : sol)
        cout << p << " ";

	cout << endl;

	cout << "Machine: " << endl;
    for (const auto &p : jobsType)
        cout << p.indexMachine << " ";


	cout << endl;
	#endif

    for (int i = 0; i < sol.size(); ++i){
        sols[jobsType[sol[i]].indexMachine].push_back(sol[i]);
        // sols[machine[sol[i]]].push_back(sol[i]);
    }

    for (int i = 0; i < sols.size(); ++i) {
		#ifdef DEBUG
        cout << "\n------------------------------------------------------------------------------------------\n";
        cout << "Machine: " << i << " = ";
        for (int j = 0; j < sols[i].size(); ++j)
            cout << sols[i][j] << " ";
        cout << "\n------------------------------------------------------------------------------------------\n\n";
		#endif

        totalCost += KTNSReportIfDef(sols[i]);
    }

	#ifdef DEBUG
	cout << "\n------------------------------------------------------------------------------------------\n";
	cout << "ALL DONE, TOTAL COST: " << totalCost;
	cout << "\n------------------------------------------------------------------------------------------\n";
	#endif

	return totalCost;

}

unsigned int cost(vector<int> sol){
	
	vector<vector<int>> sols(numberMachines);
	int totalCost = 0;

    for (int i = 0; i < sol.size(); ++i){
        sols[jobsType[sol[i]].indexMachine].push_back(sol[i]);
        // sols[machine[sol[i]]].push_back(sol[i]);
    }

    for (int i = 0; i < sols.size(); ++i) {
        totalCost += KTNS(sols[i]);
    }

	return totalCost;

}