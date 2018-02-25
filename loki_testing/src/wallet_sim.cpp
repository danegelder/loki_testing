#include <iostream>
#include<string>
#include<string.h>
#include<fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <stdlib.h>     /* strtod */
#include <unistd.h>

using namespace std;

string walletName; 
string command; 



void LogInfo( string logMessage )
{
    FILE *fp;

    fp = fopen("/tmp/w.txt","a");
    fprintf(fp,"%s", logMessage.c_str());
    fclose(fp);

}


int main() 
{


    LogInfo ( "Wallet simulator started ..........................\n");

    long long x,y;
//    for (x=0;x<10000000000;x++) {
//	y = x * 2.9182347;
//   }

    cout << "WalletStarted!\n";
    cout << "Waiting for input, such as a wallet file name....";
    cin >> walletName;

    LogInfo ( "wallet name used by wallet sim:");
    LogInfo ( walletName.c_str());
    LogInfo ( "\n\n");



    cout << "Opening wallet " << walletName << "\n\n\n";
    cout << "Balance is 3.12 Loki \n";
    cin >> command;
    cout << "Processing command " <<  command << " \n";
    if ( command.substr(0,8) == "transfer" ) {
        cout << "Transfer Succeeded.  Transaction reference is 928374923874\n";
    }
    cin >> command;
    cin >> command;
 
    LogInfo ( "Wallet simulator finishing  ..........................\n");
    for (x=0;x<10000000000;x++) {
	y = x * 2.9182347;
    }
   
    LogInfo ( "Wallet simulator finished\n");

    return 0;
}

