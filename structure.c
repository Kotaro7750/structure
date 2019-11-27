#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cell {
  int size;
  char *name;
  int name_len;
  struct cell *next;
};

struct cell *create_cell(int size, char *name, int name_len);
int input_cells();
void print_cells(int);

struct cell *head;

int main(int argc, char const *argv[]) {

  if (argc < 2) {
    printf("USAGE: ./structure [bit width]\n");
    exit(1);
  }

  int bit_width = atoi(argv[1]);
  if (bit_width == 0) {
    printf("bit width should be greater than 0\n");
    exit(1);
  }

  head = create_cell(0, "", 0);

  int cell_num = input_cells();
  print_cells(bit_width);
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

int input_cells() {
  struct cell *tmp;
  tmp = head;
  int cell_num = 0;

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

    cell_num++;
  }

  return cell_num;
}

void print_cells(int bit_width) {
  struct cell *top = head->next;
  struct cell *mid = head->next;
  struct cell *bottom = head->next;

  int line = 0;

  while (bottom != NULL) {
    // count
    printf("%d", line);
    for (int i = 1; i < bit_width; i++) {
      printf(" ");
    }
    printf("%d\n", (line + 1) * bit_width - 1);
    // top
    int line_i = 0;
    while (line_i < bit_width) {
      if (top->size <= 0) {
        top = top->next;
        break;
      }
      printf("+");
      line_i++;

      int cell_i;
      for (cell_i = 1; cell_i < top->size && line_i < bit_width; cell_i++) {
        printf("-");
        line_i++;
      }

      if (line_i == bit_width) {
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
    while (line_i < bit_width) {
      if (mid->size <= 0) {
        mid = mid->next;
        break;
      }
      printf("|");
      line_i++;

      int cell_i;
      for (cell_i = 1; cell_i < mid->size && line_i < bit_width; cell_i++) {
        if (cell_i - 1 < mid->name_len) {
          printf("%c", mid->name[cell_i - 1]);
        } else {
          printf(" ");
        }
        line_i++;
      }

      if (line_i == bit_width) {
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
    while (line_i < bit_width) {
      if (bottom->size <= 0) {
        bottom = bottom->next;
        break;
      }
      printf("+");
      line_i++;

      int cell_i;
      for (cell_i = 1; cell_i < bottom->size && line_i < bit_width; cell_i++) {
        printf("-");
        line_i++;
      }

      if (line_i == bit_width) {
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
