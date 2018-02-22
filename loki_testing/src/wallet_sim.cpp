#include <iostream>
#include<string.h>

using namespace std;

string walletName; 
string command; 

int main() 
{
    cout << "WalletStarted!\n";
    cout << "Waiting for input, such as a wallet file name....";
    cin >> walletName;
    cout << "Opening wallet " << walletName << "\n\n\n";
    cout << "Balannce is 3.12 Loki \n";
    cin >> command;
    cout << "Processing command " <<  command << " \n";
    return 0;
}

