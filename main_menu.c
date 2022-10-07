#include "main_menu.h"
#include "htable.h"
#include "user_input.h"
#include "matrix_menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define INVALID -1
#define EXIT 0
#define MATRIX_MENU 1

#define NUM_OPTIONS 2
#define HTABLE_SIZE 5

static const struct key_val options[NUM_OPTIONS] = { {"e", EXIT},
						     {"m", MATRIX_MENU},
};


static inline void greeting();
static inline void show_menu();


void main_menu()
{
	struct htable table;
	int choice;

	htable_init(&table, HTABLE_SIZE);
	htable_add_arr(&table, options, NUM_OPTIONS);

	greeting();

	while (1) {
		show_menu();
		choice = get_menu_selection(&table);

		switch (choice) {
		case EXIT :
			exit(0);
		case MATRIX_MENU :
			matrix_menu();
			break;
		case INVALID :
			printf("Invalid selection. Try again...\n");
			continue;
		}
	}
out:
	htable_free(&table);
}


static inline void greeting()
{
	printf("Welcome to my Matrix Program!\n");
}


static inline void show_menu()
{
	printf("Select an option from the Main Menu\n"
	       "(e) to Exit\n"
	       "(m) to go to the Matrix Menu\n");
}





