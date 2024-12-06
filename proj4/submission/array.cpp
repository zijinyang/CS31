#include <string>
#include <iostream>
#include <cassert>
using namespace std;

// //prints array seperated by commas, bugs if a is empty
// void printArr(string a[], int size){
// 	for(int i = 0; i < size - 1; i++){
// 		cout << a[i] << ", ";
// 	}
// 	cout << a[size-1] << endl;
// }

//checks if inputs are valid
bool checkIfValid(const string a[], int n){
	if(n < 0){
		return false;
	}
	return true;
}

int appendToAll(string a[], int n, string value){
	if(!checkIfValid(a, n)){
		return -1;
	}
	for(int i{0}; i < n; i++){
		a[i] += value;
	}
	return n;
}

int lookup(const string a[], int n, string target){
	if(!checkIfValid(a,n)){
		return -1;
	}
	for(int i{0}; i < n; i++){
		if(a[i] == target){
			return i;
		}
	}
	return -1;
}
int positionOfMax(const string a[], int n){
	if(!checkIfValid(a,n))
		return -1;
	if(n == 0)
		return -1;
	int maxInd = 0;
	for(int i{0}; i < n; i++){
		if(a[i] > a[maxInd]){
			maxInd = i;
		}
	}
	return maxInd;
}
int rotateLeft(string a[], int n, int pos){
	if(!checkIfValid(a,n))
		return -1;
	if(pos < 0)
		return -1;
	string last = a[pos];
	for(int i{pos + 1}; i < n; i++){ //start 1 right to pos as swapping current with left 
		a[i-1] = a[i];
	}
	a[n-1] = last;
	return pos;
}
int countRuns(const string a[], int n){
	if(!checkIfValid(a,n))
		return -1;
	if(n == 0)
		return 0;
	int numRuns = 1;
	string current = a[0];
	for(int i{0}; i < n; i++){
		if(a[i] != current){
			current = a[i];
			numRuns++;
		}
	}
	return numRuns;
}

int flip(string a[], int n){
	if(!checkIfValid(a,n))
		return -1;
	string temp;
	for(int i{0}; i < n/2; i++){
		temp = a[i];
		a[i] = a[n-i-1];
		a[n-i-1] = temp;
	}
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2){
	if(!checkIfValid(a1, n1) || !checkIfValid(a2, n2)){
		cout << "tf";
		return -1;
	}
	int i {0};
	//stops at smaller n, no out of bounds
	while(i < n1 && i < n2){
		if(a1[i] != a2[i])
			return i;
		i++; 
	}
	return i;
}
int subsequence(const string a1[], int n1, const string a2[], int n2){
	if(!checkIfValid(a1, n1) || !checkIfValid(a2, n2)){
		return -1;
	}
	int j{0};
	int startingInd =0;
	for(int i{0}; i < n1; i++){
		//if conseq end, reset and don't run
		if(a1[i] != a2[j]){
			j = 0;
			continue;
		}
		if(j == 0){
			startingInd = i;
		}
		j++;
		if(j > n2-1){
			return startingInd;	
		}
	}
	return -1;
}
int lookupAny(const string a1[], int n1, const string a2[], int n2){
	if(!checkIfValid(a1, n1) || !checkIfValid(a2, n2)){
		return -1;
	}
	for(int i{0}; i < n1; i++){
		for(int j{0}; j < n2; j++){
			if(a1[i] == a2[j]){
				return i;
			}
		}
	}
	return -1;	
}
int separate(string a[], int n, string separator){
	if(!checkIfValid(a,n))
		return -1;
	//set left and right bount for great and less than separator
	int separatorLeftInd = 0;
	int separatorRightInd = n-1;
	int i{0};
	while(i < n){
		//if less than, swap then increment i and bound
		if(a[i] < separator){
			string temp = a[separatorLeftInd];
			a[separatorLeftInd] = a[i];
			a[i] = temp;
			separatorLeftInd++;
			i++;	
		}else if(a[i] > separator){ //don't increment i as swapping new towards left
			string temp = a[separatorRightInd];
			a[separatorRightInd] = a[i];
			a[i] = temp;
			separatorRightInd--;
		}else if(a[i] == separator){
			i++;
		}
		if(i > separatorRightInd){
			break;
		}
	}
	return separatorLeftInd;	
}


int main(){
	// string candidate[5] = { "donald", "kamala", "jd", "tim", "jill" };
	// string arr[5]{ "donald!", "kamala!", "jd!", "tim", "jill" };
	// //test appendToAll
	// cout << "appendToAll Testing:" << endl;
	// cout << appendToAll(candidate, 3, "!") << endl;
	// printArr(candidate, 5);

	// cout << endl;
	string h[7] = { "melania", "kamala", "donald", "tim", "", "doug", "jd" };
	assert(lookup(h, 7, "doug") == 5);
	assert(lookup(h, 7, "donald") == 2);
	assert(lookup(h, 2, "donald") == -1);
	assert(positionOfMax(h, 7) == 3);

	string g[4] = { "melania", "kamala", "jd", "usha" };
	assert(differ(h, 4, g, 4) == 2);
	assert(appendToAll(g, 4, "?") == 4 && g[0] == "melania?" && g[3] == "usha?");
	assert(rotateLeft(g, 4, 1) == 1 && g[1] == "jd?" && g[3] == "kamala?");

	string e[4] = { "donald", "tim", "", "doug" };
	assert(subsequence(h, 7, e, 4) == 2);

	string d[5] = { "kamala", "kamala", "kamala", "tim", "tim" };
	assert(countRuns(d, 5) == 2);

	string f[3] = { "jd", "donald", "gwen" };
	assert(lookupAny(h, 7, f, 3) == 2);
	assert(flip(f, 3) == 3 && f[0] == "gwen" && f[2] == "jd");

	assert(separate(h, 7, "jd") == 3);

	cout << "All tests succeeded" << endl;
}