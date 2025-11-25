#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "DBFunct.h"

// Global Variables
char input[30];
int size = 200;
int accNum;
char pin[50];
bool isNum;

// Create Bank Account Function
void createAccount() {
    char accName[50];
    char accType[10];
    char accID[50];
    int c;

    printf("======================================\n");
    printf("Create Bank Account\n");
    printf("======================================\n");
    printf("Enter '0' to return to menu...\n");

    //Name input
    printf("Enter your name: ");
    scanf("%49s", accName);
    if (strcmp(accName, "0") == 0) {
        while (getchar() != '\n'); 
        return;
    } 

    //ID number input
    while (true) {
        isNum = true;

        printf("Enter your 8-digit ID Number: ");
        if (scanf("%s", &accID) != 1 || strlen(accID) != 8) {
            printf("=============================================================\n");
            printf("Invalid ID input. Must be 8 digit number.\n"); 
            printf("=============================================================\n");
            while (getchar() != '\n');
        } else {
            for (int i = 0; i < 8; i++) {
                if (!isdigit(accID[i])){
                    isNum = false;
                    break;
                }
            }
            if (isNum) {
                while (getchar() != '\n');
                break;
            } else {
                printf("=============================================================\n");
                printf("Enter numbers only.\n"); 
                printf("=============================================================\n");
                continue;
            }
        }
    }

    //Account type input
    while (true) {
        printf("Type of account (Savings or Current): ");
        if (scanf("%9s", accType) != 1) {
            printf("=============================================\n");
            printf("Invalid account type input.\n");
            printf("=============================================\n");
            while (getchar() != '\n'); 
        } else {
            for (c = 0; accType[c]; c++) {
                accType[c] = tolower(accType[c]);
            }
            if (strcmp(accType, "savings") == 0) {
                strcpy(accType, "Savings");
                break;
            } else if (strcmp(accType, "current") == 0) {
                strcpy(accType, "Current");
                break;
            } else {
                printf("=============================================\n");
                printf("Invalid account type input.\n");
                printf("=============================================\n");
                while (getchar() != '\n'); 
            }
        }
    }

    //PIN input
    while (true) {
        isNum = true;
        
        printf("Four-digit PIN: ");
        if (scanf("%s", &pin) != 1 || strlen(pin) != 4) {
            printf("=======================================================\n");
            printf("Invalid PIN input. Must be 4 digits.\n");
            printf("=======================================================\n");
            while (getchar() != '\n'); 
        } else {
            for (int i = 0; i < 4; i++) {
                if (!isdigit(pin[i])){
                    isNum = false;
                    break;
                }
            }
            if (isNum) {
                while (getchar() != '\n');
                break;
            } else {
                printf("=============================================================\n");
                printf("Enter numbers only.\n");
                printf("=============================================================\n");
                continue;
            }
        }
    } 

    //Generate Account Number
    accNum = generateAccNum();

    //Insert into database
    insertDB(accName, accType, accID, pin, accNum);

    //Log the transaction
     logTransaction("Create Account", accNum, 0);

    printf("=============================================\n");
    printf("Account created successfully!\n");
    printf("Name: %s\n", accName);
    printf("ID: %s\n", accID);
    printf("Account Type: %s\n", accType);
    printf("PIN: %s\n", pin);
    printf("Account Number: %d\n", accNum);
    printf("Balance: RM 0.00\n");
    printf("=============================================\n");
}

