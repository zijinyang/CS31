#include <iostream>
#include <string>
#include <iomanip> 
using namespace std;

int main()
{
    //init input vars, then prompt user for data
    int odometerStart, odometerEnd, rentalDays, startingMonth;
    string name; char luxury;
    
    cout << "Odometer at start: "; cin >> odometerStart;
    cout << "Odometer at end: "; cin >> odometerEnd;
    cout << "Rental days: "; cin >> rentalDays;
    cin.ignore(10000, '\n');
    cout << "Customer name: "; getline(cin, name);
    cout << "Luxury car? (y/n): "; cin >> luxury;
    cout << "Starting month (1=Jan, 2=Feb, etc.): "; cin >> startingMonth;

    double totalCost = 0.0;

    //calculate pre-day cost
    if(luxury == 'n'){
        totalCost += rentalDays * 45;

    }else{
        totalCost += rentalDays * 75;
    }
    
    //per-mile charge calculations
    int milesTot = odometerEnd - odometerStart;
    int milesLeft = milesTot;

    //if total is greater than 600
    if(milesLeft > 600){
        totalCost += (milesLeft - 600) * 0.21;
        milesLeft = 600;
    }

    //if total is greater than 100
    if(milesLeft > 100){
        if(startingMonth <= 3 && startingMonth >=12){
            totalCost += (milesLeft - 100) * 0.33;
            milesLeft = 100;
        }else{
            totalCost += (milesLeft - 100) * 0.27;
            milesLeft = 100;
        }
    }
    //base mile cost
    totalCost += milesLeft * 0.33;

    cout <<	"---\n";

    //catch exceptions
    if(odometerStart < 0){
        cout << "The starting odometer reading must not be negative.\n";
        return 1;
    }
    if(odometerEnd < odometerStart){
        cout << "The ending odometer reading must be at least as large as the starting reading.\n";
        return 1;
    }
    if(rentalDays <= 0){
        cout << "The number of rental days must be positive.\n";
        return 1;
    }
    if(name == ""){
        cout << "You must enter a customer name.\n";
        return 1;
    }
    if(luxury != 'y' && luxury != 'n'){
        cout << "You must enter y or n.\n";
        return 1;
    }
    if(startingMonth > 12 || startingMonth < 1){
        cout << "The month number must be in the range 1 through 12.\n";
        return 1;
    }

    //return cost rounding totalCost to two decmial points
	cout << "The rental charge for " << name <<" is $" << fixed << setprecision(2) << totalCost;
}