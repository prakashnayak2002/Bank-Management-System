#include "BANK_header.h"

/* ===================== ATM MAIN MENU ===================== */
void ATM_MENU()
{

    while(1)
    {
        int choice;   // Stores ATM menu choice

        // Display ATM menu
        cout << "\n========== SBI ATM ==========\n";
        cout << "1. Withdraw\n";
        cout << "2. Deposit\n";
        cout << "3. Check Balance\n";
        cout << "4. Pin Change \n" ;
        cout << "0. Exit\n";
        cout << "=============================\n";
        cout << "Enter choice: ";

        // Read user choice
        cin >> choice;

        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Handle invalid input
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize> :: max(),'\n');
            cout << "Please enter the valid choice" << endl ;
            continue ;
        }

        // Bank object (not directly used here, but kept as is)
        Bank b;

        // Perform action based on choice
        switch (choice)
        {
            case 1:
                ATM_Withdraw();     // ATM withdraw operation
                break;
            case 2:
                ATM_Deposit();      // ATM deposit operation
                break;
            case 3:
                ATM_CheckBalance(); // ATM balance enquiry
                break;
            case 4:
                ATM_Pinchange();   // ATM Pin Change 
            case 0:
                cout << "Thanks for Visiting Our SBI Bank ATM" << endl ;
                return;             // Exit ATM menu
            default:
                cout << "Invalid choice\n";
        }
    }
}

/* ===================== ATM WITHDRAW ===================== */
void ATM_Withdraw()
{
    Bank sbi;              // Bank object to hold account data
    char acc[20];          // Account number
    double amt;            // Withdrawal amount
    bool locked = false;   // Account lock status flag

    // Open bank data file
    fstream file("bank.dat", ios::in | ios::out | ios::binary);
    if (!file)
    {
        cout << "File open failed\n";
        return;
    }

    // Get account number
    cout << "Enter Account Number: ";
    cin.getline(acc, sizeof(acc));

    // Find account in file
    if (!findAccount(file, sbi, acc))
    {
        cout << "❌ Account Not Found\n";
        return;
    }

    // Verify password and lock status
    if (!verifyAccountAndPassword(file, sbi, acc, locked))
        return;

    // Get withdrawal amount
    cout << "Enter Amount to Withdraw: ";
    cin >> amt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Validate amount input
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize> :: max() ,'\n');
        cout <<"Enter the valid amount " << endl;
        return ;
    }

    // Check balance sufficiency
    if (amt <= 0 || amt > sbi.balance)
    {
        cout << "❌ Invalid or Insufficient Balance\n";
        return;
    }

    // Deduct amount
    sbi.balance -= amt;

    // Move file pointer back and update record
    file.seekp(-sizeof(sbi), ios::cur);
    file.write((char*)&sbi, sizeof(sbi));

    cout << "✅ Withdraw Successful\n";
}

/* ===================== ATM DEPOSIT ===================== */
void ATM_Deposit()
{
    Bank sbi;              // Bank object
    char acc[20];          // Account number
    double amt;            // Deposit amount
    bool locked = false;   // Lock status

    // Open bank file
    fstream file("bank.dat", ios::in | ios::out | ios::binary);
    if (!file)
    {
        cout << "File open failed\n";
        return;
    }

    // Input account number
    cout << "Enter Account Number: ";
    cin.getline(acc, sizeof(acc));

    // Find account
    if (!findAccount(file, sbi, acc))
    {
        cout << "❌ Account Not Found\n";
        return;
    }

    // Verify password
    if (!verifyAccountAndPassword(file, sbi, acc, locked))
        return;

    // Input deposit amount
    cout << "Enter Amount to Deposit: ";
    cin >> amt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Validate input
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize> :: max() ,'\n');
        cout <<"Enter the valid amount " << endl;
        return ;
    }

    // Validate amount
    if (amt <= 0)
    {
        cout << "❌ Invalid Amount\n";
        return;
    }

    // Add amount
    sbi.balance += amt;

    // Update record in file
    file.seekp(-sizeof(sbi), ios::cur);
    file.write((char*)&sbi, sizeof(sbi));

    cout << "✅ Deposit Successful\n";
}

/* ===================== ATM CHECK BALANCE ===================== */
void ATM_CheckBalance()
{
    Bank sbi;             // Bank object
    char acc[20];         // Account number
    bool locked = false;  // Lock status

    // Open bank file
    fstream file("bank.dat", ios::in | ios::out | ios::binary);
    if (!file)
    {
        cout << "File open failed\n";
        return;
    }

    // Input account number
    cout << "Enter Account Number: ";
    cin.getline(acc, sizeof(acc));

    // Search account
    if (!findAccount(file, sbi, acc))
    {
        cout << "❌ Account Not Found\n";
        return;
    }

    // Verify password
    if (!verifyAccountAndPassword(file, sbi, acc, locked))
        return;

    // Display account details
    cout << "\n====== ACCOUNT DETAILS ======\n";
    cout << "Name    : " << sbi.name << endl;
    cout << "Balance : " << fixed << setprecision(2) << sbi.balance << endl;
    cout << "=============================\n";
}

void ATM_Pinchange()
{
    
    fstream file("bank.dat", ios :: binary | ios :: in | ios :: out);
    if(file.fail())
    {
        cout << "File Open Failed" << endl ;
        return ;
    }

    Bank sbi ;

    char acc[20] ;
    cout << "Enter the Bank Account Nuber" << endl ;
    cin.getline(acc,sizeof(acc));
    //cin.ignore();

    char pass[20] ;
    cout << "Enter the Old password" <<  endl ;
    cin.getline(pass,sizeof(pass));
    
    while(file.read((char*)&sbi, sizeof(sbi)))
    {
        if(strcmp(acc,sbi.account) == 0)
        {
            if(sbi.locked == 1)
            {
                cout << "The Account is Locked Contact Bank" << endl ;
                return ;
            }
            if(strcmp(pass,sbi.password) == 0)
            {
                cout << "Enter the new Password (minimum 4 character) " << endl ;
                cin.getline(pass , sizeof(pass));
                strcpy(sbi.password ,pass) ;
                file.seekp(-(sizeof(sbi)) , ios :: cur);
                file.write((char*)&sbi , sizeof(sbi));

                cout << "Password Updated Successfully" <<  endl ;
                return ;
            }
            else
            {
                cout << "Please enter the correct previous Password" << endl ;
                return ; 
            }
        }
    }
    cout << "Account Number is not Found " << endl ;
    return ;
}