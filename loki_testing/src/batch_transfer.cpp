#include <iostream>
#include<string>
#include<string.h>
#include<fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <stdlib.h>     /* strtod */
#include <unistd.h>


#define  MAX_NUMBER_OF_TRANSACTIONS  2000

#define PIPE_READ 0
#define PIPE_WRITE 1

int childPID;


using namespace std;

ifstream inFile;

string walletName; 
string command; 
string thisLine;
int numberOfTransactions = 0;

string response;
double InitialBalance = 100.0;
double TotalPayments  = 0.0;

int maxNumberOfTransactions = MAX_NUMBER_OF_TRANSACTIONS;
string destAddress[MAX_NUMBER_OF_TRANSACTIONS];
string destAmount [MAX_NUMBER_OF_TRANSACTIONS];
string destDelay  [MAX_NUMBER_OF_TRANSACTIONS];
string destPayID  [MAX_NUMBER_OF_TRANSACTIONS];
string destTXRef  [MAX_NUMBER_OF_TRANSACTIONS];
string destStatus [MAX_NUMBER_OF_TRANSACTIONS];




int j;

size_t find_nth(const string& haystack, size_t pos, const string& needle, size_t nth) 
{
    size_t found_pos = haystack.find(needle, pos);
    if(0 == nth || string::npos == found_pos)  return found_pos;
    return find_nth(haystack, found_pos+1, needle, nth-1);
}


int Transfer ( string address, string amount, string delay, string paymentid, string & txref, string & status )

{
    long long x,y;

    for (x = 0; x<500000000; x++) {
      y = x * 2;
    }
    // dump all transfers executed by this wallet, and check if the payment id exists

    // if payment id exists, then update status with 'TransferComplete' and update 'txref' with the transaction reference

    // otherwise, check available balance to confirm it is in excess of the transfer amount 

    // if balance is sufficient, then execute transfer, if not, skip this transaction


    if ( address == "L239487" ) {
	txref = "3409809808";
        status = "COMPLETE";
        return 0;
    }

    if ( address == "L984047" ) {
	txref = "0989682847";
        status = "COMPLETE";
        return 0;
    }

    if ( address == "L690449" ) {
	txref = "0248464649";
        status = "COMPLETE";
        return 0;
    }

    if ( address == "L309478" ) {
	txref = "3833535444";
        status = "COMPLETE";
        return 0;
    }


    status = "Pending";

}


void OutputStatus()
{
    int Txn;
    int PendingCount = 0;
    int CompleteCount = 0;

    system("clear");

    
    for (Txn = 0; Txn < numberOfTransactions; Txn++) {
        if ( destStatus[Txn] == "Pending" ) {
            PendingCount++;
        }
        if ( destStatus[Txn] == "COMPLETE" ) {
            CompleteCount++;
        }
    }

    cout << "Completed Transactions = " << (double) CompleteCount * 100.0 / (double) numberOfTransactions << "%, Pending Transactions = " << (double) PendingCount * 100.0 / (double) numberOfTransactions << "%.\n\n";


    cout << "Account     Amount Delay Reference                             Status \n";
 
    for (Txn = 0; Txn < numberOfTransactions; Txn++) {

        cout << destAddress[Txn] << "    " << destAmount[Txn] << "    " << destDelay[Txn] << "     " << destPayID[Txn] << "     " << destTXRef[Txn] << "     " << destStatus[Txn] << "\n";


    }
    cout.flush();
}



int ProcessTransactions()
{
    int Txn;

    for (Txn = 0; Txn < numberOfTransactions; Txn++) {
        Transfer( destAddress[Txn] , destAmount[Txn], destDelay[Txn],  destPayID[Txn], destTXRef[Txn], destStatus[Txn]);

        OutputStatus();
    }

    return 1;

}



