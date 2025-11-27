#include <stdio.h>
#include <errno.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <ctype.h>


#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

// Create Database Directory Function
void createDB() {
    char dirname[] = "database";
    int check = mkdir(dirname,0777);

    //Check if directory created or exists
    if (!check) {
        return;
    } else {
        printf("Directory already exists\n");
    }
}

// Insert into Database Function
void insertDB(char accName[50], char accType[10], char accID[50], char pin[50], int accNum) {
    float balance = 0;
    char filename[20];
    FILE *fptr;
    sprintf(filename, "database/%d.txt", accNum);
    fptr = fopen(filename, "w");

    // Print account details to file
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return;
    } else {
        fprintf(fptr, "Account Number: %d\n", accNum);
        fprintf(fptr, "Name: %s\n", accName);
        fprintf(fptr, "ID: %s\n", accID);
        fprintf(fptr, "Account Type: %s\n", accType);
        fprintf(fptr, "PIN: %s\n", pin);
        fprintf(fptr, "Balance: %.2f\n", balance);
        fclose(fptr);
    }
}

// Generate Account Number Function
int generateAccNum() {
    int accNum;
    FILE *fptr;
    int exists;
    
    while (true) {
        int digits = (rand() % 3) + 7; 
        int min = 1;
        for (int i = 1; i < digits; i++) min *= 10;
        int max = min * 10 - 1;

        accNum = min + rand() % (max - min + 1);

        // Check uniqueness in index file
        exists = 0;
        fptr = fopen("database/index.txt", "r");
        if (fptr != NULL) {
            int num;
            while (fscanf(fptr, "%d", &num) == 1) {
                if (num == accNum) {
                    exists = 1;
                    break;
                }
            }
            fclose(fptr);
        }

        if (!exists) {
            // Append new account number to index
            fptr = fopen("database/index.txt", "a");
            if (fptr) {
                fprintf(fptr, "%d\n", accNum);
                fclose(fptr);
            }
            return accNum;
        }
    }
}


// Delete Bank Account Function
void deleteAcc(int accNum, char idLast4[50], char inputPin[50]) {
    char filename[20];
    FILE *fptr;
    int FullID;
    int PIN;
    char confirm;
    char line[100];
    int NUMidLast4 = atoi(idLast4);
    int NUMinputPin = atoi(inputPin);

    sprintf(filename, "database/%d.txt", accNum);
    fptr = fopen(filename, "r");
    if (!fptr) {
        printf("======================================\n");
        printf("Account not found.\n");
        printf("======================================\n");
        return;
    }

    // Verify ID and PIN
    while (fgets(line, sizeof(line), fptr)) {
        if (sscanf(line, "ID: %d", &FullID) == 1) {
            if (FullID % 10000 != NUMidLast4) {
                printf("======================================\n");
                printf("ID verification failed.\n");
                printf("======================================\n");
                fclose(fptr);
                return;
            }
        }
        if (sscanf(line, "PIN: %d", &PIN) == 1) {
            if (PIN != NUMinputPin) {
                printf("======================================\n");
                printf("PIN verification failed.\n");
                printf("======================================\n");
                fclose(fptr);
                return;
            }
        }
    }

    fclose(fptr);
    
    // Confirm deletion
    while (true) {
        printf("Are you sure you want to delete account %d? (Y/N): ", accNum); 
        scanf(" %c", &confirm);
        while ((getchar()) != '\n');
        if (tolower(confirm) == 'y') {
            sprintf(filename, "database/%d.txt", accNum);
            remove(filename);
            printf("======================================\n");
            printf("Account deleted successfully.\n");
            printf("======================================\n");
            break;
        } else if (tolower(confirm) == 'n') {
            printf("======================================\n");
            printf("Account deletion cancelled.\n");
            printf("======================================\n");
            break;
        } else {
            printf("======================================\n");
            printf("Invalid input.\n");
            printf("======================================\n");
        }
        }

    FILE *fptrIndex = fopen("database/index.txt", "r");
    FILE *fptrTemp = fopen("database/index_temp.txt", "w");
    int num;
    if (fptrIndex && fptrTemp) {
        while (fscanf(fptrIndex, "%d", &num) == 1) {
            if (num != accNum) {
                fprintf(fptrTemp, "%d\n", num);
            }
        }
        fclose(fptrIndex);
        fclose(fptrTemp);
        remove("database/index.txt");
        rename("database/index_temp.txt", "database/index.txt");
    }
}


