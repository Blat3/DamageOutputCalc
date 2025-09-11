#include <iostream>
#include <random>
#include <map>

using namespace std;

random_device rd;  // Obtain a random number from hardware
default_random_engine eng(rd()); // Seed the generator
uniform_int_distribution<int> distribution(1, 6);//losowanie liczby z przedziału a,b

map<int, int> results;
map<int, double> hitPercentage,woundPercentage,damagePercentage;

int sum, succeses, extraLethalDice;

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
	return distribution(eng);
}

void diceThrow(int number, int threshold) {
	for (int i = 0;i < number;i++) {
		int temp = d6Roll();
		results[temp] += 1;
		if (temp>=threshold) {
			sum += 1;
			succeses += 1;
		}
	}
}

void hitDiceThrow(int number, int threshold,bool lethal,bool sustain,int crit) {
	for (int i = 0;i < number;i++) {
		int temp = d6Roll();
		results[temp] += 1;
		if (sustain && temp==crit) {
			sum+=1;
		}
		if (lethal && temp==crit) {
			extraLethalDice+=1;
		}
		if (temp>=threshold) {
			sum += 1;
			succeses += 1;
		}
	}
}

int woundLogic(int strength, int toughness)
{
    double division=strength/toughness;
    if(strength==toughness)
    {
        return 4;
    }
    if(1<division && division<2)
    {
        return 3;
    }
    if(division>=2)
    {
        return 2;
    }
    if(0.5<division  && division<1)
    {
        return 5;
    }
    return 6;
}

int invuLogic(int save,int AP,int invu,bool cover) {
	if (cover && (AP!=0 || save>3)) {
		if(save+AP-1<invu) {
			return save+AP-1;
		}
		return invu;
	}
	if (save+AP<invu) {
		return save+AP;
	}
	return invu;
}

bool isValidBoolInput()
{
	bool decisionOuput=false;
	char decision;
	cin >> decision;
	int temp = int(decision);
	if(temp>=65 && temp<=90)
	{
		temp += 32;
		decision = char(temp);
	}
	if (decision == 'y') { decisionOuput = true; }
	else if (decision == 'n') { decisionOuput = false; }
	else {
		cout << "Invalid input. Please try again" << endl;
		isValidBoolInput();
	}
	return decisionOuput;
}

void simulation(int attacks,bool lethal, bool sustain, int threshold1, int threshold2, int threshold3, int loops) {
	for (int i = 0;i < loops;i++) {
		succeses = 0;
		extraLethalDice=0;
		hitDiceThrow(attacks, threshold1,lethal,sustain,6);
		hitPercentage[succeses] += 1;
		int newAttacks = sum;
		sum = 0;
		results.clear();
		succeses = 0;
		diceThrow(newAttacks, threshold2);
		woundPercentage[succeses] += 1;
		newAttacks = sum+extraLethalDice;
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
	bool cover;
	int attacks,strength,toughness,AP,save;
	int invuSave=7;
	int threshold1;
	int loops;

	//todo zrobić listę broni i zapisywać oraz odczytywać typowe profile np. bolter, melta
	//port na aplikację
	cout << "Number of attacks: ";
	cin >> attacks;
	cout << "Hit on: ";
	cin >> threshold1;
	cout << "Strength of attacker: ";
	cin >> strength;
	cout << "AP: ";
	cin >> AP;
	cout<<"Lethal hits?(y/n): ";
	bool lethalHits=isValidBoolInput();
	cout<<"Sustain hits?(y/n): ";
	bool sustainHits=isValidBoolInput();
	cout << "Toughness of defender: ";
	cin >> toughness;
	cout << "Saves on: ";
	cin >> save;
	cout << "Invunerable save?(y/n): ";
	if (isValidBoolInput()) {
		cout <<"What value?: " <<endl;
		cin >> invuSave;
	}
	cout<<"Do you have cover?(y/n): ";
	cover=isValidBoolInput();
	cout << "Number of simulations: ";
	cin >> loops;
	int threshold2=woundLogic(strength,toughness);
	int threshold3=invuLogic(save,AP,invuSave,cover);
	simulation(attacks, lethalHits, sustainHits, threshold1, threshold2, threshold3, loops);

	cout << "Hit stats: " << endl;
	displayStats(hitPercentage, loops);

	cout << "Wound stats: " << endl;
	displayStats(woundPercentage, loops);

	cout << "Damage stats: " << endl;
	displayStats(damagePercentage, loops);
}
