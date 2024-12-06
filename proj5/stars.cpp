#include "utilities.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cctype>
using namespace std;

const char WORDFILENAME[] = "/Users/kevinyang/Documents/CS31/proj5/text.txt";

/*
Given a cstring and an index, remove char at that index by shifting all chars to the right one char left
*/
void removeChar(char word[], int& index){
	for(int i{index}; word[i] != '\0'; i++){
		word[i] = word[i+1];
	}
}

int runOneRound(const char words[][7], int nWords, int wordnum){
    bool correct = false;
	int numTries = 0;
	//check validity of inputs
	if (nWords == 0 || wordnum < 0 || wordnum >= nWords)
	{
		return -1;
	}
	char word[7]; strcpy(word, words[wordnum]);
	// cout << word
    while(!correct){   
        cout << "Trial word: ";
        char trialWord[1000]; cin.getline(trialWord, 1000);
		bool isValidWord = true;
		//check if word is right length
		if(strlen(trialWord) < 4 || strlen(trialWord) > 6){
			isValidWord = false;
		}
		//check if word has uppercase
		for(int i{0}; trialWord[i] != '\0'; i++){
			if(!islower(trialWord[i])){
				isValidWord = false;
			}
		}
		if(!isValidWord){
			cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
			continue;
		}
        // check if given word is in list, if not, continue
        bool wordInList = false;
        for(int i{0}; i < nWords; i++){
            if(strcmp(trialWord, words[i]) == 0){
                wordInList = true;
            }
        }
		if(!wordInList){
            cout << "I don't know that word." << endl;
            continue;  
        }
		
		numTries++;

		if(strcmp(trialWord, word) == 0){
			correct = true;
			return numTries; 
		}
        
        int numStars = 0;
        int numPlanets = 0;
        char tempTrialWord[7]; strcpy(tempTrialWord, trialWord);
        char tempWord[7]; strcpy(tempWord, word);

		//count stars
        for(int i{0}; tempTrialWord[i] != '\0' && tempWord[i] != '\0'; i++){
			if(tempTrialWord[i] == tempWord[i]){
				numStars++;
				removeChar(tempTrialWord, i);
				removeChar(tempWord, i);
				i--; //needed to stay on same index, as next char has moved to current index with the removal of the current char
			}
        }
		//count planets
        for(int i{0}; tempTrialWord[i] != '\0'; i++){
			for(int j{0}; tempWord[j] != '\0'; j++){
				if(tempTrialWord[i] == tempWord[j]){
					numPlanets++;
					removeChar(tempWord,j);
					removeChar(tempTrialWord, i);
					i--;
					break;
				}
			}
		}
		cout << "Stars: " << numStars << ", Planets: " << numPlanets << endl;
    } 
	return -1;   
}

int main()
{
	char words[10000][7];
	int nWords = getWords(words, 10000, WORDFILENAME);
	//check if nWords ran correclty
	if(nWords < 1){
		cout << "No words were loaded, so I can't play the game." << endl;
		return 1;
	}
	cout << "How many rounds do you want to play? ";
	int numRounds; cin >> numRounds;
	cin.ignore(1000,'\n');
	//check if numRounds is positive
	if(numRounds < 0){
		cout << "The number of rounds must be positive." << endl;
		return 1;
	}
	
	int totalTries = 0;
	int maxTries = 0;
	int minTries = 0;
	for(int i{1}; i <= numRounds; i++){
		cout << endl;
		cout << "Round " << i<< endl;
		int wordNum = randInt(0, nWords - 1);
		cout << "The secret word is " << strlen(words[wordNum]) << " letters long." << endl;
		// cerr << words[wordNum] << endl;
		int numTries = runOneRound(words, nWords, wordNum);
		//check if numTries runs correctly
		if(numTries == -1){
			cerr << "Something went wrong" << endl;
			continue;
		}
		if(numTries == 1)
			cout << "You got it in " << numTries << " try." << endl;
		else
			cout << "You got it in " << numTries << " tries." << endl;
		totalTries += numTries;
		if(numTries > maxTries)
			maxTries = numTries;
		if(numTries < minTries || i == 1) //set minTires equal to current tries if it's the first round
			minTries = numTries;

		double average = static_cast<double>(totalTries)/i;
		cout << "Average: " << fixed << setprecision(2) << average << ", minimum: " << minTries << ", maximum: " << maxTries << endl; 
	}

    return 0;
}