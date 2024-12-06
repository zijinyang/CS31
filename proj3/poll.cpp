#include <iostream>
#include <cctype>
#include <string>
#include <cassert>

using namespace std;

//*************************************
//  isValidUppercaseStateCode
//*************************************

// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.

bool isValidUppercaseStateCode(string stateCode)
{
    const string codes =
        "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
        "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NJ.NH.NM.NY.NC."
        "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
            stateCode.find('.') == string::npos  &&  // no '.' in stateCode
            codes.find(stateCode) != string::npos);  // match found
}

/*
    converts string of letters to all uppercase
*/

string toUpper(string stateCode){
    string output;
    for(int i{0}; i < stateCode.size(); i++){
        stateCode[i] = toupper(stateCode[i]);
    }
    return stateCode;
}

/*
    increments index, returns true if index out of bounds of pollData string, returns false otherwise
*/
bool incrIdx(int& idx, string pollData){
    idx++;
    if(idx == pollData.size()){
        return false;
    }
    return true;
}

/*
    Takes full pollData string and first index of the current state prediction. 
    Checkes if the state prediciton, if not, returns false.
    If it is valid, adds the number of votes in the state prediction to voteCount, and returns true
 */
bool processStatePred(string pollData, int& idx, int& voteCount){
    //increment index to first index of statePred
    if(!incrIdx(idx,pollData)){return false;}

    //check if next 1-2 char is valid number of votes, can only be 1-2 digits
    //if valid, sets equal to numVotesStr
    int numVotesLen = 0;
    string numVotesStr = "";
    if(!isdigit(pollData.at(idx))){
        return false;
    }
    while(isdigit(pollData.at(idx))){
        numVotesLen++;
        if(numVotesLen > 2){
            return false;
        }
        numVotesStr += pollData.at(idx);
        if(!incrIdx(idx,pollData)){return false;}
    }

    //convert numVotesStr into int
    int numVotes = 0;
    if(numVotesStr.size() == 2){
        numVotes += (numVotesStr.at(0) - '0') * 10;
        numVotes += (numVotesStr.at(1) - '0');
    }else{
        numVotes += (numVotesStr.at(0) - '0');
    }

    voteCount = numVotes;
    
    //move idx forwards to check if there is full 2 digit state code
    if(!incrIdx(idx,pollData)){return false;}

    //check if next 2 chars is valid state code
    if(!isValidUppercaseStateCode(toUpper(pollData.substr(idx-1,2)))){
        return false;
    }

    return true;
}

bool hasRightSyntax(string pollData){
    int i{0};
    while(i != pollData.size()){
        //dummy voteCount to give to function
        int temp = 0;
        //check if current ind is a valid party code
        if(!isalpha(pollData.at(i))){
            return false;
        }
        //check if statepred is valid
        if(!processStatePred(pollData, i, temp)){
            return false;
        }
        i++;
    }
    return true;
}

int computeVotes(string pollData, char party, int& voteCount){
    //check syntax
    if(!hasRightSyntax(pollData)){
        return 1;
    }

    int initVoteCount = voteCount;    

    int i{0};
    int numVotesTot = 0;
    while(i != pollData.size()){
        char currentParty = pollData.at(i);

        int numVotes = 0;

        processStatePred(pollData, i, numVotes);

        if(numVotes == 0){
            voteCount = initVoteCount;
            return 2;
        }

        if(!isalpha(party)){
            return 3;
        }

        if(toupper(currentParty) == toupper(party)){
            numVotesTot += numVotes;
        }
        i++;
    }
    voteCount = numVotesTot;
    return 0;
}

const string testCases[] {
    "RR40TXD54CAr6Msd28nYL06UT",
    "DR40TXD54CAr6Msd28nYL06UT",
    "rR40TXD54CAr6Msd28nYL06UT",
    "lR40TXD54CAr6Msd28nYL06UT",
    "RR",
    "RR40TXD0CA",
    "RR40 TX",
    "2R40TX",
    "RR R",
    "SR40CA D2CA",
    "KM20ILD20CA",
    "R"
};

int main(){
    for(string testCase: testCases){
        int voteCount = 20;
        cout << testCase << endl;
        // cout << hasRightSyntax(testCase.substr(1,testCase.size())) << endl;
        cout << computeVotes(testCase.substr(1,testCase.size()), testCase[0], voteCount) << endl << voteCount << endl;
    }
}