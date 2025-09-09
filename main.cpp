
#include <iostream>
#include <random>
#include <Windows.h>
#include <map>

using namespace std;

random_device rd;  // Obtain a random number from hardware
default_random_engine eng(rd()); // Seed the generator
uniform_int_distribution<int> distribution(1, 6);

map<int, int> results;
map<int, int> averageResults;
map<int, double> hitPercentage;
map<int, double> woundPercentage;
map<int, double> damagePercentage;

int sum, succeses;
float avgSum;

double maxMap(map<int, double> Percentage) {
	double max=0,
	maxIndex;
	for (auto it : Percentage)
		if (it.second > max){max=it.second; maxIndex=it.first;}
	return maxIndex;
}
void displayStats(map<int,double> Percentage,int loops) {
	for (auto it : Percentage)
		cout << it.first << ": " << it.second /loops *100<<"%" << endl;
	cout << endl;
	cout<<"Expected outcome: "<<maxMap(Percentage)<<endl;
}

int d6Roll() {
	int random_number = distribution(eng);
	return random_number;
}

void diceThrow(int number, int threshold) {
	int temp;
	for (int i = 0;i < number;i++) {
		temp = d6Roll();
		results[temp] += 1;
		if (temp>=threshold) {
			sum += 1;
			succeses += 1;
		}
	}
}

void simulation(int attacks, int threshold1, int threshold2, int threshold3, int loops) {
	int newAttacks;
	for (int i = 0;i < loops;i++) {
		succeses = 0;
		diceThrow(attacks, threshold1);
		hitPercentage[succeses] += 1;
		newAttacks = sum;
		sum = 0;
		results.clear();
		succeses = 0;
		diceThrow(newAttacks, threshold2);
		woundPercentage[succeses] += 1;
		newAttacks = sum;
		sum = 0;
		results.clear();
		succeses=0;
		for (int j = 0;j < newAttacks;j++) {
			int temp = d6Roll();
			results[temp] += 1;
			if (temp < threshold3) {
				succeses+= 1;
				sum += 1;
			}
		}
		damagePercentage[succeses] += 1;
	}
}

int main()
{
	int attacks;
	int threshold1, threshold2, threshold3;
	int loops;
	cout << "Number of attacks: ";
	cin >> attacks;
	cout << "Hit on: ";
	cin >> threshold1;
	cout << "Wound on: ";
	cin >> threshold2;
	cout << "Saves on: ";
	cin >> threshold3;
	cout << "Number of simulations: ";
	cin >> loops;
	simulation(attacks, threshold1, threshold2, threshold3, loops);

	cout << "Hit stats: " << endl;
	displayStats(hitPercentage, loops);

	cout << "Wound stats: " << endl;
	displayStats(woundPercentage, loops);

	cout << "Damage stats: " << endl;
	displayStats(damagePercentage, loops);
}

