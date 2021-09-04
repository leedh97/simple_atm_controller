#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>


using namespace std;

class Account{
public:
    Account(string _accountNumber, unsigned int _pinNumber, unsigned int _balance):
        accountNumber(_accountNumber),
        pinNumber(_pinNumber),
        balance(_balance) {}

    string GetAccountNumber(){
        return accountNumber;
    }

    unsigned int GetPinNumber(){
        return pinNumber;
    }

    void ChangeAccountNumber(string _accountNumber){
        accountNumber = _accountNumber;
    }

    void ChangePinNumber(unsigned int _pinNumber){
        pinNumber = _pinNumber;
    }

    int GetBalace(){
        return balance;
    }

    bool WithdrawMoney(const unsigned int required){
        if (balance >= required){
            balance -= required;
            return true;
        }
        else
            return false;
    }

    void DepositMoney(const unsigned int required){
        balance += required;
    }

private:
    string accountNumber;
    unsigned int pinNumber;
    int balance;
};



class Bank{
public:
    Bank(const unsigned int _account_seed) : accountSeed(_account_seed){}
    ~Bank(){}

    void CreatAccount(const unsigned int _pinNumber, const unsigned int initBalance){
        string account_number = to_string(accountSeed++);
        accounts[account_number] = new Account(account_number, _pinNumber, initBalance);

        if (map_pin_account.find(_pinNumber) != map_pin_account.end())
            map_pin_account[_pinNumber].push_back(account_number);
        else
            map_pin_account[_pinNumber] = vector<string>({account_number});
    }

    unsigned int GetAccountSeed() {
        return accountSeed;
    }

    vector<string> GetAccountList(const unsigned int _pinNumber){
        return map_pin_account[_pinNumber];
    }

    unsigned int GetBalance(const string _accountNumber){
        return accounts[_accountNumber]->GetBalace();
    }

    bool Withdraw(const string _accountNumber, const unsigned int required){
        return accounts[_accountNumber]->WithdrawMoney(required);
    }

    void Deposit(const string _accountNumber, const unsigned int required){
        return accounts[_accountNumber]->DepositMoney(required);
    }

private:
    map<unsigned int, vector<string>> map_pin_account;
    map<string, Account*> accounts;
    unsigned int accountSeed;
};


enum MENU_STEPS {STEP_Quit, STEP_Init, STEP_Pin, STEP_Account, STEP_Service};

class ATM{
public:
    ATM(Bank* _bank) : bank(_bank) {}

    int WelcomeAndInitMenu(unsigned int &pin_number){
        string inStr;
        string account_number;
        int idx;

        cout << "#####################################################" << endl;
        cout << "###                Welcome to myBank              ###" << endl;
        cout << "#####################################################" << endl;
        cout << endl << endl << endl;
        cout << "## A pin number is 10000~10599(If you want to quit program, input q)" << endl;
        cout << "## Insert card/input pin numbers : " << endl;        
        cin  >> inStr;

        if (inStr == "q"){
            cout << endl << endl << endl << endl << endl;
            return STEP_Quit;
        }

        while (1){
            pin_number = stoi(inStr);
            if (pin_number >= 10000 && pin_number < 10600){
                cout << endl << endl << endl;
                return STEP_Pin;
            }
            else {
                cout << "## !!! Wrong pin number !!! " << endl; 
                cout << "## A pin number is 10000~10599(If you want to quit program, input q)" << endl;
                cout << "## Insert card/input pin numbers : " << endl;   
                cin  >> inStr; 
            }
        }

        cout << endl << endl << endl << endl << endl;
        return STEP_Quit;
    }


    int CheckPinAndGetAccountNumber(const unsigned int pin_number, string &account_number){
        string inStr;
        int idx;

        vector<string> accounts = bank->GetAccountList(pin_number);
        if (accounts.size() > 1){
            cout << "## Your accounts : " << endl;
            cout << "## [0] : Previous menu " << endl;   
            for (int i=0; i<accounts.size(); ++i)
                cout << "## [" << i + 1 << "] : " << accounts[i] << endl;
            cout << endl;

            while(1){
                cout << "## Select account from 0 to " << accounts.size() << endl; 
                
                cin >> inStr;
                if (inStr == "q"){
                    cout << endl << endl << endl << endl << endl;
                    return STEP_Quit;
                }

                idx = stoi(inStr);
                if (idx < 0 || idx > accounts.size())
                    cout << "## !!! Wrong input !!! " << endl;
                else{                    
                    if (idx == 0){
                        cout << endl << endl << endl;
                        return STEP_Init;
                    }

                    account_number = accounts[idx-1];

                    cout << endl << endl << endl;
                    return STEP_Account;                   
                }
            }
        }
        else{
            account_number = accounts[0];

            cout << endl << endl << endl;
            return STEP_Account;  
        }
    }


