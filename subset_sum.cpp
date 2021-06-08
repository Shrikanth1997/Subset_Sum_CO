#include<iostream>
#include<vector>
#include<chrono>
#include <numeric>
#include <algorithm>

using namespace std;

// Benchmark instances
typedef struct inp {
	vector<int>arr;
	int n;
	int sum;
}inp; 

// Number of Instances needed to be tested
int totalInstances = 20;

// Timer variables
// Timer for running each benchmark instance
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::duration;
using chrono::seconds;

auto t1 = high_resolution_clock::now();
auto t2 = high_resolution_clock::now();
auto t_ms = duration_cast<seconds>(t2 - t1);

// Time Limit for each instance
int timeLimit = 60;

// Generate the random inputs required for each instance
vector<inp> generateInput(vector<inp>inputBenchmark) {

	int length = 35;
	for(int i=0;i<totalInstances;i++){
		
		for(int j=0;j<length;j++){
			inputBenchmark[i].arr.push_back(rand()%100);
		}
		inputBenchmark[i].n = length;
		auto rand_sum = accumulate(inputBenchmark[i].arr.begin(), inputBenchmark[i].arr.end(), 0);
		inputBenchmark[i].sum = rand() % rand_sum;

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
    	t_ms = duration_cast<seconds>(t2 - t1);
		
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

bool subsetGreedy(vector<int> arr, int n, int sum){
	
	sort(arr.begin(), arr.end(), comp);

	int s=0;

	for(int i=0;i<n;i++){
					if(s+arr[i]<=sum)
			s+=arr[i];
	}
	
	cout<<"Output for greedy: "<<s<<" ,Actual sum: "<<sum<<" with difference: "<<sum-s<<"\n\n";


	if(sum == s)
		return true;
	else
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
		cout<<endl<<"Running Greedy Algorithm----------------------------------------"<<endl;


		// Run each instance
		for(int i=0;i<totalInstances;i++){
	
    	t1 = high_resolution_clock::now();

				
			// Check each function execution time
			if (subsetGreedy(inputBenchmark[i].arr, inputBenchmark[i].n, inputBenchmark[i].sum) == true)
      	  foundSetG++;
    	else
      	  notFoundSetG++;

    	//auto t2 = high_resolution_clock::now();

    	/* Getting number of milliseconds as an integer. */
    	t_ms = duration_cast<seconds>(t2 - t1);
		
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

	return 0;
}
