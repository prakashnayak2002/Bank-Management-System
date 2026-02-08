#ifndef BANK_H
#define BANK_H

// Standard input-output stream
#include <iostream>

// File handling
#include <fstream>

// String operations
#include <cstring>

// Formatting output (setw, setprecision)
#include <iomanip>

// Input limits (numeric_limits)
#include <limits>

// Standard library (exit)
#include <stdlib.h>

// Character checks (isdigit, isalpha)
#include <ctype.h>

using namespace std;


/* ===================== FUNCTION DECLARATIONS ===================== */

// Validate account number (digits + length)
bool is_valid_account(char *account);

// Check if account number already exists
bool duplicate_accounts(char * account);

// Validate account holder name
bool is_valid_name(char * name);

// Admin authentication with permanent lock
bool adminlevelpassword();
bool adminlevelpassword1();


// Initialize admin lock file
void initiadminlevelfile();

// ATM functions
void ATM_Withdraw();
void ATM_Deposit();
void ATM_CheckBalance();
void ATM_MENU();
void ATM_Pinchange();

/* ==================================================================*/


/* ===================== ADMIN CREDENTIALS ===================== */

#define ADMIN_PASSWORD  "Prakash@2002"
#define ADMIN_USERNAME  "Prakash@2002"

/* =============================================================*/

using namespace std ;
class Bank
{
    public:
    // Account data members
    char account[20];      // Account number
    char password[20];     // Account password
    char name[30];         // Account holder name
    double balance;        // Balance amount
    int locked;            // 0 = active, 1 = locked

    /*=============================== Create Account ===================================*/
    void creat_Account()
    {
        while(true)
        {
            cout << "Enter the Account Number : " ;
            cin >> account ;
            if(!is_valid_account(account))
            {
                cout << "Please Enter Valid Account Number minumum 8 and maximum 18 digits only" << endl ;
                continue ;
            }

            if(duplicate_accounts(account) == 1)
            {
                cout <<  "The account number is allready exist " << endl;
                return ;
            } 
            break;
        }

        cin.ignore(numeric_limits<streamsize> :: max() ,'\n');

        while(true)
        {
            cout << "Enter the Account Holder Name : ";
            cin.getline(name,sizeof(name));
            if(!is_valid_name(name))
            {
                cout << "Enter The valid name " << endl;
                continue ;
            }
            break;
        }
        while(true)
        {
            cout << "Enter Initial Balance : " ;
            cin >> balance ;
            cin.ignore(numeric_limits<streamsize> :: max(),'\n');

            if(cin.fail()) //  header file include <limits>  for user invalid input instead of 1234 if user give asdf
            {
                cin.clear(); // to clear the cin
                cin.ignore(numeric_limits<streamsize> :: max(),'\n'); // act as fflush clear the input buffer
                cout <<"Please enter the numericals" << endl;
                continue;
            }

            if(balance < 0)
            {
                cout << "The Amount Should not be negative " << endl ;
                continue ;
            }
            break;
        }
        while(true)
        {
            cout << "Set Account Password (min 4 chars): " ;
            cin.getline(password,sizeof(password));

            if(strlen(password) < 4)
            {
                cout << "Password too short!" << endl;
                continue;
            }
            break;
        }

        locked  = 0 ;
        fstream file("bank.dat", ios :: app  | ios :: binary);
        if(!file.is_open())
        {
            cout << " File is Failed to open" << endl;
            return ;
        }
        file.write((char *)this , sizeof(*this));
        cout <<  "Creating Account Successfully " << endl;
        file.close();

    }

/*==================================== Deposit Function ===================================*/
    void Deposit()
    {
        char account1[20] ;
        cout << "Enter  the Account number to deposit : ";
        cin.getline(account1 , sizeof(account1));

        double balance1 ;
        cout << "Enter the amount : ";
        cin >> balance1 ;

        fstream file("bank.dat",ios :: in | ios :: out | ios :: binary );
        if(!file.is_open())
        cout << "File Fails to open : " <<  endl ;
        int flag = 0 ;

        while(file.read((char*)this , sizeof(*this)))
        {
            if(locked != 0)
            {
                cout << "❌ Account is LOCKED. Contact bank.\n";
                return;
            }
            if(strcmp(account , account1)== 0)
            {
                balance = balance + balance1 ;
                file.seekp(-(sizeof(*this)),ios::cur);
                file.write((char*)this , sizeof(*this));
                cout << "Amount Deposit successfully" <<  endl ;
                flag =1;
                break;
            }
        }
        if(flag == 0)
        {
            cout << "Invalid Account Number " << endl ;
            file.close() ;
            return ;
        }
        file.close() ;
    }

/*======================================= withdraw function =================================*/
    void Withdraw()
    {
        char acc[20]; 
        double amnt  ;

        cout << "Enter The Account Number : " ;
        cin.getline(acc,sizeof(acc));
        cout << "Enter The amount to Withdraw : "  ;
        cin >> amnt ;

        fstream file("bank.dat" ,  ios :: binary | ios :: in | ios :: out);
        if(!file.is_open())
        {
            cout << "File Failed to open " <<  endl ;
            return ;
        }
        while(file.read((char*)this , sizeof(*this)))
        {
            if(locked != 0)
            {
                cout << "❌ Account is LOCKED. Contact bank.\n";
                return;
            }
            if(strcmp(acc,account) == 0)
            {
                if(balance >= amnt)
                {
                    balance = balance - amnt ;
                    file.seekp(-sizeof(*this) , ios :: cur);
                    file.write((char*)this , sizeof(*this));
                    cout << "Amount Withdraw Successfully" << endl ;
                    return ;
                    file.close();
                }
                else
                {
                    cout << "Insufficient balance to withdraw" <<  endl ;
                    file.close();
                    return ;
                }
            }
            
        }
    }

/*======================================= Check Balance Function =================================*/

