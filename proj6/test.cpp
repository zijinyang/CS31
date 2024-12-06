#include <iostream>
using namespace std;

void removeS(char* ptr){
    char* output = ptr;

    while(*ptr != 0){
        if(*ptr != 's' && *ptr != 'S'){
            *output = *ptr;
            output++;
        }
        ptr++; 
    }
    *output = 0;
}

int main()
{
    char msg[50] = "She'll blossom like a massless princess.";
    removeS(msg);
    cout << msg;  // prints   he'll bloom like a male prince.
}