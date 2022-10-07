#include "user_input.h"
#include "matrix_menu.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


static void getstr(char **str);


int get_menu_selection(struct htable *table)
{
	char *str;
	int ret;

	getstr(&str);
	ret = htable_at(table, str);
	free(str);

	return ret;
}


void get_matrix_data(struct matrix *mat)
{
	char *str;
	char *tokstr;
	char *save;
	char *token;
	char *ptr;
	int i = 0;
	int num = mat->rows * mat->cols;

	assert(mat);
	assert(num > 0);

	while (i != num) {
		//first = 1;
		printf("Enter the Matrix values by row\n");

		getstr(&str);
		tokstr = str;

		for (; ((token = strtok_r(tokstr, " ", &save)) != NULL) && (i <= num);
							       i++, tokstr = NULL) {
			mat->mat[i] = strtol(token, &ptr, 10);
		}

		if (i != num) {
			printf("Error, wrong number of values input for Matrix"
							 " size. Try again\n");
			free(str);
			i = 0;
		}
	}

	free(str);
}


void get_matrix_dim(int *dim, const char *prompt)
{
	char *str;
	char *ptr;
	int failed = 0;

	printf("%s\n", prompt);

	while (1) {
		if (failed)
			printf("Error, try again.\n");

		getstr(&str);

		if ((*dim = strtol(str, &ptr, 10)) < 1) {
			failed = 1;
			free(str);
		}
		else
			break;
	}

	free(str);
}


static void getstr(char **str)
{
	int c, i = 0, len = 2;

	assert(str);

	fflush(stdin);

	*str = malloc(len * sizeof(char));
	assert(*str);

	while (((c = getc(stdin)) != '\n') && (c != EOF)) {
		if (i > len - 2) {
			len *= 2;
			*str = realloc(*str, len * sizeof(char));
			assert(*str);
		}

		(*str)[i] = c;
		i++;
	}

	(*str)[i] = '\0';
}

















