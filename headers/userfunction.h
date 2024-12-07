#ifndef _USERFUNCTIONH_
#define _USERFUNCTIONH_
#include "define.h"

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
    printf("\033[H\033[J");
    FILE *seat_file;
    struct Ticket ticket;
    char temp_seat[3], temp_char[2];
    int num1, num2, x;
    bool isFound;

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
        if (id == film.id) {
            isFound = true;
            strncpy(ticket.film_name, film.name, sizeof(film.name));
            ticket.total_price = film.price * ticket.ticket_amount;
            char filepath[] = "./data/seats/";
            strcat(filepath, film.name);
            strcat(filepath, ".txt");

            printf("\033[H\033[J");
            j = 0;
            while(j != ticket.ticket_amount) {
                seat_file = fopen(filepath, "r+");

                if (seat_file == NULL) {
                    perror("Cannot open file!\n");
                    exit(1);
                }

                printf(">> Ticket-%d\n", j + 1);
                printf(" |");
                for (i = 0; i < 7; i++) {
                    printf(" %d", row[i]);
                }
                printf("\n-|----------------\n");

                i = 0;
                char read[72];

                while (fgets(read, sizeof(read), seat_file)) {
                    printf("%c|", col[i]);
                    printf(" %s", read);
                    i++;
                }

                printf("-|----------------\n");

                printf("// Note: (o) empty, (x) reserved\n");
                printf("// Select Your Seat Column (ex: A) >> ");
                scanf("%s", temp_seat);
                num1 = convertToInt(temp_seat);
                printf("// Select Your Seat Row (ex: 1) >> ");
                scanf("%d", &num2);

                sprintf(temp_char, "%d", num2);

                x = ((num1 * 15) - 3) - ((7 - num2) * 2);
                fseek(seat_file, x, SEEK_SET);

                if (fgetc(seat_file) == 'x') {
                    printf("\033[H\033[J");
                    printf("=!!Seat is Booked!!=\n");
                } else {
                    printf("\033[H\033[J");
                    fseek(seat_file, x, SEEK_SET);
                    fwrite("x", 1, 1, seat_file);

                    strcat(temp_seat, temp_char);
                    if (j == 0) {
                        strncpy(ticket.seat_no, temp_seat, sizeof(temp_seat));
                    } else {
                        strcat(ticket.seat_no, temp_seat);
                    }

                    strcat(ticket.seat_no, ",");
                    j++;
                }

                fclose(seat_file);
            }


            int size = strlen(ticket.seat_no);
            ticket.seat_no[size - 1] = '\0';

            fprintf(
                transaction_file,
                "%s; %s; %s; %d %.2f\n",
                ticket.full_name,
                ticket.film_name,
                ticket.seat_no,
                ticket.ticket_amount,
                ticket.total_price
            );

            printf("\033[H\033[J");
            printf("// Book Info:\n");
            printf("| Full Name     : %s\n", ticket.full_name);
            printf("| Film Name     : %s\n", ticket.film_name);
            printf("| Ticket Amount : %d\n", ticket.ticket_amount);
            printf("| Total Price   : %.2f\n", ticket.total_price);
            printf("| Seat No(s)    : %s\n", ticket.seat_no);
            printf("================Data Successfully Stored================\n");
            printf("// Press any key to continue...");
            getch();
            printf("\033[H\033[J");
            break;
        } 
    }

    if (isFound != true) {
        printf("\033[H\033[J");
        printf("=!!Invalid Id!!=");
    }

    fclose(film_file);
    fclose(transaction_file);
}

void readTransaction() {
    printf("\033[H\033[J");
    FILE *file = fopen(TRANSACTION_FILE_PATH, "r");

    if (file == NULL) {
        printf("Cannot open file!\n");
        exit(1);
    }

    struct Ticket ticket;
    printf(">> Transaction(s) List");
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
    printf("\033[H\033[J");
}
#endif