    int GetServiceNumber(const string account_number, int &service_num){
        string inStr;

        cout << "## Current Balance in account " << account_number << ": $" << bank->GetBalance(account_number) << endl;
        while(1){
            cout << "## Select what you want (If you want to quit program, input q)" << endl;
            cout << "## 0 : Prev. menu " << endl;   
            cout << "## 1 : Deposit money " << endl;   
            cout << "## 2 : Withdraw money " << endl;   
            
            cin >> inStr;
            if (inStr == "q"){
                cout << endl << endl << endl << endl << endl;
                return STEP_Quit;
            }

            service_num = stoi(inStr);
            if (service_num < 0 || service_num > 2)
                cout << "## !!! Wrong input !!! " << endl;
            else{
                if (service_num == 0){
                    cout << endl << endl << endl;
                    return STEP_Pin;
                }
                else{
                    cout << endl << endl << endl;
                    return STEP_Service;
                }
            }
        }

        cout << endl << endl << endl << endl << endl;
        return STEP_Quit;
    }

    int ProvideService(const string account_number, int service_num){
        string inStr;
        int money;
        switch (service_num){
            case 1:
                while (1){
                    cout << "## Input money to deposit (possible: $ 0 ~ $ 10000): " << endl; 
                    cin >> inStr;
                    if (inStr == "q"){
                        cout << endl << endl << endl << endl << endl;
                        return STEP_Quit;
                    }

                    money = stoi(inStr);
                    if (money < 0 || money > 10000) 
                        cout << "## !!! Wrong input !!! " << endl;
                    else 
                        break;
                }

                bank->Deposit(account_number, money);
                cout << "## Current Balance in the account " << account_number << ": $" << bank->GetBalance(account_number) << endl;        
                break;    

            case 2:
                while (1){
                    unsigned int max_withdraw = bank->GetBalance(account_number);
                    cout << "## Input money to withdraw (possible: $ 0 ~ $ " << max_withdraw << "): " << endl; 
                    cin >> inStr;
                    if (inStr == "q"){
                        cout << endl << endl << endl << endl << endl;
                        return STEP_Quit;
                    }

                    money = stoi(inStr);
                    if (money < 1 || money > max_withdraw) 
                        cout << "## !!! Wrong input !!! " << endl;
                    else 
                        break;
                }

                bank->Withdraw(account_number, money) ;
                cout << "## Current Balance in account " << account_number << ": $" << bank->GetBalance(account_number) << endl;
                break;
        }    
        
        cout << endl << endl << endl;
        int idx;
        while (1){
            cout << "## Select Menu(If you want to quit program, input q)" << endl;
            cout << "## 0 : Prev. menu " << endl;   
            cout << "## 1 : Init. menu " << endl;   
            cin >> inStr;

            if (inStr == "q"){
                cout << endl << endl << endl << endl << endl;
                return STEP_Quit;
            }

            idx = stoi(inStr);
            if (idx == 0){
                cout << endl << endl << endl;
                return STEP_Account;
            }
            else if (idx == 1){
                cout << endl << endl << endl;
                return STEP_Init;
            }
            else
                cout << "## !!! Wrong input !!! " << endl;
        }

        cout << endl << endl << endl << endl << endl;
        return STEP_Quit;

    }

private:
    Bank *bank;
};


int main(){
    srand((unsigned int)time(NULL));

    Bank *myBank = new Bank(10000);
    //Generate 1000 accounts;
    //There are 600 pin numbers(10000~10599)
    //Each account has balance of 0~1000000 dollars

    for (int i=1000; i<2000; ++i){
        myBank->CreatAccount(rand() % 600 + 10000, rand() % 1000000);
    }

    ATM atm(myBank);

    
    int curr_step = STEP_Init;

    string inStr;
    unsigned int pin_number;
    string account_number;
    bool quitProgram = false;
    int service_num;

    while(1){        
        switch (curr_step){
            case STEP_Init:
                curr_step = atm.WelcomeAndInitMenu(pin_number);
                break;
            case STEP_Pin:
                curr_step = atm.CheckPinAndGetAccountNumber(pin_number, account_number);
                break;
            case STEP_Account:
                curr_step = atm.GetServiceNumber(account_number, service_num);
                break;
            case STEP_Service:
                curr_step = atm.ProvideService(account_number, service_num);
                break;
        }

        if (curr_step == STEP_Quit)
            break;
    }

    return 0;
}

