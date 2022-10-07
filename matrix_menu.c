#include "matrix_menu.h"
#include "htable.h"
#include "user_input.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define INVALID -1
#define EXIT 0
#define BACK_TO_MAIN_MENU 1
#define ADD 2
#define SUB 3
#define MUL 4
#define TRANSPOSE 5
#define ELIMINATE 6

#define NUM_OPTIONS 7
#define HTABLE_SIZE 11

// when adding an option define a new macro and bump NUM_OPTIONS
static const struct key_val options[NUM_OPTIONS] = { {"e", EXIT},
						     {"b", BACK_TO_MAIN_MENU},
						     {"add", ADD},
						     {"sub", SUB},
						     {"mul", MUL},
						     {"tr", TRANSPOSE},
						     {"elim", ELIMINATE},
};


static void matrix_init(struct matrix *mat, const int num)
{
	assert(num > 0);

	mat->mat = calloc(num, sizeof(float));
	assert(mat->mat);
}


static void matrix_free(struct matrix *mat)
{
	free(mat->mat);
}


static void matrix_show(const struct matrix *mat)
{
	assert(mat);
	assert(mat->cols > 0);
	assert(mat->rows > 0);

	for (int i = 0; i < mat->rows; i++) {
		for (int j = 0; j < mat->cols; j++) {
			printf("%f ", mat->mat[i * mat->cols + j]);
		}
		printf("\n");
	}
}

static inline void swap_floats(float *a, float *b)
{
	float tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}


/*
 * Gets data for a single matrix. Used for
 * transpose, LU Decomp, and Elimination.
 */
static void matrix_user_input_1(struct matrix *a)
{
	int size;

	get_matrix_dim(&(a->rows), "Enter number of rows");
	get_matrix_dim(&(a->cols), "Enter number of columns");

	size = a->rows * a->cols;

	matrix_init(a, size);
	get_matrix_data(a);
}


/*
 * Gets data for two matrices.  Used for add and sub.
 */
static void matrix_user_input_2(struct matrix *a, struct matrix *b)
{
	int size;

	get_matrix_dim(&(a->rows), "Enter number of rows");
	get_matrix_dim(&(a->cols), "Enter number of columns");

	b->rows = a->rows;
	b->cols = a->cols;

	size = a->rows * a->cols;

	matrix_init(a, size);
	matrix_init(b, size);

	get_matrix_data(a);
	get_matrix_data(b);
}


static void matrix_add_arr(struct matrix *a, const struct matrix *b)
{
	int size;

	assert(a->rows == b->rows);
	assert(a->cols == b->cols);

	size = a->rows * a->cols;
	for (int i = 0; i < size; i++) 
		a->mat[i] += b->mat[i];
}


static void matrix_sub_arr(struct matrix *a, const struct matrix *b)
{
	int size;

	assert(a->rows == b->rows);
	assert(a->cols == b->cols);

	size = a->rows * a->cols;
	for (int i = 0; i < size; i++) 
		a->mat[i] -= b->mat[i];
}


static void matrix_add()
{
	struct matrix a;
	struct matrix b;

	matrix_user_input_2(&a, &b);
	matrix_add_arr(&a, &b);
	matrix_show(&a);
	matrix_free(&a);
	matrix_free(&b);
}


static void matrix_sub()
{
	struct matrix a;
	struct matrix b;

	matrix_user_input_2(&a, &b);
	matrix_sub_arr(&a, &b);
	matrix_show(&a);
	matrix_free(&a);
	matrix_free(&b);
}


static void matrix_transpose_arr(struct matrix *mat)
{
	for (int i = 0; i < mat->rows - 1; i++) {
		for (int j = 0; j < mat->cols; j++)
			swap_floats(&(mat->mat[i * mat->cols + j]),
				    &(mat->mat[j * mat->rows + i]));
	}

	swap_floats(&(mat->rows), &(mat->cols));
}


static void matrix_transpose()
{
	struct matrix mat;

	matrix_user_input_1(&mat);
	matrix_transpose_arr(&mat);
	matrix_show(&mat);
	matrix_free(&mat);
}


static void matrix_user_input_multiply(struct matrix *a, struct matrix *b)
{
	int success = 0;

	do {
		matrix_user_input_1(a);
		matrix_user_input_1(b);

		if (a->cols == b->rows)
			success = 1;
		else {
			printf("Error, can't multiply Matrices of these "
					"dimensions\n");
		}
	} while (!success);
}


static void matrix_multiply_arr(const struct matrix *a,
				const struct matrix *b,
				struct matrix *c)
{
	assert(a);
	assert(b);
	assert(c);

	for (int i = 0; i < a->rows; i++) {
		for (int j = 0; j < b->cols; j++) {
			for (int k = 0; k < a->cols; k++) {
				c->mat[i * b->cols + j]
					+= a->mat[i * a->cols + k]
					* b->mat[k * b->cols + j];
			}
		}
	}
}


static void matrix_multiply()
{
	struct matrix a;
	struct matrix b;
	struct matrix res;

	matrix_user_input_multiply(&a, &b);

	res.rows = a.rows;
	res.cols = b.cols;
	matrix_init(&res, res.rows * res.cols);

	matrix_multiply_arr(&a, &b, &res);
	matrix_show(&res);
	matrix_free(&a);
	matrix_free(&b);
	matrix_free(&res);
}


/*
 * returns - 0 if successful, -1 otherwise
 */
static int matrix_eliminate_arr(struct matrix *mat)
{
	assert(mat);

	float tmp;
	for (int i = 0; i < mat->rows; i++) {
		for (int j = i + 1; j < mat->rows; j++) {
			if (mat->mat[i * mat->cols + i] == 0)
				return -1; // row swap needed

			tmp = mat->mat[j * mat->cols + i] /
			       mat->mat[i * mat->cols + i];

			for (int k = i; k < mat->cols; k++) {
				mat->mat[j * mat->cols + k] -=
				mat->mat[i * mat->cols + k] * tmp;
			}
		}
	}
	return 0;
}


static void matrix_eliminate()
{
	struct matrix mat;

	matrix_user_input_1(&mat);

	if (matrix_eliminate_arr(&mat))
		printf("Cannot eliminate without row swaps\n");
	else
		matrix_show(&mat);

	matrix_free(&mat);
}

static inline void show_menu()
{
	printf("Select an option from the Matrix Menu\n"
		"(e) to exit\n"
		"(b) to go back to the Main Menu\n"
		"(add) to add Matrices\n"
		"(sub) to subtract Matrices\n"
		"(mul) to multiply Matrices\n"
		"(tr) to transpose a Matrix\n"
		"(elim) to change a Matrix to REF\n");
}


void matrix_menu()
{
	struct htable table;
	int choice;

	htable_init(&table, HTABLE_SIZE);
	htable_add_arr(&table, options, NUM_OPTIONS);

	while (1) {
		show_menu();
		choice = get_menu_selection(&table);

		switch (choice) {
		case EXIT :
			exit(0);
		case BACK_TO_MAIN_MENU :
			goto out;
		case ADD :
			matrix_add();
			break;
		case SUB :
			matrix_sub();
			break;
		case MUL :
			matrix_multiply();
			break;
		case TRANSPOSE :
			matrix_transpose();
			break;
		case ELIMINATE :
			matrix_eliminate();
			break;
		case INVALID :
			printf("Invalid selection. Try again...\n");
			continue;
		}
	}
out:
	htable_free(&table);
}