int main() 
{

    int startChar;
    int endChar;
    int aStdinPipe[2];
    int aStdoutPipe[2];

    system("clear");

    cout << "TRANSFER LOKI based on contents of transactions.csv\n\n\n";
    cout << "";


    inFile.open("transactions.csv");

    while ( inFile  >> thisLine ) {
        destAddress[numberOfTransactions] = thisLine.substr(0, thisLine.find(","));                     // First field is the Address
	startChar  = find_nth(thisLine, 0, ",", 0) + 1;
	endChar    = find_nth(thisLine, 0, ",", 1);
        destAmount [numberOfTransactions] = thisLine.substr(startChar,endChar-startChar);        	// Second field is the Amount to Transfer
	startChar  = endChar + 1;
	endChar    = find_nth(thisLine, 0, ",", 2);
        destDelay    [numberOfTransactions] = thisLine.substr(startChar, endChar-startChar);            // Third field is the Block Delay
	startChar  = endChar + 1;
	endChar    = thisLine.length();
        destPayID    [numberOfTransactions] = thisLine.substr(startChar, endChar-startChar);              // Fourth field is the PaymentID
        destTXRef[numberOfTransactions] = "                    ";
        destStatus[numberOfTransactions] = "unknown";
        if ( atoi( destDelay[numberOfTransactions].c_str() )  > 0 ) {
            cout << "Send " << destAmount[numberOfTransactions] << " Loki to "  << destAddress[numberOfTransactions] << " with delay of " << destDelay[numberOfTransactions] << " blocks [" <<  destPayID[numberOfTransactions]  <<   "]\n"; 
        } else {
            cout << "Send " << destAmount[numberOfTransactions] << " Loki to "  << destAddress[numberOfTransactions] << " [" <<  destPayID[numberOfTransactions]  <<   "]\n";
        }

        TotalPayments += strtod( destAmount[numberOfTransactions].c_str(), NULL );
        numberOfTransactions++;
        if ( numberOfTransactions >= maxNumberOfTransactions) {
	    cout << "Too Many Transactions ....... only reading and processing the first " << maxNumberOfTransactions << " transactions\n";
            break;
	}
    }

    if ( numberOfTransactions == 0) {
	cout << "No transactions read from file 'transactions.csv' in the current directory - aborting.\n";
        return 0;
    }

    cout << "Number of transactions to be processed: " << numberOfTransactions << "\n";
    cout << "Currently Available Balance:            " << InitialBalance << "\n";
    cout << "Total amount of transfers to process:   " << TotalPayments << "\n";

    cout << "\n\n";

    cout << "This should leave a balance of:         " << InitialBalance - TotalPayments << "\n";
    cout << "\n\nNote: It is possible that some of these transactions have been process or partly processed.\nIf so, duplicate payments will not be submitted.";
    cout << "\n\nDo you wish to continue with these transfers? (y/n) ";
    cin >> response;
    if (response == "y") {
        cout << "Processing........\n";
    } else {
        cout << "Aborting\n";
    }

    j = 0;

    cout << "Launching wallet....................\n";

    childPID = fork();			// fork a new process
    
    if ( childPID == 0 ) {		// this code is the CHILD process, which has stdin and stdout re-directed such that the parent can interact with it
	cout << "This is the CHILD process running......\n\n";


        // redirect stdin
        if (dup2(aStdinPipe[PIPE_READ], STDIN_FILENO) == -1) {
          exit(0);
        }

        // redirect stdout
        if (dup2(aStdoutPipe[PIPE_WRITE], STDOUT_FILENO) == -1) {
          exit(0);
        }

        // redirect stderr
        if (dup2(aStdoutPipe[PIPE_WRITE], STDERR_FILENO) == -1) {
          exit(0);
        }

	cout << "This is the CHILD process running...... pipes now redirected\n\n";
        // all these are for use by parent only
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        close(aStdoutPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]); 
	cout << "This is the CHILD process running...... old pipes closed, about to execl\n\n";
       
        execl ( "./wallet_sim" ,  "./wallet_sim" , (char *) 0);

        cout << "Child process ...... execve failed ...............\n" ;

    } else if (childPID > 0) {

        // parent continues here
	cout << "This is the PARENT process running......\n\n";

        // close unused file descriptors, these are for child only
        close(aStdinPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]); 

        // Include error check here
//        if (NULL != szMessage) {
             //write(aStdinPipe[PIPE_WRITE], szMessage, strlen(szMessage));
        //}

        // Just a char by char read here, you can change it accordingly
        //while (read(aStdoutPipe[PIPE_READ], &nChar, 1) == 1) {
             //write(STDOUT_FILENO, &nChar, 1);
        //}

        // done with these in this example program, you would normally keep these
        // open of course as long as you want to talk to the child
        //close(aStdinPipe[PIPE_WRITE]);
        //close(aStdoutPipe[PIPE_READ]);

    }


cin >> j;
    while ( ProcessTransactions() != 0 ) {

	ProcessTransactions();
        j++;

        if (j > 10) {
	    break;
        }

    } 


    return 0;
}

