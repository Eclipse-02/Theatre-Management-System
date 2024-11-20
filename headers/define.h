#ifndef _DEFINEH_
#define _DEFINEH_
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#define FILM_FILE_PATH "./data/films/film_data.txt"
#define TRANSACTION_FILE_PATH "./data/transactions/transaction_data.txt"

int i, j;
int row[] = {1, 2, 3, 4, 5, 6, 7};
char col[] = {'A', 'B', 'C', 'D', 'E'};

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

#endif