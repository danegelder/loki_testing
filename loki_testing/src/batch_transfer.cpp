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

#define DEVTTY "/dev/tty"		// for Windows use "con"
std::ofstream ofs(DEVTTY);

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
double AvailableBalance = 0.0;
double TotalBalance = 0.0;

int maxNumberOfTransactions = MAX_NUMBER_OF_TRANSACTIONS;
string destAddress[MAX_NUMBER_OF_TRANSACTIONS];
string destAmount [MAX_NUMBER_OF_TRANSACTIONS];
string destDelay  [MAX_NUMBER_OF_TRANSACTIONS];
string destPayID  [MAX_NUMBER_OF_TRANSACTIONS];
string destTXRef  [MAX_NUMBER_OF_TRANSACTIONS];
string destStatus [MAX_NUMBER_OF_TRANSACTIONS];
string tmpString;




int j;

size_t find_nth(const string& haystack, size_t pos, const string& needle, size_t nth) 
{
    size_t found_pos = haystack.find(needle, pos);
    if(0 == nth || string::npos == found_pos)  return found_pos;
    return find_nth(haystack, found_pos+1, needle, nth-1);
}


void LogInfo( string logMessage )
{
    FILE *fp;

    fp = fopen("/tmp/w.txt","a");
    fprintf(fp,"%s", logMessage.c_str());
    fclose(fp);

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
    int UnknownCount = 0;
    int UnprocessedCount = 0;

    double PendingAmount = 0.0;
    double CompleteAmount = 0.0;
    double UnknownAmount = 0.0;
    double UnprocessedAmount = 0.0;

    ofs << "\033[2J\033[1;1H";

    for (Txn = 0; Txn < numberOfTransactions; Txn++) {
        if ( destStatus[Txn] == "Pending" ) {
            PendingAmount += atof( destAmount[Txn].c_str() );
            PendingCount++;
        }
        if ( destStatus[Txn] == "COMPLETE" ) {
            CompleteAmount += atof( destAmount[Txn].c_str() );
            CompleteCount++;
        }
        if ( destStatus[Txn] == "Unprocessed" ) {
            UnprocessedAmount += atof( destAmount[Txn].c_str() );
            UnprocessedCount++;
        }
        if ( destStatus[Txn] == "Unknown" ) {
            UnknownAmount += atof( destAmount[Txn].c_str() );
            UnknownCount++;
        }
    }

    ofs << "Completed Transactions = " << (double) CompleteCount * 100.0 / (double) numberOfTransactions << "%, Pending Transactions = " << (double) PendingCount * 100.0 / (double) numberOfTransactions << "%.\n\n";

    if ( AvailableBalance < UnprocessedAmount ) {
        ofs << "Available Balance " << AvailableBalance << " is currently less than Unprocessed Balance of " << UnprocessedAmount << " so need to wait.\n";
    } else {
        ofs << "Available Balance " << AvailableBalance << " is more than Unprocessed Balance of " << UnprocessedAmount << " so continuing to process.\n";
    }

    ofs << "Unknown     Transactions:         " << UnknownCount <<     ", with value of " << UnknownAmount << "\n";
    ofs << "Unprocessed Transactions:         " << UnprocessedCount << ", with value of " << UnprocessedAmount << "\n";
    ofs << "Pending     Transactions:         " << PendingCount <<     ", with value of " << PendingAmount << "\n";
    ofs << "Complete    Transactions:         " << CompleteCount <<    ", with value of " << CompleteAmount << "\n";
    ofs << "------------------------------------------------------------------------" << "\n";
    ofs << "All         Transactions:         " << CompleteCount + PendingCount + UnprocessedCount + UnknownCount <<    ", with value of " << CompleteAmount + PendingAmount + UnprocessedAmount + UnknownAmount << "\n";

    ofs << "\n";

    ofs << "Account     Amount Delay Reference                             Status \n";
 
    for (Txn = 0; Txn < numberOfTransactions; Txn++) {

        ofs << destAddress[Txn] << "    " << destAmount[Txn] << "    " << destDelay[Txn] << "     " << destPayID[Txn] << "     " << destTXRef[Txn] << "     " << destStatus[Txn] << "\n";


    }
    ofs.flush();
}


double  GetAvailableBalance()
{
    return (double) 124.56;
}


