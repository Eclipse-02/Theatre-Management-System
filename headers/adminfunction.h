#ifndef _ADMINFUNCTIONH_
#define _ADMINFUNCTIONH_
#include "define.h"

void addFilms(int amount) {
    struct Film film[amount];

    for (i = 0; i < amount; i++) {
        printf("\033[H\033[J");
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

    printf("\033[H\033[J");
    for (i = 0; i < amount; i++) {
        printf("\n// Film-%d Info:\n", i+1);
        printf("| Id: %d\n", film[i].id);
        printf("| Name: %s\n", film[i].name);
        printf("| Release Date: %s\n", film[i].release_date);
        printf("| Rating: %.2f\n", film[i].rating);
        printf("| Price: %.2f\n", film[i].price);
        printf("| Genre(s): %s\n", film[i].genre);
    }
    printf("================Data Successfully Stored================\n");
    printf("// Press any key to continue...");
    getch();
    printf("\033[H\033[J");
}

void readFilms(bool isViewing) {
    printf("\033[H\033[J");
    FILE *file = fopen(FILM_FILE_PATH, "r");

    if (file == NULL) {
        printf("Cannot open file!\n");
        exit(1);
    }

    struct Film film;
    printf(">> Film(s) List");
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
        printf("\033[H\033[J");
    }
}

void viewFilmSeats(int id) {
    printf("\033[H\033[J");
    FILE *film_file = fopen(FILM_FILE_PATH, "r");
    char read[64];
    char filmName[50];
    bool isFound;

    if (film_file == NULL) {
        printf("Cannot open file!\n");
        exit(1);
    }

    struct Film film;
    while (fscanf(film_file, "%d %49[^;]; %s %f %f %99[^;];", &film.id, film.name, film.release_date, &film.rating, &film.price, film.genre) != EOF) {
        if (id ==  film.id) {
            isFound = true;
            strcpy(filmName, film.name);
            char filepath[] = "./data/seats/";
            strcat(filepath, film.name);
            strcat(filepath, ".txt");
            FILE *seat_file = fopen(filepath, "r");
            if (seat_file == NULL) {
                perror("Cannot open file!\n");
                exit(1);
            }

            printf(">> %s\n", filmName);
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

            printf("-|----------------\n");

            fclose(seat_file);
            printf("// Note: (o) empty, (x) reserved\n");
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
}
#endif