    void Checkbalance()
    {
        char acc[20];
        cout << "Enter the Account number : " ;
        cin.getline(acc,sizeof(acc));
        ifstream in("bank.dat" ,ios :: binary);
        if(in.fail())
        {
            cout << "File Failed to Open in Check Balance " <<  endl ;
            return ;
        }
        int flag = 0 ;
        while(in.read((char*)this , sizeof(*this)))
        {
            if(locked != 0)
            {
                cout << "❌ Account is LOCKED. Contact bank.\n";
                return;
            }
            if(strcmp(acc,account) == 0)
            {
                cout << "=====================================================================" << endl;
                cout << "                        Account Details                              " << endl;
                cout << "=====================================================================" << endl ;
                cout  << setw(30) << left << "NAME" << setw(25) << "ACC No." << setw(25) << "Balance" << endl;
                cout << setw(30) << left << name << setw(25) << account << setw(25) << fixed << setprecision(2)<< balance  << endl;
                cout << "=====================================================================" << endl;
                flag = 1 ;
                break;
        
            }
        }
        if(flag == 0)
        {
            cout << "Accoun Not FOUND!!" << endl;
            return ; 
        }
        in.close();
    }

/*======================================= Display Function =================================*/

    void Displayaccounts()
    {
        if(!adminlevelpassword())
        return ;

        ifstream in("bank.dat", ios::binary);
        if(!in)
        {
            cout << "ERROR: Database not accessible\n";
            return;
        }

        Bank temp;
        int sl = 1;
        bool found = false;

        cout << "\n+=========================================================================+\n";
        cout << "|                       ALL CUSTOMER ACCOUNTS                             |\n";
        cout << "+=========================================================================+\n";

        cout << "| " << setw(6)  << left << "S.No" << "| " << setw(25) << "Name" << "| " << setw(22) << "Account Number" << "| " << setw(12) << "Balance" << " |\n";

        cout << "+-------------------------------------------------------------------------+\n";

        while(in.read((char*)&temp, sizeof(temp)))
        {
            cout << "| " << setw(6)  << sl++ << "| " << setw(25) << temp.name << "| " << setw(22) << temp.account << "| " << setw(12) << fixed << setprecision(2) << temp.balance << " |\n";
            found = true;
        }

        if(!found)
        cout << "|                     No account records found                            |\n";

        cout << "+=========================================================================+\n";
        in.close();
    }

/*======================================= Unlock Function =================================*/

