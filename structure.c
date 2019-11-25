#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE 32

struct cell {
  int size;
  char *name;
  int name_len;
  struct cell *next;
};

struct cell *create_cell(int size, char *name, int name_len);
void print_cells();

struct cell *head;

int main(int argc, char const *argv[]) {

  head = create_cell(0, "", 0);

  struct cell *tmp;
  tmp = head;

  while (1) {
    int size;
    char name[255];
    int name_len;

    printf("size? :");
    scanf("%d", &size);
    if (size <= 0) {
      break;
    }

    printf("name? :");
    scanf("%s", name);
    printf("\n");

    name_len = strlen(name);

    tmp->next = create_cell(size, name, name_len);
    tmp = tmp->next;
  }

  print_cells();
  return 0;
}

struct cell *create_cell(int size, char *name, int name_len) {
  struct cell *new_cell;
  new_cell = (struct cell *)calloc(1, sizeof(struct cell));

  new_cell->size = size;
  new_cell->name_len = name_len;
  new_cell->name = (char *)calloc(name_len + 1, sizeof(char));

  snprintf(new_cell->name, name_len + 1, "%s", name);

  new_cell->next = NULL;

  return new_cell;
}

void print_cells() {
  struct cell *top = head->next;
  struct cell *mid = head->next;
  struct cell *bottom = head->next;

  int seek_pos = 0;

  while (bottom != NULL) {
    // top
    int line_i = 0;
    while (line_i < LINE) {
      if (top->size <= 0) {
        top = top->next;
        break;
      }
      printf("+");
      line_i++;

      int cell_i;
      for (cell_i = 1; cell_i < top->size && line_i < LINE; cell_i++) {
        printf("-");
        line_i++;
      }

      if (line_i == LINE) {
        printf("+\n");
        break;
      }
      top = top->next;
      if (top == NULL) {
        printf("+\n");
        break;
      }
    }

    // mid
    line_i = 0;
    while (line_i < LINE) {
      if (mid->size <= 0) {
        mid = mid->next;
        break;
      }
      printf("|");
      line_i++;

      int cell_i;
      for (cell_i = 1; cell_i < mid->size && line_i < LINE; cell_i++) {
        if (cell_i - 1 < mid->name_len) {
          printf("%c", mid->name[cell_i - 1]);
        } else {
          printf(" ");
        }
        line_i++;
      }

      if (line_i == LINE) {
        printf("|\n");
        break;
      }
      mid = mid->next;
      if (mid == NULL) {
        printf("|\n");
        break;
      }
    }

    // bottom
    line_i = 0;
    while (line_i < LINE) {
      if (bottom->size <= 0) {
        bottom = bottom->next;
        break;
      }
      printf("+");
      line_i++;

      int cell_i;
      for (cell_i = 1; cell_i < bottom->size && line_i < LINE; cell_i++) {
        printf("-");
        line_i++;
      }

      if (line_i == LINE) {
        bottom->size -= cell_i;
        printf("+\n");
        break;
      }
      bottom = bottom->next;
      if (bottom == NULL) {
        printf("+\n");
        break;
      }
    }
  }
}