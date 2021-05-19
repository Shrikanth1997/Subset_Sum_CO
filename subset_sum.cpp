#include<bits/stdc++.h>

using namespace std;

typedef struct inp {
	vector<int>arr;
	int n;
	int sum;
}inp; 

void generateInput(inp inputBenchmark) {

		

}

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
	
	inp inputBenchmark[100];
  
	// Timer for running the benchmarks
	/*using chrono::system_clock;	
	auto finish = system_clock::now() + 5s;
  do
    {
    } while (system_clock::now() < finish);*/
    
		using chrono::high_resolution_clock;
    using chrono::duration_cast;
    using chrono::duration;
    using chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

		// Check each function execution time
		vector<int> set{3, 34, 4, 12, 5, 2, 4, 57, 9, 6, 11, 47, 30, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};
    int sum = 500;
    int n = set.size();
		cout << "Size: " << n << "\n";

    if (isSubsetSum(set, n, sum) == true)
        printf("Found a subset with given sum\n");
    else
        printf("No subset with given sum\n");


    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto t_ms = duration_cast<milliseconds>(t2 - t1);
		
		cout << t_ms.count() << "ms\n";

	return 0;
}
