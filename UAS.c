#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#include <stdbool.h>
#define FILM_FILE_PATH "./data/films/film_data.txt"
#define TRANSACTION_FILE_PATH "./data/transactions/transaction_data.txt"

int choice, amount, i, j;
int row[] = {1, 2, 3, 4, 5, 6, 7};
char col[] = {'A', 'B', 'C', 'D', 'E'};
char seats[34];

struct Film {
    int id;
    char name[50];
    char release_date[50];
    float rating;
    float price;
    char genre[100];
};

struct Ticket {
    char full_name[50];
    char film_name[50];
    char seat_no[50];
    int ticket_amount;
    float total_price;
};

void addFilms(int amount) {
    struct Film film[amount];

    for (i = 0; i < amount; i++) {
        printf("\n// Data %d :\n", i + 1);
        printf("| Id                                  : ");
        scanf("%d", &film[i].id);
        printf("| Name                                : ");
        fflush(stdin);
        fgets(film[i].name, sizeof(film[i].name), stdin);
        film[i].name[strcspn(film[i].name, "\n")] = 0;
        printf("| Release Date (dd/mm/yyyy)           : ");
        scanf("%s", film[i].release_date);
        printf("| Rating                              : ");
        scanf("%f", &film[i].rating);
        printf("| Price                               : ");
        scanf("%f", &film[i].price);
        printf("| Genre (separate it using ',')       : ");
        fflush(stdin);
        fgets(film[i].genre, sizeof(film[i].genre), stdin);
        film[i].genre[strcspn(film[i].genre, "\n")] = 0;
    }

    FILE *film_file = fopen(FILM_FILE_PATH, "a");

    if (film_file == NULL) {
        printf("Cannot open file!\n");
        exit(1);
    }

    for (i = 0; i < amount; i++) {
        fprintf(
            film_file,
            "%d %s; %s %.2f %.2f %s;\n",
            film[i].id,
            film[i].name,
            film[i].release_date,
            film[i].rating,
            film[i].price,
            film[i].genre
        );
    }

    fclose(film_file);

    for (i = 0; i < amount; i++) {
        char filepath[] = "./data/seats/";
        strcat(filepath, film[i].name);
        strcat(filepath, ".txt");
        FILE *seat_file = fopen(filepath, "w");
        if (seat_file == NULL) {
            perror("Cannot open file!\n");
            exit(1);
        }

        for (i = 0; i < 5; i++) {
            fprintf(seat_file, "o o o o o o o\n");
        }

        fclose(seat_file);
    }

    printf("================Data Successfully Stored================\n\n");
}

void readFilms(bool isViewing) {
    FILE *file = fopen(FILM_FILE_PATH, "r");

    if (file == NULL) {
        printf("Cannot open file!\n");
        exit(1);
    }

    struct Film film;
    printf("\n=================================================================================\n");
    printf("| Id |        Name        | Release Date | Rating |   Price   |      Genre      |\n");
    printf("=================================================================================\n");
    while (fscanf(file, "%d %49[^;]; %s %f %f %99[^;];", &film.id, film.name, film.release_date, &film.rating, &film.price, film.genre) != EOF) {
        printf("|%-4d|%-20s|%-14s|%-8.2f|%-11.2f|%-17s|\n", film.id, film.name, film.release_date, film.rating, film.price, film.genre);
    }
    printf("=================================================================================\n");

    fclose(file);

    if (isViewing) {
        printf("// Press any key to continue...");
        getch();
    }
}

void viewFilmSeats(int id) {
    FILE *film_file = fopen(FILM_FILE_PATH, "r");
    char read[64];

    if (film_file == NULL) {
        printf("Cannot open file!\n");
        exit(1);
    }

    struct Film film;
    while (fscanf(film_file, "%d %49[^;]; %s %f %f %99[^;];", &film.id, film.name, film.release_date, &film.rating, &film.price, film.genre) != EOF) {
        if (id ==  film.id) {
            char filepath[] = "./data/seats/";
            strcat(filepath, film.name);
            strcat(filepath, ".txt");
            FILE *seat_file = fopen(filepath, "r");
            if (seat_file == NULL) {
                perror("Cannot open file!\n");
                exit(1);
            }

            printf(" |");
            for (i = 0; i < 7; i++) {
                printf(" %d", row[i]);
            }
            printf("\n-|----------------\n");

            i = 0;

            while (fgets(read, sizeof(read), seat_file)) {
                printf("%c|", col[i]);
                printf(" %s", read);
                i++;
            }

            printf("\n-|----------------\n");

            fclose(seat_file);
            printf("// Note: (o) empty, (x) reserved\n");
            printf("// Press any key to continue...");
            getch();
        } else {
            printf("=!!Invalid Id!!=\n");
        }
    }

    fclose(film_file);
}

