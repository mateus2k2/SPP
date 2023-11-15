#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

/* 
g++ src/MeuC++/loadData.cpp -o src/MeuC++/loadData -Wall
./src/MeuC++/loadData
*/


int numberJobs, numberTools, capacityMagazine;	
std::vector<std::vector<bool>>  toolJob;
std::vector<std::vector<int>>  JobTools; 
unsigned int sum = 0;


void loadInstance(string filename){
	
	// Variaveis locais
	string line; 
    ifstream ifs;
	int i=0;
	int j=0;
	
	// Abre o arquivo com a instância
	ifs.open(filename);
			
	// Ler o arquivo
	if ( ifs.is_open()){
		getline(ifs,line);
		numberJobs = stoi(line); // Recupera a quantidade de tarefas

		getline(ifs,line);
		numberTools = stoi(line); // Recupera a quantidade de Ferramentas
		
		getline(ifs,line);
		capacityMagazine = stoi(line); // Recupera a capacidade do magazine
		
		// Redimensiona as variaveis 		
		toolJob.assign(numberTools, vector<bool>(numberJobs,false));
		JobTools.resize(numberJobs);
			
		// Preenche as variaveis	
		while(getline(ifs,line)){
			
			stringstream ss(line);
			while ( getline (ss ,line, ' ')){
				toolJob[i][j] = stoi(line);
				if(toolJob[i][j]){
					 JobTools[j].push_back(i);
					 ++sum;
				}
			j++;
			}
		j=0;
		i++;
		}
	// Fecha o arquivo	
	ifs.close();
	}else{
		std::cout << "Could not open the file! \n";
	}
}

int main(){
        
    loadInstance("/home/mateus/WSL/IC/dataOG/KTNS/Instances/Catanzaro/Tabela1/datA1");
    
    cout << "numberJobs: " << numberJobs << "\n";
    cout << "numberTools: " << numberTools << "\n";
    cout << "capacityMagazine: " << capacityMagazine << "\n";
    cout << "sum: " << sum << "\n";
    
    for(unsigned int i = 0; i < numberJobs; ++i){
        cout << "JobTools[" << i << "]: ";
        for (auto t = JobTools[i].begin(); t != JobTools[i].end(); ++t){
            cout << *t << " ";
        }
        cout << "\n";
    }
    
    for(unsigned int i = 0; i < numberTools; ++i){
        cout << "toolJob[" << i << "]: ";
        for (auto t = toolJob[i].begin(); t != toolJob[i].end(); ++t){
            cout << *t << " ";
        }
        cout << "\n";
    }
}