double  GetTotalBalance()
{
    return (double) 486.23587;
}

string  GetTransactionStatus ( string TransactionReference )
{
    if ( TransactionReference == "ref3") {
        return "COMPLETE";
    } else {
        return "Unprocessed";
    }
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
    long long x,y;

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

    int newPipe[2];
    pipe( newPipe );
    char TmpStr[100];

    sprintf(TmpStr,"PreFork batch_transfer: newPipe[0]=%d\n",newPipe[0]);
    LogInfo( TmpStr);
    sprintf(TmpStr,"PreFork batch_transfer: newPipe[1]=%d\n",newPipe[1]);
    LogInfo( TmpStr);

    childPID = fork();			// fork a new process
    
    if ( childPID == 0 ) {		// this code is the CHILD process, which has stdin and stdout re-directed such that the parent can interact with it
	cout << "This is the CHILD process running......\n\n";
        sprintf(TmpStr,"PostFork:child batch_transfer: newPipe[0]=%d\n",newPipe[0]);
        LogInfo( TmpStr);
        sprintf(TmpStr,"PostFork:child batch_transfer: newPipe[1]=%d\n",newPipe[1]);
        LogInfo( TmpStr);
    

	close( 0 );		// close stdin
	dup (newPipe[0]);	// duplicate read end of pipe into stdin

	close (1 );
        dup ( newPipe[1] ) ;	// duplicate write end of the pipe into stdout

        close ( newPipe[0] ) ;	// close read end of pipe
        close ( newPipe[1] ) ;	// close write end of pipe



//        close ( newPipe[1] ) ;	// stdout

	LogInfo ("This is the CHILD process running...... pipes now redirected\n\n");
	LogInfo ("This is the CHILD process running...... old pipes closed, about to execl\n\n");
       
        execl ( "/home/ubuntu/loki_testing/src/wallet_sim" ,  "wallet_sim" , NULL);

        LogInfo ("Child process ...... execve failed ...............\n" );

    } else if (childPID > 0) {

        sprintf(TmpStr,"PostFork:parent batch_transfer: newPipe[0]=%d\n",newPipe[0]);
        LogInfo( TmpStr);
        sprintf(TmpStr,"PostFork:parent batch_transfer: newPipe[1]=%d\n",newPipe[1]);
        LogInfo( TmpStr);
    

        
        // parent continues here
        sprintf(TmpStr,"This is the PARENT process running...... (child pid = %d)\n",childPID);
        LogInfo( TmpStr);

	close( 0 );		// close stdin
	dup (newPipe[1]);	// duplicate write  end of pipe into stdin

	close (1 );
        dup ( newPipe[0] ) ;	// duplicate read end of the pipe into stdout

        close ( newPipe[0] ) ;	// close read end of pipe
        close ( newPipe[1] ) ;	// close write end of pipe


        LogInfo("batch_transfer: About to issue 'wallet' text\n");

	cout << "wallet\n";

        LogInfo("batch_transfer: Issued 'wallet' text\n");

        for (x = 0; x<500000000; x++) {
          y = x * 2;
        }


        LogInfo("batch_transfer: Waiting for text....\n");

        cin >> tmpString;


        LogInfo("batch_transfer: Got response:");
        LogInfo(tmpString);
        LogInfo("\n");
        LogInfo("batch_transfer: Waiting for text....\n");

        cin >> tmpString;


        LogInfo("batch_transfer: Got response:");
        LogInfo(tmpString);
        LogInfo("\n");
        LogInfo("batch_transfer: Waiting for text....\n");

        cin >> tmpString;


        LogInfo("batch_transfer: Got response:");
        LogInfo(tmpString);
        LogInfo("\n");


	cout << "transfer amount\n";
        LogInfo("batch_transfer: Transfer command issued..... Waiting for text....\n");

        cin >> tmpString;


        sprintf(TmpStr,"batch_transfer: Got response: [%s]\n",tmpString.c_str());
        LogInfo( TmpStr);

	cout << "exit\nexit\n";
        
        LogInfo("batch_transfer: Finished, after issueing multiple exit comands.\n");


	ofs << "Printed via std::ofstream"  << endl; // will not be redirected



    }


    while ( ProcessTransactions() != 0 ) {

	ProcessTransactions();
        j++;

        if (j > 2) {
	    break;
        }

    } 


    return 0;
}