// Deposit Function
void depositAcc(float amount, int accNum, char inputPin[50]) {
    char filename[20];
    FILE *fptr;
    float balance;
    int PIN;
    char line[100][100];
    int count = 0;
    int NUMinputPin = atoi(inputPin);
    
    sprintf(filename, "database/%d.txt", accNum);
    fptr = fopen(filename, "r");
    if (!fptr) {
        printf("======================================\n");
        printf("Account not found.\n");
        printf("======================================\n");
        return;
    }

    // Verify PIN
    while (fgets(line[count], sizeof(line[count]), fptr)) {
        if (sscanf(line[count], "PIN: %d", &PIN) == 1) {
            if (PIN != NUMinputPin) {
                printf("======================================\n");
                printf("PIN verification failed.\n");
                printf("======================================\n");
                fclose(fptr);
                return;
            }
        }
        count++;
    }
    fclose(fptr);

    for (int i = 0; i < count; i++) {
        if (sscanf(line[i], "Balance: %f", &balance) == 1) {
            balance += amount;
            sprintf(line[i], "Balance: %.2f\n", balance);
            break;
        }
    }
    
    sprintf(filename, "database/%d.txt", accNum);
    fptr = fopen(filename, "w");
    if (!fptr) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fputs(line[i], fptr);
    }

    fclose(fptr);
    printf("====================================================\n");
    printf("Deposit successful. Current balance: RM %.2f\n", balance);
    printf("====================================================\n");
}


// Withdrawal Function
void withdrawAcc(float amount, int accNum, char inputPin[50]) {
    char filename[20];
    FILE *fptr;
    float balance;
    int PIN;
    char line[100][100];
    int count = 0;
    int NUMinputPin = atoi(inputPin);
    
    sprintf(filename, "database/%d.txt", accNum);
    fptr = fopen(filename, "r");
    if (!fptr) {
        printf("Account not found.\n");
        return;
    }

    // Verify PIN
    while (fgets(line[count], sizeof(line[count]), fptr)) {
        if (sscanf(line[count], "PIN: %d", &PIN) == 1) {
            if (PIN != NUMinputPin) {
                printf("PIN verification failed.\n");
                fclose(fptr);
                return;
            }
        }
        count++;
    }
    fclose(fptr);

    // Check sufficient balance and update
    for (int i = 0; i < count; i++) {
        if (sscanf(line[i], "Balance: %f", &balance) == 1) {
            if (balance < amount) {
                printf("Insufficient balance. Current balance: RM %.2f\n", balance);
                return;
            }
            balance -= amount;
            sprintf(line[i], "Balance: %.2f\n", balance);
            break;
        }
    }
    
    sprintf(filename, "database/%d.txt", accNum);
    fptr = fopen(filename, "w");
    if (!fptr) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fputs(line[i], fptr);
    }

    fclose(fptr);

    printf("Withdraw successful. Current balance: RM %.2f\n", balance);
}