    void Admin_UnlockAccount()
    {
        Bank sbi;
        char acc[20];
        int found = 0;

        if (!adminlevelpassword())
            return;

        fstream file("bank.dat", ios::in | ios::out | ios::binary);
        if (!file)
        {
            cout << "File open failed\n";
            return;
        }

        cout << "Enter Account Number to Unlock: ";
        cin.getline(acc, sizeof(acc));

        while (file.read((char*)&sbi, sizeof(sbi)))
        {
            if (strcmp(sbi.account, acc) == 0)
            {
                found = 1;

                if (sbi.locked == 0)
                {
                    cout << "✅ Account is already ACTIVE\n";
                    return;
                }

                // unlock
                sbi.locked = 0;

                file.seekp(-sizeof(sbi), ios::cur);
                file.write((char*)&sbi, sizeof(sbi));

                cout << "🔓 Account UNLOCKED successfully\n";
                return;
            }
        }

        if (!found)
            cout << "❌ Account Not Found\n";

        file.close();
    }
/*======================================= Delete Function =================================*/

    void DeleteAccount()
    {
        Bank sbi;

        if (!adminlevelpassword())
            return;

        ifstream in("bank.dat", ios::binary);
        ofstream out("temp.dat", ios::binary);

        if (!in || !out)
        {
            cout << "\n❌ ERROR: Unable to access account database\n";
            return;
        }

        char acc[20];
        cout << "\n=============================================\n";
        cout << "              DELETE ACCOUNT                 \n";
        cout << "=============================================\n";
        cout << "Enter Account Number to Delete : ";
        cin.getline(acc, sizeof(acc));

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "❌ Invalid Input\n";
            return;
        }

        int found = 0;
        while (in.read((char*)&sbi, sizeof(sbi)))
        {
            if (strcmp(sbi.account, acc) == 0)
            {
                found = 1;
                continue;   // skip record (delete)
            }
            out.write((char*)&sbi, sizeof(sbi));
        }

        in.close();
        out.close();

        if (!found)
        {
            cout << "\n❌ Account Number NOT FOUND\n";
            return;
        }

        remove("bank.dat");
        rename("temp.dat", "bank.dat");

        cout << "\n✅ Account Deleted Successfully\n";
        cout << "---------------------------------------------\n";
    }

/*======================================= Edit name Function =================================*/

    void Editname()
    {
        Bank sbi;

        if (!adminlevelpassword())
            return;

        fstream file("bank.dat", ios::binary | ios::in | ios::out);
        if (!file)
        {
            cout << "\n❌ ERROR: Unable to open account database\n";
            return;
        }

        char acc[20];
        cout << "\n=============================================\n";
        cout << "              EDIT ACCOUNT NAME              \n";
        cout << "=============================================\n";
        cout << "Enter Account Number : ";
        cin.getline(acc, sizeof(acc));

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "❌ Invalid Input\n";
            return;
        }

        while (file.read((char*)&sbi, sizeof(sbi)))
        {
            if (strcmp(sbi.account, acc) == 0)
            {
                char name[30];
                cout << "Enter New Account Holder Name : ";
                cin.getline(name, sizeof(name));

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "❌ Invalid Input\n";
                    return;
                }

                strcpy(sbi.name, name);
                file.seekp(-sizeof(sbi), ios::cur);
                file.write((char*)&sbi, sizeof(sbi));

                cout << "\n✅ Name Updated Successfully\n";
                cout << "---------------------------------------------\n";
                return;
            }
        }

        cout << "\n❌ Account Number NOT FOUND\n";
    }

};

bool verifyAccountAndPassword(fstream &file,Bank &sbi,const char *acc,bool &isLocked);
bool findAccount(fstream &file, Bank &sbi, const char *acc);


#endif