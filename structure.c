#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum line_type { Top, Mid, Bottom };

struct cell {
  int unprinted_size;
  int width;
  char *name;
  int name_len;
  struct cell *next;
  bool is_omitted;
};

void print_help(void);
struct cell *create_cell(int size, char *name, int name_len);
bool input_cells();
void print_cells(int);
void print_count(struct cell **seek, int line, int bit_width);
void print_line(struct cell **seek, char partition, char padding,
                enum line_type line_type, int bit_width);
void print_omitted();

int get_numstr_length(int num);

struct cell *head;

int main(int argc, char *argv[]) {

  int opt;

  int bit_width = 32;

  while ((opt = getopt(argc, argv, "hw:")) != -1) {
    switch (opt) {
    case 'h':
      print_help();
      return 0;
      break;

    case 'w':
      if (optarg == NULL) {
        printf("USAGE: ./structure [bit width]\n");
        print_help();
        return 0;
      }
      bit_width = atoi(optarg);
      if (bit_width == 0) {
        fprintf(stderr, "field width should be greater than 0.\n");
        exit(1);
      }
      break;

    default:
      print_help();
      return 0;
      break;
    }
  }

  head = create_cell(0, "", 0);

  bool is_exceed_cell_exist = input_cells();
  print_cells(bit_width);

  if (is_exceed_cell_exist) {
    printf("\n");
    print_omitted();
  }
  return 0;
}

void print_help(void) {
  printf("USAGE: structure [-w <value>] [-h]\n\n");
  printf("\t-w: specify field unit width. default is 32.\n");
  printf("\t-h: show help.\n");
}

struct cell *create_cell(int width, char *name, int name_len) {
  struct cell *new_cell;
  new_cell = (struct cell *)calloc(1, sizeof(struct cell));

  new_cell->unprinted_size = width;
  new_cell->width = width;
  new_cell->name_len = name_len;
  new_cell->name = (char *)calloc(name_len + 1, sizeof(char));

  snprintf(new_cell->name, name_len + 1, "%s", name);

  if (name_len >= width) {
    new_cell->is_omitted = true;
  } else {
    new_cell->is_omitted = false;
  }
  new_cell->next = NULL;

  return new_cell;
}

bool input_cells() {
  struct cell *tmp;
  tmp = head;
  bool is_exceed_cell_exist = false;

  while (1) {
    int width;
    char name[255];
    int name_len;

    printf("width? :");
    scanf("%d", &width);
    if (width <= 0) {
      break;
    }

    printf("name? :");
    scanf("%s", name);
    printf("\n");

    name_len = strlen(name);

    tmp->next = create_cell(width, name, name_len);
    tmp = tmp->next;
    if (tmp->is_omitted) {
      is_exceed_cell_exist = true;
    }
  }

  return is_exceed_cell_exist;
}

void print_cells(int bit_width) {
  struct cell *count = head->next;
  struct cell *top = head->next;
  struct cell *mid = head->next;
  struct cell *bottom = head->next;

  int line = 0;

  while (bottom != NULL) {
    // count
    print_count(&count, line, bit_width);

    print_line(&top, '+', '-', Top, bit_width);
    print_line(&mid, '|', ' ', Mid, bit_width);
    print_line(&bottom, '+', '-', Bottom, bit_width);

    if (bottom != NULL && bottom->unprinted_size > 0) {
      line++;
    }
  }
}

