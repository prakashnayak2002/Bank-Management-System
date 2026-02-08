#include "BANK_header.h"
bool is_valid_account(char *account )
{
    int len = strlen(account);

    if(len < 6 || len >18)
    {
        return false ;
    }
    
    for(int i = 0 ; account[i] != '\0' ; i++)
    {
        if(!isdigit(account[i]))
        return false ;
    }
    return true ;
}

bool duplicate_accounts(char * account)
{
    ifstream in("bank.dat" , ios :: binary);
    if(in.fail())
    {
        cout << "File Failed to open" << endl ;
        return false ;
    }
    Bank sbidupli ;
    while(in.read((char*)&sbidupli ,sizeof(sbidupli)))
    {
        if(strcmp(sbidupli.account , account) == 0)
        return true ;
    }
    return false ;

}

bool is_valid_name(char * name)
{
    int space_count = 0 ;
    int valid = 0 ;
    if(strlen(name) == 0)
    {
        return false;
    }
    for(int i = 0 ; name[i] != '\0' ; i++)
    {
        if(isalpha(name[i]))
        continue;
        else if(isspace(name[i]))
        {
            space_count++;
            if(space_count > 2 || i==0 || name[i+1] == '\0' || (name[i] == ' ' && name[i+1] == ' '))
            {
                cout << "The name Should not have space more then two" << endl ; 
                return false ;
            }
        }
        else 
        return false;
    }
    return true ;
}

bool verifyAccountAndPassword(fstream &file,Bank &sbi,const char *acc,bool &isLocked)
{
    char pass[20];
    int attempts = 3;

    // Account already read, pointer at correct position
    if (sbi.locked)
    {
        cout << "❌ Account is LOCKED. Contact bank.\n";
        isLocked = true;
        return false;
    }

    while (attempts--)
    {
        cout << "Enter Password: ";
        cin.getline(pass, sizeof(pass));

        if (strcmp(sbi.password, pass) == 0)
        {
            cout << "✅ Password Verified\n";
            return true;
        }

        cout << "❌ Wrong Password. Attempts left: "<< attempts << endl;
    }

    // Lock account after 3 failures
    sbi.locked = 1;
    file.seekp(-sizeof(sbi), ios::cur);
    file.write((char*)&sbi, sizeof(sbi));

    cout << "🚫 Account LOCKED due to 3 wrong password attempts\n";
    isLocked = true;
    return false;
}

bool findAccount(fstream &file, Bank &sbi, const char *acc)
{
    file.clear();
    file.seekg(0, ios::beg);

    while (file.read((char*)&sbi, sizeof(sbi)))
    {
        if (strcmp(sbi.account, acc) == 0)
        return true;
    }
    return false;
}

bool adminlevelpassword()
{
    fstream file("adminpassword.dat", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "File Failed to Open\n";
        return false;
    }

    int lock = 0;
    file.read((char*)&lock, sizeof(lock));

    char user[25];
    char pass[25];

    /* ===== LOCK CHECK ===== */
    if (lock == 1)
    {
        cout << "\n=============================================\n";
        cout << "        ADMIN IDENTITY VERIFICATION           \n";
        cout << "=============================================\n";

        cout << "Enter the Admin Username : ";
        cin >> user;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (strcmp(user, ADMIN_USERNAME) != 0)
        {
            cout << "\n❌ ACCESS DENIED\n";
            cout << "System Terminated for Security Reasons\n";

            file.close();
            exit(0);
        }

        cout << "\n✅ USERNAME VERIFIED\n";
        cout << "System Unlocked Successfully\n";

        lock = 0;
        file.seekp(0, ios::beg);
        file.write((char*)&lock, sizeof(lock));
    }

    /* ===== PASSWORD PANEL ===== */
    cout << "\n=====================================================\n";
    cout << "              ADMIN AUTHENTICATION PANEL             \n";
    cout << "=====================================================\n";

    int attempts = 3;
    while (attempts--)
    {
        cout << "\n🔐 Enter Admin Password : ";
        cin >> pass;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (strcmp(pass, ADMIN_PASSWORD) == 0)
        {
            cout << "\n✅ ACCESS GRANTED\n";
            cout << "-----------------------------------------------------\n";
            file.close();
            return true;
        }

        cout << "\n❌ INVALID PASSWORD";
        cout << "\nAttempts Left : " << attempts << endl;
    }

    /* ===== PERMANENT LOCK ===== */
    cout << "\n🚫 TOO MANY WRONG ATTEMPTS\n";
    cout << "SYSTEM LOCKED PERMANENTLY\n";

    lock = 1;
    file.seekp(0, ios::beg);
    file.write((char*)&lock, sizeof(lock));
    file.close();

    exit(0);
}

void initiadminlevelfile()
{
    ifstream in("adminpassword.dat", ios::binary);

    // If file already exists, do nothing
    if (in.good())
    {
        in.close();
        return;
    }

    // File does not exist → create it
    in.close();
    ofstream out("adminpassword.dat", ios::binary);

    int lock = 0;   // 0 = UNLOCKED, 1 = LOCKED
    out.write((char*)&lock, sizeof(lock));
    out.close();
}

bool adminlevelpassword1()
{
    fstream file("adminpassword.dat", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "Admin lock file open failed\n";
        exit(0);
    }

    int lock = 0;
    file.read((char*)&lock, sizeof(lock));

    char user[25];
    char pass[25];

    /* ===== SYSTEM LOCK CHECK ===== */
    if (lock == 1)
    {
        cout << "\n🚫 SYSTEM LOCKED\n";
        cout << "Enter Admin Username : ";
        cin >> user;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (strcmp(user, ADMIN_USERNAME) != 0)
        {
            cout << "\n❌ INVALID USERNAME\n";
            cout << "ACCESS DENIED\n";
            file.close();
            exit(0);
        }

        cout << "\n✅ USERNAME VERIFIED\n";
        cout << "SYSTEM UNLOCKED\n";

        lock = 0;
        file.seekp(0, ios::beg);
        file.write((char*)&lock, sizeof(lock));
    }

    /* ===== PASSWORD PANEL ===== */
    cout << "\n====================================\n";
    cout << "        ADMIN AUTHENTICATION         \n";
    cout << "====================================\n";

    int attempts = 3;
    while (attempts--)
    {
        cout << "Enter Admin Password : ";
        cin >> pass;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (strcmp(pass, ADMIN_PASSWORD) == 0)
        {
            cout << "\n✅ ACCESS GRANTED\n";
            file.close();
            return true;
        }

        cout << "❌ WRONG PASSWORD | Attempts left: "
             << attempts << endl;
    }

    /* ===== PERMANENT LOCK ===== */
    cout << "\n🚫 TOO MANY WRONG ATTEMPTS\n";
    cout << "SYSTEM LOCKED PERMANENTLY\n";

    lock = 1;
    file.seekp(0, ios::beg);
    file.write((char*)&lock, sizeof(lock));
    file.close();

    exit(0);
}

