#include<iostream>
#include<vector>
#include<chrono>
#include<numeric>
#include<algorithm>
#include<fstream>
#include<math.h>
#include<string>
#include<climits>
#include<stdlib.h>
#include <map>

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

	for(int i=0;i<n;i++)
		cout<<arr[i]<<" ";
	cout<<endl;	
	cout<<"Output for greedy: "<<s<<" ,Actual sum: "<<sum<<" with difference: "<<sum-s<<"\n\n";
	cout<<"Set for Greedy = "<<endl;
	for(int i=0;i<setGreedy.size();i++)
		cout<<setGreedy[i]<<" ";
	cout<<endl<<endl;


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

vector<int> approxRandom(vector<int> &arr, int size, int target) {
    sort(arr.begin(), arr.end(), comp);
    srand(time(0));  // Initialize random number generator.
    int s = 0;
    map<int,  int> m;
    vector<int> randomNeighbor;
    while(s <= target) {
        //generate random index
        int rIndex = rand() % size;
        //make sure its unique
        if (!m[rIndex]) {
            m[rIndex]++;
            s += arr[rIndex];
            randomNeighbor.push_back(rIndex);
            //cout << arr[rIndex] << " ";
        }
    }
    return randomNeighbor;
}

vector<int> partialGreedy(vector<int> arr, int n, int sum) {
    vector<int> ans;
    sort(arr.begin(), arr.end(), comp);

    int s=0;

    for(int i=0;i<n;i++){
        if(s+arr[i] < sum) { //does not include solution
            s+=arr[i];
            ans.push_back(arr[i]);
        }
    }
    return ans;
}

bool searchAlgorithm(vector<int> arr, int n, int t, vector<int> neighborIndexs) {
    map<int, int> m;
    for (auto e : arr)
        m[e]++;
    int approxSum = 0;
    for (auto e : neighborIndexs) {
        approxSum += arr[e];
        m[arr[e]]--;
    }
    int iterations = 0;
    int mindex = 0; //index value of largest number
    int mindexLocation = 0; //where in the neighborIndex array mindex is stored
    int maxdex = neighborIndexs.back(); //index of the smallest number
    while(true) {
        if (approxSum < t) {
            //look for a subset to add
            if (m[t-approxSum]) {
                return true;
            }

            //make sure its in range and not already used
            if (maxdex + 1 >= n)
                return false;
            if (   find(neighborIndexs.begin(), neighborIndexs.end(), maxdex+1) == neighborIndexs.end()  ) {
                neighborIndexs.push_back(maxdex + 1);
                maxdex++;
                //update sum
                approxSum += arr[maxdex];
                m[arr[maxdex]]--;

            } else {
                return false;
            }

        } else {
            //check if if there is a removal that gives the target
            for (int i = 0; i < neighborIndexs.size(); i++) {
                if ( (approxSum - arr[neighborIndexs[i]]) == t)
                    return true;
            }
            //check if a replacement can give the target
            for (auto e : neighborIndexs) {
                int tempSumAfterRemoval = approxSum - arr[e];
                int searchValue = t - tempSumAfterRemoval;
                if (m[searchValue])
                    return true;
            }
            //replace biggest with next smallest
            //should be less than n
            //should not already be in the index array
            //need to update max to the next largest
            //and min becomes the new addition
            int nextSmallestIndex = maxdex++;

            if (nextSmallestIndex >= n)
                return false;

            if (find(neighborIndexs.begin(), neighborIndexs.end(), nextSmallestIndex) == neighborIndexs.end()) {
                //update table
                m[arr[mindex]]++;
                m[arr[nextSmallestIndex]]--;
                //update sum
                approxSum = (approxSum - arr[mindex]) + arr[nextSmallestIndex];


                //swap
                neighborIndexs[mindexLocation] = nextSmallestIndex;
                //swap largest numbers index with the largest number remaining
                //the largest number remaing should have the smallest index value
                int minFound = INT_MAX;
                for (int i = 0; i < neighborIndexs.size(); i++) {
                    if (neighborIndexs[i] < minFound)
                        minFound = neighborIndexs[i];
                    mindexLocation = i;
                }
                //mindex updated to the index of the second largest number
                mindex = minFound;

                //new number added will be the smallest of the numbers so far
                maxdex = nextSmallestIndex;
            }
        }
        iterations++;
    }
}

bool randomSearch(vector<int> arr, int n, int t) {
   //generate random approximation
   vector<int> randNeighbor = approxRandom(arr, n, t);
   //apply search algorithm
   return searchAlgorithm(arr, n, t, randNeighbor);
}

bool greedSearch(vector<int> arr, int n, int t) {
    //generate greedy neighbor approximation
    vector<int> greedNeighbor = partialGreedy(arr, n, t);
    //apply search algorithm
    return searchAlgorithm(arr, n, t, greedNeighbor);
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
		int foundSetRS = 0, notFoundSetRS = 0, benchmarksCompleteRS = 0;
		int foundSetGS = 0, notFoundSetGS = 0, benchmarksCompleteGS = 0;

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
*/
    cout<<endl<<"Running random approximations and search-----------------------------------"<<endl;
    for (int i = 0; i < totalInstances;i++) {
        t1 = high_resolution_clock::now();

        //use a random algorithm to find initial subset
        if (randomSearch(inputBenchmark[i].arr, inputBenchmark[i].n, inputBenchmark[i].sum))
            foundSetRS++;
        else
            notFoundSetRS++;

        //auto t2 = high_resolution_clock::now();

        // Getting number of milliseconds as an integer.
        t_ms = duration_cast<microseconds>(t2 - t1);

        // If the benchmark instance took more than 60s then time limit exceeded
        if(t_ms.count() < timeLimit){
            benchmarksCompleteRS++;
            cout<<"Benchmark " << i+1 << " completed in time: " << (-1 * t_ms.count())/1000 << "\n\n";
        }
        else{
            cout<<"Benchmark " << i+1 << " Time limit exceeded\n\n";
        }

    }
    cout<<"Number of sets found: " << foundSetRS <<endl;
    cout<<"Number of sets not found: " << notFoundSetRS <<endl;
    cout<<"Fraction of benchmarks completed: " << benchmarksCompleteRS << " out of " << totalInstances
        << " Percentage: " << (benchmarksCompleteRS*100)/totalInstances <<endl;

    cout<<endl<<"Running greedy approximation and search-----------------------------------"<<endl;
    for (int i = 0; i < totalInstances;i++) {
        t1 = high_resolution_clock::now();
        //use a random algorithm to find initial subset
        if (greedSearch(inputBenchmark[i].arr, inputBenchmark[i].n, inputBenchmark[i].sum))
            foundSetGS++;
        else
            notFoundSetGS++;

        //auto t2 = high_resolution_clock::now();

        // Getting number of milliseconds as an integer.
        t_ms = duration_cast<microseconds>(t2 - t1);

        // If the benchmark instance took more than 60s then time limit exceeded
        if(t_ms.count() < timeLimit){
            benchmarksCompleteGS++;
            cout<<"Benchmark " << i+1 << " completed in time: " << (-1 * t_ms.count())/1000 << "\n\n";
        }
        else{
            cout<<"Benchmark " << i+1 << " Time limit exceeded\n\n";
        }

    }
    cout<<"Number of sets found: " << foundSetGS <<endl;
    cout<<"Number of sets not found: " << notFoundSetGS <<endl;
    cout<<"Fraction of benchmarks completed: " << benchmarksCompleteGS << " out of " << totalInstances
        << " Percentage: " << (benchmarksCompleteGS*100)/totalInstances <<endl;


    return 0;
}