void print_count(struct cell **seek, int line, int bit_width) {
  int line_i = 0;
  int prev_overflow = 0;

  while (line_i < bit_width) {
    // 既に書き終わったセルに関しては無視する
    if ((*seek)->unprinted_size <= 0) {
      (*seek) = (*seek)->next;
      break;
    }

    int correction;
    if (prev_overflow == 0) {
      //前のセルのカウントが溢れてなかったらその行の開始ビットを出力
      printf("%d", line_i + line * bit_width);

      //開始ビットの桁数による補正用 (ひと桁なら0)
      correction = get_numstr_length(line_i + line * bit_width) - 1;
      line_i += correction + 1;

      // correction+1:実際に出力した数->unprinted_sizeがこれを超えるとオーバーフロー（通常時）
      if (correction + 1 > (*seek)->unprinted_size) {
        prev_overflow = correction + 1 - (*seek)->unprinted_size;
      } else {
        prev_overflow = 0;
      }

      int cell_i = 1;
      for (cell_i = 1;
           cell_i < (*seek)->unprinted_size - correction && line_i < bit_width;
           cell_i++) {
        printf(" ");
        line_i++;
      }
    } else {
      if (prev_overflow <= (*seek)->unprinted_size) {
        int cell_i;
        for (cell_i = prev_overflow;
             cell_i < (*seek)->unprinted_size && line_i < bit_width; cell_i++) {
          printf(" ");
          line_i++;
        }

        prev_overflow = 0;
      } else {
        prev_overflow -= (*seek)->unprinted_size;
      }
    }

    if (line_i == bit_width) {
      printf("%d\n", (line + 1) * bit_width - 1);
      break;
    }

    if ((*seek)->next == NULL) {
      printf("%d\n", line_i + line * bit_width - 1);
      (*seek) = (*seek)->next;
      break;
    }
    (*seek) = (*seek)->next;
  }
}

void print_line(struct cell **seek, char partition, char padding,
                enum line_type line_type, int bit_width) {
  int line_i = 0;
  while (line_i < bit_width) {
    //既に書き終わったセルに関しては無視する
    if ((*seek)->unprinted_size <= 0) {
      (*seek) = (*seek)->next;
      break;
    }

    // Midで行をまたいで表示するときは先頭の|は表示しない
    if (line_type == Mid && (*seek)->width != (*seek)->unprinted_size) {
      printf(" ");
    } else {
      printf("%c", partition);
    }
    line_i++;

    int cell_i = 1;
    for (cell_i = 1;
         cell_i <= ((*seek)->unprinted_size - 1) && (line_i <= bit_width - 1);
         cell_i++) {
      // Midで省略されておらず、まだ出力するべき名前が残っている場合は名前を出力する
      if (line_type == Mid && (cell_i - 1 < (*seek)->name_len) &&
          (*seek)->is_omitted == false) {
        //行をまたいだ後半には名前を表示しない
        if ((*seek)->width != (*seek)->unprinted_size) {
          printf("%c", padding);
        } else {
          printf("%c", (*seek)->name[cell_i - 1]);
        }
      } else {
        printf("%c", padding);
      }
      line_i++;
    }

    //行の最大幅に達したとき
    if (line_i == bit_width) {
      // Bottomだったら未出力のサイズを変更する
      if (line_type == Bottom) {
        (*seek)->unprinted_size -= cell_i;
      }
      // Midで次の行に出力しない場合は区切り文字を出力する
      if (line_type == Mid) {
        if ((*seek)->unprinted_size - cell_i <= 0) {
          printf("%c", partition);
        }
      } else {
        printf("%c", partition);
      }
      printf("\n");
      break;
    }

    (*seek) = (*seek)->next;

    if ((*seek) == NULL) {
      printf("%c\n", partition);
      break;
    }
  }
}

int get_numstr_length(int num) {
  if (num == 0) {
    return 1;
  }
  int digit = 0;
  while (num != 0) {
    num /= 10;
    digit++;
  }
  return digit;
}

void print_omitted() {
  printf("---ommitted field---\n");
  struct cell *tmp = head->next;

  int count = 0;

  while (tmp != NULL) {
    if (tmp->is_omitted) {
      if (tmp->width == 1) {
        int padding = get_numstr_length(count) + 1;
        for (int i = 0; i < padding; i++) {
          printf(" ");
        }
        printf("%d bit:", count);
      } else {
        printf("%d-%d bit:", count, count + tmp->width - 1);
      }
      printf("\t%s\n", tmp->name);
    }
    count += tmp->width;
    tmp = tmp->next;
  }

  printf("--------------------\n");
}