// Remittance Function
void remittanceAcc(float amount, int senderAccNum, int receiverAccNum, char inputPin[50]) {
    char senderFilename[20], receiverFilename[20];
    FILE *fptr;
    float senderBalance, receiverBalance;
    int PIN;
    char senderLine[100][100];
    char receiverLine[100][100];
    int senderCount = 0, receiverCount = 0;
    float fee = 0;
    char senderType[10], receiverType[10];
    int NUMinputPin = atoi(inputPin);
    
    sprintf(senderFilename, "database/%d.txt", senderAccNum);
    fptr = fopen(senderFilename, "r");
    if (!fptr) {
        printf("Sender account not found.\n");
        return;
    }

    // Verify PIN and get account type
    while (fgets(senderLine[senderCount], sizeof(senderLine[senderCount]), fptr)) {
        if (sscanf(senderLine[senderCount], "PIN: %d", &PIN) == 1) {
            if (PIN != NUMinputPin) {
                printf("PIN verification failed.\n");
                fclose(fptr);
                return;
            }
        }
        sscanf(senderLine[senderCount], "Account Type: %s", senderType);
        senderCount++;
    }
    fclose(fptr);

    sprintf(receiverFilename, "database/%d.txt", receiverAccNum);
    fptr = fopen(receiverFilename, "r");
    if (!fptr) {
        printf("Receiver account not found.\n");
        return;
    }

    while (fgets(receiverLine[receiverCount], sizeof(receiverLine[receiverCount]), fptr)) {
        sscanf(receiverLine[receiverCount], "Account Type: %s", receiverType);
        receiverCount++;
    }

    fclose(fptr);

    // Determine fee based on account types
    for (int i = 0; senderType[i]; i++) {
        senderType[i] = tolower(senderType[i]);
    }

    for (int i = 0; receiverType[i]; i++) {
        receiverType[i] = tolower(receiverType[i]);
    }

    if (strcmp(senderType, "savings") == 0 && strcmp(receiverType, "current") == 0) {
        fee = amount * 0.02;
    } else if (strcmp(senderType, "current") == 0 && strcmp(receiverType, "savings") == 0) {
        fee = amount * 0.03;
    } else {
        fee = 0;
    }

    // Update sender balance
    
    for (int i = 0; i < senderCount; i++) {
        if (sscanf(senderLine[i], "Balance: %f", &senderBalance) == 1) {
            if (senderBalance < amount + fee) {
                printf("Insufficient balance in sender's account. Current balance: RM %.2f\nFee is RM %.2f\n", senderBalance , fee);
                return;
            }
            senderBalance -= amount + fee;
            sprintf(senderLine[i], "Balance: %.2f\n", senderBalance);
            break;
            }
        }
    
    sprintf(senderFilename, "database/%d.txt", senderAccNum);
    fptr = fopen(senderFilename, "w");
    if (!fptr) {
        printf("Error opening sender file for writing.\n");
        return;
    }

    for (int i = 0; i < senderCount; i++) {
        fputs(senderLine[i], fptr);
    }

    fclose(fptr);

    // Update receiver's account
    sprintf(receiverFilename, "database/%d.txt", receiverAccNum);
    fptr = fopen(receiverFilename, "r");
    if (!fptr) {
        printf("Receiver account not found.\n");
        return;
    }

    receiverCount = 0;
    while (fgets(receiverLine[receiverCount], sizeof(receiverLine[receiverCount]), fptr)) {
        receiverCount++;
    }
    fclose(fptr);

    for (int i = 0; i < receiverCount; i++) {
        if (sscanf(receiverLine[i], "Balance: %f", &receiverBalance) == 1) {
            receiverBalance += amount;
            sprintf(receiverLine[i], "Balance: %.2f\n", receiverBalance);
            break;
        }
    }
    fptr = fopen(receiverFilename, "w");
    if (!fptr) {
        printf("Error opening receiver file for writing.\n");
        return;
    }
    for (int i = 0; i < receiverCount; i++) {
        fputs(receiverLine[i], fptr);
    }
    fclose(fptr);
    printf("Total deduction from account was RM %.2f = RM %.2f (WITHDRAW AMOUNT) + RM %.2f (FEE)\n", amount + fee, amount, fee);
    printf("Remittance successful. Sender's current balance: RM %.2f\n", senderBalance);
}

// Log Transaction Function
void logTransaction(const char* action, int accNum, float amount) {
    FILE *fptr;
    fptr = fopen("database/transaction.log", "a");
    if (!fptr) return;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (amount >= 0) {
        fprintf(fptr, "[%02d-%02d-%04d %02d:%02d:%02d] Action: %s | Account: %d | Amount: RM%.2f\n",
                tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                action, accNum, amount);
    } else {
        fprintf(fptr, "[%02d-%02d-%04d %02d:%02d:%02d] Action: %s | Account: %d\n",
                tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                action, accNum);
    }

    fclose(fptr);
}

int* BankAccList(int size) {
    int *accList = calloc(size, sizeof(int));
    FILE *fptr = fopen("database/index.txt", "r");
    int count = 0;
    int num;

    if (!fptr) return accList;

    while (count < size && fscanf(fptr, "%d", &num) == 1) {
        accList[count++] = num;
    }
    fclose(fptr);
    return accList;
}
