#include <iostream>
#include <random>
#include <map>


std::random_device rd;  // Obtain a random number from hardware
std::default_random_engine eng(rd()); // Seed the generator
std::uniform_int_distribution<int> distribution(1, 6);//losowanie liczby z przedziału a,b

std::map<int, int> results;
std::map<int, double> hitPercentage,woundPercentage,damagePercentage;

int sum, succeses, extraLethalDice;

double maxMap(std::map<int, double> Percentage) {
    double max=0,
            maxIndex;
    for (auto it : Percentage)
        if (it.second > max){max=it.second; maxIndex=it.first;}
    return maxIndex;
}

void displayStats(std::map<int,double> Percentage,int loops) {
    for (auto it : Percentage)
        std::cout << it.first << ": " << it.second*100 /loops <<"%"<< std::endl;
    std::cout << std::endl;
    std::cout<<"Expected outcome: "<<maxMap(Percentage)<<std::endl;
}

int d6Roll() {
    return distribution(eng);
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
    if(strength==toughness)
    {
        return 4;
    }
    if(strength>toughness && strength<2*toughness)
    {
        return 3;
    }
    if(strength>2*toughness)
    {
        return 2;
    }
    if(strength<toughness && 2*strength>toughness)
    {
        return 5;
    }
    if (2*strength<toughness)
    {return 6;}
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
    std::cin >> decision;
    int temp = int(decision);
    if(temp>=65 && temp<=90)
    {
        temp += 32;
        decision = char(temp);
    }
    if (decision == 'y') { decisionOuput = true; }
    else if (decision == 'n') { decisionOuput = false; }
    else {
        std::cout << "Invalid input. Please try again" << std::endl;
        isValidBoolInput();
    }
    return decisionOuput;
}

void simulation(int attacks,bool lethal, bool sustain, int threshold1, int threshold2, int threshold3, int loops) {
    int newAttacks;
    for (int i = 0;i < loops;i++) {
        succeses = 0;
        extraLethalDice=0;
        hitDiceThrow(attacks, threshold1,lethal,sustain,6);
        hitPercentage[succeses] += 1;
        newAttacks = sum;
        sum = 0;
        succeses = 0;
        results.clear();
        diceThrow(newAttacks, threshold2);
        woundPercentage[succeses] += 1;
        newAttacks = sum+extraLethalDice;
        sum = 0;
        succeses=0;
        results.clear();

        for (int j = 0;j < newAttacks;j++) {
            int temp = d6Roll();
            results[temp] += 1;
            if (temp < threshold3) {
                succeses+= 1;
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

    bool run=true;
    while (run) {
        invuSave=7;
        //todo zrobić listę broni i zapisywać oraz odczytywać typowe profile np. bolter, melta
        //port na aplikację
        std::cout << "Number of attacks: ";
        std::cin >> attacks;
        std::cout << "Hit on: ";
        std::cin >> threshold1;
        std::cout << "Strength of attacker: ";
        std::cin >> strength;
        std::cout << "AP: ";
        std::cin >> AP;
        std::cout<<"Lethal hits?(y/n): ";
        bool lethalHits=isValidBoolInput();
        std::cout<<"Sustain hits?(y/n): ";
        bool sustainHits=isValidBoolInput();
        std::cout << "Toughness of defender: ";
        std::cin >> toughness;
        std::cout << "Saves on: ";
        std::cin >> save;
        std::cout << "Invunerable save?(y/n): ";
        if (isValidBoolInput()) {
            std::cout <<"What value?: " << std::endl;
            std::cin >> invuSave;
        }
        std::cout<<"Does enemy have cover?(y/n): ";
        cover=isValidBoolInput();
        std::cout << "Number of simulations: ";
        std::cin >> loops;
        int threshold2=woundLogic(strength,toughness);
        int threshold3=invuLogic(save,AP,invuSave,cover);
        simulation(attacks, lethalHits, sustainHits, threshold1, threshold2, threshold3, loops);

        std::cout << "Hit stats: " << std::endl;
        displayStats(hitPercentage, loops);

        std::cout << "Wound stats: " << std::endl;
        displayStats(woundPercentage, loops);

        std::cout << "Damage stats: " << std::endl;
        displayStats(damagePercentage, loops);

        std::cout <<"Do you want to run program again?(y/n): " ;
        run=isValidBoolInput();
    }
    return 0;
}