// Delete Bank Account Function
void deleteAccount() {
    char idLast4[50];
    int* accList = BankAccList(size);
    bool exist = false;

    // List of existing accounts
    while (true) {
        printf("======================================\n");
        printf("Bank Account List:\n");
        int count = 0;
        while (accList[count] != 0) count++;
        for (int i = 0; i < count; i++) {
            printf("%d. %d\n", i + 1, accList[i]);
        }
        printf("======================================\n");
        printf("Enter '0' to return to menu...\n");

        //Account number input
        printf("Enter your bank account number: ");
        if (scanf("%d", &accNum) != 1) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else if (accNum == 0) {
            while (getchar() != '\n');
            return;
        } 
        else if (accNum < 1000000 || accNum > 999999999) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else {
            while (getchar() != '\n'); 
            for (int i = 0; i < count; i++){
                if (accNum == accList[i])
                    exist = true;
            }
            if (exist) {
                break;
            } else {
                printf("======================================\n");
                printf("Account Number doesn't exist.\n");
                printf("======================================\n");
            }
        }
    }

    // ID last 4 digits input
    while (true) {
        isNum = true;

        printf("Enter last four digits of ID Number: ");
        if (scanf("%s", &idLast4) != 1 || strlen(idLast4) != 4) {
            printf("======================================\n");
            printf("Invalid ID input. Enter 4 digit number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } else {
            for (int i = 0; i < 4; i++) {
                if (!isdigit(idLast4[i])) {
                    isNum = false;
                    break;
                }
            }
            if(isNum) {
                while (getchar() != '\n'); 
                break;
            } else {
                printf("=============================================================\n");
                printf("Enter numbers only.\n");
                printf("=============================================================\n");
                continue;
            }
        }
    }

    // PIN input
    while (true) {
        isNum = true;

        printf("Enter your PIN Number: ");
        if (scanf("%s", &pin) != 1 || strlen(pin) != 4) {
            printf("=======================================================\n");
            printf("Invalid PIN input. Must be 4 digits.\n");
            printf("=======================================================\n");
            while (getchar() != '\n'); 
        } else {
            for (int i = 0; i < 4; i++) {
                if (!isdigit(pin[i])){
                    isNum = false;
                    break;
                }
            }
            if (isNum) {
                while (getchar() != '\n');
                break;
            } else {
                printf("=============================================================\n");
                printf("Enter numbers only.\n");
                printf("=============================================================\n");
                continue;
            }
        }
    }

    // Delete account
    deleteAcc(accNum, idLast4, pin);
}


// Deposit Function
void deposit() {
    float amount;

    printf("======================================\n");
    printf("Deposit Function\n");
    printf("======================================\n");
    printf("Enter '0' to return to menu...\n");

    // Account number input
    while (true) {
        printf("Enter your bank account number: ");
        if (scanf("%d", &accNum) != 1) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else if (accNum == 0) {
            while (getchar() != '\n');
            return;
        } 
        else if (accNum < 1000000 || accNum > 999999999) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else {
            while (getchar() != '\n');
            break;
        }
    }

    // PIN input
    while (true) {
        isNum = true;

        printf("Enter your PIN Number: ");
        if (scanf("%s", &pin) != 1 || strlen(pin) != 4) {
            printf("=======================================================\n");
            printf("Invalid PIN input. Must be 4 digits.\n");
            printf("=======================================================\n");
            while (getchar() != '\n'); 
        } else {
            for (int i = 0; i < 4; i++) {
                if (!isdigit(pin[i])){
                    isNum = false;
                    break;
                }
            }
            if (isNum) {
                while (getchar() != '\n');
                break;
            } else {
                printf("=============================================================\n");
                printf("Enter numbers only.\n");
                printf("=============================================================\n");
                continue;
            }
        }
    }

    // Amount input
    while (true) {
        printf("Amount to deposit: ");
        if (scanf("%f", &amount) != 1 || amount <= 0 || amount > 50000) {
            printf("===========================================================\n");
            printf("Invalid amount input. Must be in between RM1 to RM50000.\n");
            printf("===========================================================\n");
            while (getchar() != '\n'); 
        } else {
            while (getchar() != '\n');
            break;
        }
    }

    // Deposit into account
    depositAcc(amount, accNum, pin);

    // Log the transaction
    logTransaction("Deposit", accNum, amount);
}

// Withdrawal Function
void withdrawal() {
    float amount;

    printf("======================================\n");
    printf("Withdrawal Function\n");
    printf("======================================\n");
    printf("Enter '0' to return to menu...\n");

    // Account number input
    while (true) {
        printf("Enter your bank account number: ");
        if (scanf("%d", &accNum) != 1) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else if (accNum == 0) {
            while (getchar() != '\n');
            return;
        } 
        else if (accNum < 1000000 || accNum > 999999999) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else {
            while (getchar() != '\n');
            break;
        }
    }

    // PIN input
    while (true) {
        isNum = true;

        printf("Enter your PIN Number: ");
        if (scanf("%s", &pin) != 1 || strlen(pin) != 4) {
            printf("=======================================================\n");
            printf("Invalid PIN input. Must be 4 digits.\n");
            printf("=======================================================\n");
            while (getchar() != '\n'); 
        } else {
            for (int i = 0; i < 4; i++) {
                if (!isdigit(pin[i])){
                    isNum = false;
                    break;
                }
            }
            if (isNum) {
                while (getchar() != '\n');
                break;
            } else {
                printf("=============================================================\n");
                printf("Enter numbers only.\n");
                printf("=============================================================\n");
                continue;
            }
        }
    }

    // Amount input
    while (true) {
        printf("Amount to withdraw: ");
        if (scanf("%f", &amount) != 1 || amount <= 0) {
            printf("===========================================================\n");
            printf("Invalid amount input. Must be greater than RM0.\n");
            printf("===========================================================\n");
            while (getchar() != '\n'); 
        } else {
            while (getchar() != '\n');
            break;
        }
    }

    // Withdraw from account
    withdrawAcc(amount, accNum, pin);
    // Log the transaction
    logTransaction("Withdrawal", accNum, amount);
}

// Remittance Function
void remittance() {
    float amount;
    int senderAccNum;
    int receiverAccNum;

    printf("======================================\n");
    printf("Remittance Function\n");
    printf("======================================\n");
    printf("Enter '0' to return to menu...\n");


    // Sender Account Number input
    while (true) {
        printf("Sender Account Number: ");

        if (scanf("%d", &senderAccNum) != 1) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else if (senderAccNum == 0) {
            while (getchar() != '\n');
            return;
        } 
        else if (senderAccNum < 1000000 || senderAccNum > 999999999) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else {
            while (getchar() != '\n');
            break;
        }
    }

    // PIN input
    while (true) {
        isNum = true;

        printf("Enter your PIN Number: ");
        if (scanf("%s", &pin) != 1 || strlen(pin) != 4) {
            printf("=======================================================\n");
            printf("Invalid PIN input. Must be 4 digits.\n");
            printf("=======================================================\n");
            while (getchar() != '\n'); 
        } else {
            for (int i = 0; i < 4; i++) {
                if (!isdigit(pin[i])){
                    isNum = false;
                    break;
                }
            }
            if (isNum) {
                while (getchar() != '\n');
                break;
            } else {
                printf("=============================================================\n");
                printf("Enter numbers only.\n");
                printf("=============================================================\n");
                continue;
            }
        }
    }

    // Receiver Account Number input
    while (true) {
        printf("Receiver Account Number: ");
        if (scanf("%d", &receiverAccNum) != 1) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else if (receiverAccNum < 1000000 || receiverAccNum > 999999999) {
            printf("======================================\n");
            printf("Invalid account number.\n");
            printf("======================================\n");
            while (getchar() != '\n');
        } 
        else {
            while (getchar() != '\n');
            break;
        }
    }

    if (senderAccNum == receiverAccNum) {
        printf("=======================================================================\n");
        printf("Sender Account number cannot be the same as Receiver Account Number.\n");
        printf("=======================================================================\n");
        return;
    }

    // Amount input
    while (true) {
        printf("Amount to remit: ");
        if (scanf("%f", &amount) != 1 || amount <= 0) {
            printf("======================================\n");
            printf("Invalid amount input.\n");
            printf("======================================\n");
            while (getchar() != '\n'); 
        } else {
            while (getchar() != '\n');
            break;
        }
    }

    // Remit amount
    remittanceAcc(amount, senderAccNum, receiverAccNum, pin);
    // Log the transaction
    logTransaction("Remittance", senderAccNum, amount);
}


// Check Menu Input Function
bool checkInput(char *input) {
    char *checkList[] = {"1", "create", "2", "delete", "3", "deposit", "4", "withdrawal", "5", "remittance", "6", "exit"};

    for (int i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }
    for (int i = 0; i < sizeof(checkList)/sizeof(checkList[0]); i++) {
        if (strcmp(input, checkList[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Menu Function
char* menu() {
    // Display menu and get user input
    while (true){
        printf("======================================\n");
        printf("            BANKING SYSTEM            \n");
        printf("======================================\n");
        printf("1. Create New Bank Account\n");
        printf("2. Delete Bank Account\n");
        printf("3. Deposit\n");
        printf("4. Withdrawal\n");
        printf("5. Remittance\n");
        printf("6. Exit\n");
        printf("======================================\n");
        printf("Select one function: ");
        scanf("%s", input);
        while ((getchar()) != '\n'); 
        
        if (checkInput(input)) {
            return input;
        } else {
            printf("======================================\n");
            printf("Invalid input. Please try again.\n");
            printf("======================================\n");
        }
    }
}

// Show Session Information Function
void showSessionInfo() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int* accList = BankAccList(size);
    int count = 0;
    while (count < size && accList[count] != 0) count++;

    printf("======================================\n");
    printf("Banking System Session\n");
    printf("Date/Time: %02d-%02d-%04d %02d:%02d:%02d\n",
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
           tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Loaded accounts: %d\n", count);
    printf("======================================\n");

}


// Main Function
int main() {
    srand(time(NULL));
    // Initialize database directory
    createDB();
    // Show session information
    showSessionInfo();
    
    // Main loop
    while (true) {
        menu();
        if (strcmp(input, "1") == 0 || strcmp(input, "create") == 0) {
            createAccount();
        } else if (strcmp(input, "2") == 0 || strcmp(input, "delete") == 0) {
            deleteAccount();
        } else if (strcmp(input, "3") == 0 || strcmp(input, "deposit") == 0) {
            deposit();
        } else if (strcmp(input, "4") == 0 || strcmp(input, "withdrawal") == 0) {
            withdrawal();
        } else if (strcmp(input, "5") == 0 || strcmp(input, "remittance") == 0) {
            remittance();
        } else if (strcmp(input, "6") == 0 || strcmp(input, "exit") == 0) {
            printf("======================================\n");
            printf("Exiting the program.\n");
            printf("======================================\n");
            break;
        }   
    }
    return 0;
}