void adminLogin() {
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
                return;
            
            default:
            printf("=!!Invalid Command!!=\n");
                break;
        }
    }
}

int convertToInt(char c[]) {
    if (strcmp(c, "A") == 0) {
        return 1;
    } else if (strcmp(c, "B") == 0) {
        return 2;
    } else if (strcmp(c, "C") == 0) {
        return 3;
    } else if (strcmp(c, "D") == 0) {
        return 4;
    } else {
        return 5;
    }
}

void addTransaction(int id) {
    FILE *seat_file;
    struct Ticket ticket;
    char temp_seat[3], temp_char[2], read[64];
    int num1, num2, x;

    printf("\n// Fill your data:\n");
    printf("| Full Name                           : ");
    fflush(stdin);
    fgets(ticket.full_name, sizeof(ticket.full_name), stdin);
    ticket.full_name[strcspn(ticket.full_name, "\n")] = 0;
    printf("| Ticket Amount                       : ");
    scanf("%d", &ticket.ticket_amount);

    FILE *film_file = fopen(FILM_FILE_PATH, "r");
    FILE *transaction_file = fopen(TRANSACTION_FILE_PATH, "a");

    if (film_file == NULL) {
        printf("Cannot open file!\n");
        exit(1);
    }

    struct Film film;
    while (fscanf(film_file, "%d %49[^;]; %s %f %f %99[^;];", &film.id, film.name, film.release_date, &film.rating, &film.price, film.genre) != EOF) {
        if (id ==  film.id) {
            strncpy(ticket.film_name, film.name, sizeof(film.name));
            ticket.total_price = film.price * ticket.ticket_amount;
            char filepath[] = "./data/seats/";
            strcat(filepath, film.name);
            strcat(filepath, ".txt");

            j = 0;
            while(j != ticket.ticket_amount) {
                seat_file = fopen(filepath, "r+");

                if (seat_file == NULL) {
                    perror("Cannot open file!\n");
                    exit(1);
                }

                printf("\n |");
                for (i = 0; i < 7; i++) {
                    printf(" %d", row[i]);
                }
                printf("\n-|----------------\n");

                i = 0;

                while (fgets(read, sizeof(read), seat_file)) {
                    printf("%c|", col[i]);
                    printf(" %s", read);
                    i++;
                }

                printf("\n-|----------------\n");

                printf("// Note: (o) empty, (x) reserved\n");
                printf("// Select Your Seat Column (ex: A) >> ");
                scanf("%s", temp_seat);
                num1 = convertToInt(temp_seat);
                printf("// Select Your Seat Row (ex: 1) >> ");
                scanf("%d", &num2);

                sprintf(temp_char, "%d", num2);
                strcat(temp_seat, temp_char);
                if (j == 0) {
                    strncpy(ticket.seat_no, temp_seat, sizeof(temp_seat));
                } else {
                    strcat(ticket.seat_no, temp_seat);
                }
                strcat(ticket.seat_no, ",");

                x = ((num1 * 15) - 3) - ((7 - num2) * 2);
                fseek(seat_file, x, SEEK_SET);
                fwrite("x", 1, 1, seat_file);

                fclose(seat_file);
                j++;
            }

            fprintf(
                transaction_file,
                "%s; %s; %s; %d %.2f\n",
                ticket.full_name,
                ticket.film_name,
                ticket.seat_no,
                ticket.ticket_amount,
                ticket.total_price
            );

            printf("================Data Successfully Stored================\n\n");
        } else {
            printf("=!!Invalid Id!!=\n");
        }
    }

    fclose(film_file);
    fclose(transaction_file);
}

void readTransaction() {
    FILE *file = fopen(TRANSACTION_FILE_PATH, "r");

    if (file == NULL) {
        printf("Cannot open file!\n");
        exit(1);
    }

    struct Ticket ticket;
    printf("\n===================================================================================\n");
    printf("|    Full Name    |     Film Name     |   Seat No   | Tick Amount |  Total Price  |\n");
    printf("===================================================================================\n");
    while (fscanf(file, "\n%49[^;]; %49[^;]; %49[^;]; %d %f", ticket.full_name, ticket.film_name, ticket.seat_no, &ticket.ticket_amount, &ticket.total_price) != EOF) {
        printf("|%-17s|%-19s|%-13s|%-13d|%-15.2f|\n", ticket.full_name, ticket.film_name, ticket.seat_no, ticket.ticket_amount, ticket.total_price);
    }
    printf("===================================================================================\n");

    fclose(file);

    printf("// Press any key to continue...");
    getch();
}

void userLogin() {
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
                return;
            
            default:
                printf("=!!Invalid Command!!=\n");
                break;
        }
    }
}

int main() {
    while (1) {
        printf("\n==============================================\n");
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
                printf("=!!Invalid Command!!=\n");
                break;
        }
    }

    return 0;
}
