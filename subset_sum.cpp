#include<iostream>
#include<vector>
#include<chrono>
#include<numeric>
#include<algorithm>
#include<fstream>
#include<math.h>
#include<string>

using namespace std;

// Benchmark instances
typedef struct inp {
	vector<int>arr;
	int n;
	int sum;
}inp; 

// Number of Instances needed to be tested
int totalInstances = 10;

// Timer variables
// Timer for running each benchmark instance
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::duration;
using chrono::microseconds;

auto t1 = high_resolution_clock::now();
auto t2 = high_resolution_clock::now();
auto t_ms = duration_cast<microseconds>(t2 - t1);

// Time Limit for each instance
int timeLimit = 60;

// Generate the random inputs required for each instance
vector<inp> generateInput(vector<inp>inputBenchmark) {

	int length = 30;
	for(int i=0;i<totalInstances;i++){
		
		for(int j=0;j<length;j++){
			inputBenchmark[i].arr.push_back(rand()%100 + 1);
		}
		inputBenchmark[i].n = length;
		auto rand_sum = accumulate(inputBenchmark[i].arr.begin(), inputBenchmark[i].arr.end(), 0);
		inputBenchmark[i].sum = rand() % rand_sum + 1;

		length+=2;
	}	

	return inputBenchmark;

}

// Check if a susbet sum exists
bool isSubsetSum(vector<int> arr, int n, int sum)
{
    if (sum == 0)
        return true;

    if (n == 0)
        return false;

			t2 = high_resolution_clock::now();

    	/* Getting number of milliseconds as an integer. */
    	t_ms = duration_cast<microseconds>(t2 - t1);
		
			// If the benchmark instance took more than 60s then time limit exceeded
			if(t_ms.count() > timeLimit){
				return false;
			}

    if (arr[n - 1] > sum)
        return isSubsetSum(arr, n - 1, sum);

    return isSubsetSum(arr, n - 1, sum) 
						|| isSubsetSum(arr, n - 1, sum - arr[n - 1]);
}

bool comp (int i,int j) { return (i>j); }


bool subsetGreedy(vector<int> arr, int n, int sum, vector<int> &setGreedy){


	sort(arr.begin(), arr.end(), comp);	

	int s=0;

	for(int i=0;i<n;i++){
		if(s+arr[i]<=sum){
			s+=arr[i];
			setGreedy.push_back(arr[i]);	
		}	
	}

	/*for(int i=0;i<n;i++)
		cout<<arr[i]<<" ";
	cout<<endl;	
	cout<<"Output for greedy: "<<s<<" ,Actual sum: "<<sum<<" with difference: "<<sum-s<<"\n\n";
	cout<<"Set for Greedy = "<<endl;
	for(int i=0;i<setGreedy.size();i++)
		cout<<setGreedy[i]<<" ";
	cout<<endl<<endl;
	*/

	if(sum == s)
		return true;
	else
		return false;

}


void writeFile(vector<int> arr, int i, int sumT){

	std::ofstream sum;
/*
data;

param:
I: Value :=
0    5
1    10
2    15
3    20
4    25
5    30
6    35
7    40
8    45;

param TargetSum := 32;*/

	sum.open("AMPL/data/sum" + std::to_string(i) + ".dat");
	
	sum<<"data;"<<endl<<endl;
	sum<<"param:"<<endl;
	sum<<"I: Value :="<<endl;



	for(int j=0;j<arr.size();j++){
		if(j==arr.size()-1)
			sum << j << "    "<<arr[j] << ";"<<endl<<endl;
		else
			sum << j << "    "<<arr[j] << endl;
	}	
		
	sum<<"param TargetSum := "<<sumT<<";"<<endl;

	sum.close();

	
}


void printVector(vector<int> arr){
	for(int i=0;i<arr.size();i++)
		cout<<arr[i]<<" ";
	cout<<endl;
}

vector<int> getNeighbor(vector<int> A, vector<int> set){
	//srand(time(NULL));

	vector<int> B = A;	
	int i = rand() % set.size();
	int j = rand() % set.size();
	
	auto xi = find(A.begin(), A.end(), set[i]);
	if(xi != A.end()){
		auto bxi = find(B.begin(), B.end(), set[i]);
		if(bxi != B.end()){
			int pos = bxi - B.begin(); 
			B.erase(B.begin() + pos);
		}
	}
	else
		B.push_back(set[i]);
	
	auto xj = find(A.begin(), A.end(), set[j]);
	float r = static_cast <float> (rand())/static_cast <float> (RAND_MAX);
	if(xj != A.end()){
		if( r < 0.5 ){
			auto bxj = find(B.begin(), B.end(), set[j]);
			if(bxj != B.end()){
				int pos = bxj - B.begin();
				B.erase(B.begin() + pos);
			}
		}
	}
	else {
		if( r < 0.5 ) {
			B.push_back(set[j]);
		}
	}

	return B;
}

bool simulatedAnnealing(vector<int> ini, int actual_sum, vector<int> set){
	
	float temp = 5000; //Change back to 5000
	float alpha = 0.983; //Change back to 0.983
	float epsilon = 0.001;
	
	int itr = 0;
	int initial_sum = accumulate(ini.begin(), ini.end(), 0);
	int nsum;

	cout<<"Expected: "<<actual_sum<<"\n";
	while(temp > epsilon) {
		while(itr < 10000){ // Change back to itr<1000
			//srand(time(NULL));	
			vector<int> neighbor = getNeighbor(ini, set);
			auto neighbor_sum = accumulate(neighbor.begin(), neighbor.end(), 0);
		
			// Exit if the solution is found
			if(neighbor_sum == actual_sum){
				cout<<"Found true\n";
				return true;
			}
			
			float delta = neighbor_sum - initial_sum;
			
			float r = static_cast <float> (rand())/static_cast <float> (RAND_MAX);
			if(delta < 0)
				ini = neighbor;
			else if(r < exp(-1* (delta/temp)))
				ini = neighbor; 
			itr++;
			nsum = neighbor_sum;
		}
		temp *= alpha;
	}

		
	cout<<"SA sum: "<<nsum<<"\n";
	cout<<"Difference: "<<nsum - actual_sum<<"\n";
	return false;
}


