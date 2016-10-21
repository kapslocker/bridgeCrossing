#include <iostream>
#include <cmath>
#include <fstream>
#include <limits.h>
using namespace std;

const int MAX_P = 10;
int dp[1024];	// Saves f(S) for a given mask. (The masks <1024 => # of people less than 10. )
int n;				// The number of people.
int t[MAX_P];	// The time taken for an individual person to cross the bridge.
int C;				// The capacity of a boat.
/*
* Evaluates the complement of a given set S (mask s).
* Complement is the value 1111..MAX_P times - s.
*/
int comp(int s){
	int p = pow(2,MAX_P) -1;
	return p - s;
}
/*
* Evaluates union of two sets s, g.
* This is the bitwise OR of the two masks.
*/
int uni(int s, int g){
	return s|g;
}
/*
* Removes the elements of G from S.
* This works because G is always a subset of S (in our case).
*/
int negation(int s,int g){
	return s - g;
}

/*
*	makes a binary representation for a singleton set
*	containing a person i.( indexed from 0), which is simply 2^i.
*/
int make_set(int i){
	return pow(2,i);
}

int cardinality(int s){
	return __builtin_popcount(s);
}

/*
* The fastest person in a given set S.
* In the binary representation of S,
* 	if bit at index i == 1,
* 		check if the person has speed less than the current min speed.
*/
int fastest(int s){
	int i = 0;
	int f = 0,min_t = INT_MAX;
	while(s>0){
		int k = s%2;
		if(k){
			if(t[i]<min_t){
				min_t = t[i];
				f = i;
			}
		}
		i++;
		s /=2;
	}
	return f;
}

/*
*	returns the time taken to transfer the set
*	completely from the South to North bank.
* This is equal to the time taken to cross the bridge,
* by the slowest person in the set.
*/
int complete_time(int s){
	int i = 0;
	int f = 0,max_t = INT_MIN;
	while(s>0){
		int k = s%2;
		if(k){
			if(t[i]>max_t){
				max_t = t[i];
				f = i;
			}
		}
		i++;
		s /=2;
	}
	return f;
}

/*
*	next(S,G) is given by (S-G) U { fastest( (P-S) U G) }
*/
int next(int s, int g){
	int people_north = uni(comp(s),g);
	int returning_person = fastest(people_north);
	return uni(negation(s,g), make_set(returning_person));
}

/*
*	The time taken to move the subset G of S from South to north and
*	return the fastest person on the northern bank with the torch.
* which is complete_time(G) + fastest in (P-S) U G .
*/
int T(int s,int g){
	int people_north = uni(comp(s),g);
	return complete_time(g) + t[fastest(people_north)];
}

/*
* The time taken to transfer a set S from the south bank to the north bank.
* The function is defined later.
*/
int f(int s);

/*
* The time to transfer a set S from south to north, given that G go first.
* with |G|<=C.
*/
int f(int s,int g){
	if(cardinality(s)<C)
		return complete_time(s);

	return T(s,g) + f(next(s,g));
}

int genf(){
	int p = 1;
	for(int i=0;i<MAX_P;i++){
		dp[p] = t[i];
		p*=2;
	}
	for(int i = 1;i<p;i++){
		if(dp[i]==0){
			if(cardinality(i)<=C){
				dp[i] = complete_time(i);
			}

		}
	}
}


int f(int s){
	return dp[s];
}
/*
* Calculates the minimum time required to move
*	a set S of people on the southern bank to the north.
*	We proceed in increasing order of set cardinality.
*/
int minT(int s){
	return 0;
}
int main(){
	ifstream fin;
	fin.open("input.txt",ios::in);
	ofstream fout;
	fout.open("output.txt",ios::out);
	for(int i=0;i<4;i++)
	{
		fin>>t[i];
	}
	fout<<next(7,6);
	fin.close();
	fout.close();
	return 0;
}
