#include <iostream>
#include <cstring>   
using namespace std;

class temp
{
    public:
        temp();
        temp(int n);
        int getNum();
    private:
        int num;
}; 

temp::temp(int n){
    num = n;
}

int temp::getNum(){
    return num;
}

void point(temp* arr[], int num, int i){
    // cout << num << endl;
    arr[i] = new temp(num);
}

int main()
{
    temp* arr[2];
    point(arr, 1, 0);
    point(arr, 2, 1);
    // cout << arr[0];
    cout << arr[0] -> getNum() << endl;
    // cout << arr[1] -> getNum() << endl;
}
