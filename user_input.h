#ifndef USER_INPUT_H_
#define USER_INPUT_H_


#include "htable.h"
#include "matrix_menu.h"


int get_menu_selection(struct htable *table);
void get_matrix_dim(int *dim, const char *prompt);
void get_matrix_data(struct matrix *mat);

#endif /* USER_INPUT_H_ */