int main(){
	srand(time(NULL));	

	// Store all instances
	vector<inp> inputBenchmark(totalInstances);
  
	// Get all instances
	inputBenchmark = generateInput(inputBenchmark);

		
		int foundSetE = 0, notFoundSetE = 0, benchmarksCompleteE = 0;
		int foundSetG = 0, notFoundSetG = 0, benchmarksCompleteG = 0;
		int foundSetS = 0, notFoundSetS = 0, benchmarksCompleteS = 0;

	/*
		cout<<"Running Exhaustive Algorithm------------------------------------"<<endl;
		// Run each instance
		for(int i=0;i<totalInstances;i++){
	
    	t1 = high_resolution_clock::now();

				
			// Check each function execution time
    	if (isSubsetSum(inputBenchmark[i].arr, inputBenchmark[i].n, inputBenchmark[i].sum) == true)
      	  foundSetE++;
    	else
      	  notFoundSetE++;

    	//auto t2 = high_resolution_clock::now();

    	// Getting number of milliseconds as an integer.
    	t_ms = duration_cast<seconds>(t2 - t1);
		
			// If the benchmark instance took more than 60s then time limit exceeded
			if(t_ms.count() < timeLimit){
				benchmarksCompleteE++;
				cout<<"Benchmark " << i+1 << " completed in time: " << t_ms.count() << '\n';
			}
			else{
				cout<<"Benchmark " << i+1 << " Time limit exceeded\n";
			}


			
		}

		cout<<"Number of sets found: " << foundSetE <<endl;
		cout<<"Number of sets not found: " << notFoundSetE <<endl;
		cout<<"Fraction of benchmarks completed: " << benchmarksCompleteE << " out of " << totalInstances 
																							 << " Percentage: " << (benchmarksCompleteE*100)/totalInstances <<endl;
		*/
	
	/*
	cout<<endl<<"Running Greedy Algorithm----------------------------------------"<<endl;


		// Run each instance
	for(int i=0;i<totalInstances;i++){
	
    	t1 = high_resolution_clock::now();
	

	vector<int> setGreedy;
				
	// Check each function execution time
	if (subsetGreedy(inputBenchmark[i].arr, inputBenchmark[i].n, inputBenchmark[i].sum, setGreedy) == true)
      	  foundSetG++;
    	else
      	  notFoundSetG++;

	//writeFile(inputBenchmark[i].arr, i, inputBenchmark[i].sum);	

    	auto t2 = high_resolution_clock::now();

    	// Getting number of milliseconds as an integer. 
    	t_ms = duration_cast<microseconds>(t2 - t1);
		
			// If the benchmark instance took more than 60s then time limit exceeded
			if(t_ms.count() < timeLimit){
				benchmarksCompleteG++;
				cout<<"Benchmark " << i+1 << " completed in time: " << t_ms.count() << '\n';
			}
			else{
				cout<<"Benchmark " << i+1 << " Time limit exceeded\n";
			}

	
			
		}

		cout<<"Number of sets found: " << foundSetG <<endl;
		cout<<"Number of sets not found: " << notFoundSetG <<endl;
		cout<<"Fraction of benchmarks completed: " << benchmarksCompleteG << " out of " << totalInstances 
																							 << " Percentage: " << (benchmarksCompleteG*100)/totalInstances <<endl;
	*/
	
	cout<<endl<<"Running Simulated Annealing----------------------------------------"<<endl;


	// Run each instance
	for(int i=0;i<totalInstances;i++){
	
    	t1 = high_resolution_clock::now();
	
	
	// Create an initial solution
	vector<int> setGreedyInitial;
				
	//Get the Initial solution set through greedy
	subsetGreedy(inputBenchmark[i].arr, inputBenchmark[i].n, inputBenchmark[i].sum - (rand()%30 + 1), setGreedyInitial);

	if (simulatedAnnealing(setGreedyInitial, inputBenchmark[i].sum, inputBenchmark[i].arr) == true)
      	  foundSetS++;
    	else
      	  notFoundSetS++;	
	
	
    	//auto t2 = high_resolution_clock::now();

    	// Getting number of milliseconds as an integer. 
    	t_ms = duration_cast<microseconds>(t2 - t1);
		
			// If the benchmark instance took more than 60s then time limit exceeded
			if(t_ms.count() < timeLimit){
				benchmarksCompleteS++;
				cout<<"Benchmark " << i+1 << " completed in time: " << (-1 * t_ms.count())/1000 << "\n\n";
			}
			else{
				cout<<"Benchmark " << i+1 << " Time limit exceeded\n\n";
			}

	
			
	}
	
		cout<<"Number of sets found: " << foundSetS <<endl;
		cout<<"Number of sets not found: " << notFoundSetS <<endl;
		cout<<"Fraction of benchmarks completed: " << benchmarksCompleteS << " out of " << totalInstances 
						<< " Percentage: " << (benchmarksCompleteS*100)/totalInstances <<endl;
	

	return 0;
}
