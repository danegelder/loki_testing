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
    cout << "Balance is 3.12 Loki \n";
    cin >> command;
    cout << "Processing command " <<  command << " \n";
    if ( command.substr(0,8) == "transfer" ) {
        cout << "Transfer Succeeded.  Transaction reference is 928374923874\n";
    }
    cin >> command;
    cin >> command;
 
    return 0;
}

