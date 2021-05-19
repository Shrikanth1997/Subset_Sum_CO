#include<bits/stdc++.h>

using namespace std;

// Benchmark instances
typedef struct inp {
	vector<int>arr;
	int n;
	int sum;
}inp; 

// Number of Instances needed to be tested
int totalInstances = 15;

// Generate the random inputs required for each instance
vector<inp> generateInput(vector<inp>inputBenchmark) {

	int length = 10;
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

    if (arr[n - 1] > sum)
        return isSubsetSum(arr, n - 1, sum);

    return isSubsetSum(arr, n - 1, sum) 
						|| isSubsetSum(arr, n - 1, sum - arr[n - 1]);
}

int main(){
	
	// Store all instances
	vector<inp> inputBenchmark(totalInstances);
  
	// Get all instances
	inputBenchmark = generateInput(inputBenchmark);

	// Timer for running each benchmark instance
		using chrono::high_resolution_clock;
    using chrono::duration_cast;
    using chrono::duration;
    using chrono::seconds;

	
		int foundSet = 0, notFoundSet = 0, benchmarksComplete = 0;

		// Run each instance
		for(int i=0;i<totalInstances;i++){
	
    	auto t1 = high_resolution_clock::now();

			// Check each function execution time
    	if (isSubsetSum(inputBenchmark[i].arr, inputBenchmark[i].n, inputBenchmark[i].sum) == true)
      	  foundSet++;
    	else
      	  notFoundSet++;


    	auto t2 = high_resolution_clock::now();

    	/* Getting number of milliseconds as an integer. */
    	auto t_ms = duration_cast<seconds>(t2 - t1);
		
			// If the benchmark instance took more than 60s then time limit exceeded
			if(t_ms.count() < 60){
				benchmarksComplete++;
				cout<<"Benchmark " << i << " completed\n";
			}
			else{
				cout<<"Time limit exceeded\n";
			}


			
		}

		cout<<"Number of sets found: " << foundSet <<endl;
		cout<<"Number of sets not found: " << notFoundSet <<endl;
		cout<<"Fraction of benchmarks completed: " << benchmarksComplete <<endl;

	return 0;
}
