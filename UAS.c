#include <stdio.h>
#include <stdbool.h>
#include "headers/adminfunction.h"
#include "headers/userfunction.h"

int choice, amount;

void adminLogin() {
    printf("\033[H\033[J");
    while (1) {
        printf("\n=======================Admin Menu=======================\n");
        printf("1. Add Films\n");
        printf("2. View Films\n");
        printf("3. View Film Seats\n");
        printf("4. Logout\n");
        printf("--------------------------------------------------------\n");
        printf("Select Menu >> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nHow much will you be adding? ");
                scanf("%d", &amount);
                addFilms(amount);
                break;
            case 2:
                readFilms(true);
                break;
            case 3:
                readFilms(false);
                printf("Select Film Id >> ");
                scanf("%d", &choice);
                viewFilmSeats(choice);
                break;
            case 4:
                printf("\033[H\033[J");
                return;
            
            default:
                printf("\033[H\033[J");
                printf("=!!Invalid Command!!=");
                break;
        }
    }
}

void userLogin() {
    printf("\033[H\033[J");
    while (1) {
        printf("\n=======================User Menu=======================\n");
        printf("1. Book Film\n");
        printf("2. View Films\n");
        printf("3. View Transactions History\n");
        printf("4. Logout\n");
        printf("--------------------------------------------------------\n");
        printf("Select Menu >> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                readFilms(false);
                printf("Select Film Id >> ");
                scanf("%d", &choice);
                addTransaction(choice);
                break;
            case 2:
                readFilms(true);
                break;
            case 3:
                readTransaction();
                break;
            case 4:
                printf("\033[H\033[J");
                return;
            
            default:
                printf("\033[H\033[J");
                printf("=!!Invalid Command!!=");
                break;
        }
    }
}

int main() {
    while (1) {
        printf("==============================================\n");
        printf("| Theatre Management System                  |\n");
        printf("==============================================\n");
        printf("Login as:\n");
        printf("1. Admin\n");
        printf("2. User\n");
        printf("3. Exit\n");
        printf("----------------------------------------------\n");
        printf("Select Option >> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                userLogin();
                break;
            case 3:
                exit(0);
                break;
            
            default:
                printf("\033[H\033[J");
                printf("=!!Invalid Command!!=\n");
                break;
        }
    }

    return 0;
}
