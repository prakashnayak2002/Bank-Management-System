#include "BANK_header.h"

int main()
{  
    // Create admin lock file if it does not already exist
    initiadminlevelfile();

    // Startup admin security check (permanent lock / username / password)
    adminlevelpassword1();

    // Create Bank object for performing operations
    Bank sbi;

    // Variable to store user menu choice
    int choice ;

    // Infinite loop for menu-driven program
    while(true)
    {
        // Display main menu
        cout << endl ;
        cout << ".....SBI BANK MENU......" << endl ;
        cout << "  1. Creating Account" << endl;
        cout << "  2. Deposit" << endl;
        cout << "  3. Withdraw" << endl;
        cout << "  4. Balance Enqure" <<endl;
        cout << "  5. Display Acounts" <<endl;
        cout << "  6. Delete Account" << endl;
        cout << "  7. Edit Name " << endl;
        cout << "  8. ATM Menu" << endl ;
        cout << "  9. Unlock Account" << endl;
        cout << "  0. Exit" << endl;
        cout << "........................" << endl  << endl ;

        // Ask user for menu choice
        cout << "Select the choice :  ";
        cin  >> choice;

        // Clear input buffer
        cin.ignore(numeric_limits<streamsize> :: max() ,'\n');
        
        // Handle invalid (non-numeric) input
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize> :: max() ,'\n');
            cout << "Please enter the valid choice " << endl ;
            continue ;
        }

        // Exit condition
        if(choice == 0)
            break; 

        // Perform operation based on user choice
        switch(choice)
        {
            case 1 : 
                sbi.creat_Account();   // Create new bank account
                break;

            case 2 :
                sbi.Deposit();         // Deposit money
                break;

            case 3 :
                sbi.Withdraw();        // Withdraw money
                break;

            case 4 :
                sbi.Checkbalance();    // Check account balance
                break ;

            case 5 :
                sbi.Displayaccounts(); // Display all accounts (admin)
                break;

            case 6 : 
                sbi.DeleteAccount();   // Delete an account (admin)
                break;

            case 7 :
                sbi.Editname();        // Edit account holder name (admin)
                break;

            case 8:
                ATM_MENU();            // Go to ATM menu
                break;

            case 9 :
                sbi.Admin_UnlockAccount(); // Unlock locked user account
                break ;

            default : 
                cout << "Enter The valid choice "<<endl;
                break;
        }
    }

    // Exit message
    cout << "Thanks for Visiting Our SBI Bank " << endl ;

    return 0 ;
}
