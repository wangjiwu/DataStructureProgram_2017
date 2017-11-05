#include <iostream>
using namespace std;
void subset_my (int i, int n, int sum);
int obj[100] = {0};
bool visited[100] = {0};
int m = 0;
int main () {
	int count = 0, target_sum = 0;
	  //max object number
	cin >> count >> target_sum;
	for (int i = 0; i < count; i++) {
		cin >> obj[i];
	}
	subset_my(1, count, target_sum);
}

void subset_my (int i, int n, int sum) {
	int j = 0;
	if (i > n) {
		return;
	}
	visited[i] = true;
	m += obj[i];

	if (m == sum) {
		cout << "{";

		for (j = 1; j <= i; j++) {
			if (visited[j]) {
				cout << obj[j] <<" ";
			}
		}
		cout << "}" << endl;

	} else if (m < sum) {
		subset_my(i+1, n, sum);
	}
	visited[i] = false;
	m -= obj[i];
	subset_my(i+1, n, sum);
	return;
}
6 10 
1 8 4 3 5 2
