#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "gfx/gfx.h"


/******************************************************************************/
/*                                   GLOBAL                                   */
/******************************************************************************/

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NODE_RADIUS 17
#define gfxReset() gfx_clear(), gfx_paint()

/******************************************************************************/

/******************************************************************************/
/*                                    STRUCT                                  */
/******************************************************************************/
struct tree_node {
  int value;
  struct tree_node *left;
  struct tree_node *right;
  bool highlighted;
};

/******************************** END STRUCT **********************************/

/******************************************************************************/
/*                                   FUNCTIONS                                */
/******************************************************************************/
void InsertNode(struct tree_node **root, int value) {
  if (*root == NULL) {
    *root = malloc(sizeof(struct tree_node));
    (*root)->value = value;
    (*root)->left = NULL;
    (*root)->right = NULL;
    (*root)->highlighted = true;
  } else {
    if (value > (*root)->value) {
      InsertNode(&(*root)->right, value);
    } else {
      InsertNode(&(*root)->left, value);
    }
  }
}

void DeleteNode(struct tree_node **root, int value) {
  if (*root == NULL) {
    return;
  } else {
    if (value < (*root)->value) {
      DeleteNode(&(*root)->left, value);
    } else if (value > (*root)->value) {
      DeleteNode(&(*root)->right, value);
    } else {
      if ((*root)->left == NULL && (*root)->right == NULL) {
        free(*root);
        *root = NULL;
      } else if ((*root)->left == NULL) {
        struct tree_node *temp = *root;
        *root = (*root)->right;
        free(temp);
        temp = NULL;
      } else if ((*root)->right == NULL) {
        struct tree_node *temp = *root;
        *root = (*root)->left;
        free(temp);
        temp = NULL;
      } else {
        struct tree_node *temp = (*root)->right;
        while (temp->left != NULL) {
          temp = temp->left;
        }
        (*root)->value = temp->value;
        DeleteNode(&(*root)->right, temp->value);
      }
    }
  }
}


struct tree_node *SearchNode(struct tree_node *root, int value) {
  if (root == NULL) {
    return NULL;
  } else {
    if (value < root->value) {
      return SearchNode(root->left, value);
    } else if (value > root->value) {
      return SearchNode(root->right, value);
    } else {
      return root;
    }
  }
}

struct tree_node *MinNode(struct tree_node **root) {
  if (*root == NULL) {
    return NULL;
  } else {
    if ((*root)->left == NULL) {
      return *root;
    } else {
      return MinNode(&(*root)->left);
    }
  }
}

struct tree_node *MaxNode(struct tree_node **root) {
  if (*root == NULL) {
    return NULL;
  } else {
    if ((*root)->right == NULL) {
      return *root;
    } else {
      return MaxNode(&(*root)->right);
    }
  }
}

struct tree_node *SuccessorNode(struct tree_node **root, int value) {
  struct tree_node *temp = SearchNode(*root, value);
  if (temp == NULL) {
    return NULL;
  } else {
    if (temp->right != NULL) {
      return MinNode(&temp->right);
    } else {
      struct tree_node *parent = *root;
      while (parent != NULL && parent->value != value) {
        if (value < parent->value) {
          parent = parent->left;
        } else {
          parent = parent->right;
        }
      }
      return parent;
    }
  }
}

struct tree_node *PredecessorNode(struct tree_node **root, int value) {
  struct tree_node *temp = SearchNode(*root, value);
  if (temp == NULL) {
    return NULL;
  } else {
    if (temp->left != NULL) {
      return MaxNode(&temp->left);
    } else {
      struct tree_node *parent = *root;
      while (parent != NULL && parent->value != value) {
        if (value < parent->value) {
          parent = parent->left;
        } else {
          parent = parent->right;
        }
      }
      return parent;
    }
  }
}

void FreeTree(struct tree_node **root) {
  if (*root != NULL) {
    FreeTree(&(*root)->left);
    FreeTree(&(*root)->right);
    free(*root);
    *root = NULL;
  }
}

void DrawNode(struct tree_node *node, int x, int y) {
  if (node != NULL) {
    if (node->highlighted) {
      gfx_set_color(49, 229, 67);
    } else {
      gfx_set_color(187, 155, 255);
    }
    gfx_filled_ellipse(x, y, NODE_RADIUS, NODE_RADIUS);
    char str[10];
    sprintf(str, "%d", node->value);
    gfx_set_color(54, 54, 54);
    gfx_text(x - 9, y - 5, str);
  }
}

void VisualizeTreePreOrder(struct tree_node *root, unsigned xStart, unsigned xEnd, unsigned y) {
  if (root == NULL) return;

  unsigned xMid = (xStart + xEnd) / 2;
  if (root->left != NULL) {
    gfx_set_color(244, 244, 244);
    gfx_line(xMid, y, (xStart + xMid) / 2, y + (NODE_RADIUS * 2) + 36);
  }
  if (root->right != NULL) {
    gfx_set_color(244, 244, 244);
    gfx_line(xMid, y, (xMid + xEnd) / 2, y + (NODE_RADIUS * 2) + 36);
  }

  DrawNode(root, xMid, y);

  VisualizeTreePreOrder(root->left, xStart, xMid, y + (NODE_RADIUS * 2) + 36);
  VisualizeTreePreOrder(root->right, xMid, xEnd, y + (NODE_RADIUS * 2) + 36);
}

void TreeHighlightReset(struct tree_node **root) {
  if (*root == NULL) {
    return;
  } else {
    (*root)->highlighted = false;
    TreeHighlightReset(&(*root)->left);
    TreeHighlightReset(&(*root)->right);
  }
}

void WriteTreeFilePreOrder(struct tree_node *root, FILE *file) {
  if (root == NULL) {
    fprintf(file, "NULL ");
  } else {
    fprintf(file, "DATA ");
    fprintf(file, "%d ", root->value);
    fprintf(file, "LEFT ");
    WriteTreeFilePreOrder(root->left, file);
    fprintf(file, "RIGHT ");
    WriteTreeFilePreOrder(root->right, file);
  }
}

void ReadTreeFilePreOrder(struct tree_node **root, FILE *file) {
  char str[24];
  fscanf(file, "%s ", str);
  if (strcmp(str, "NULL") == 0) {
    *root = NULL;
  } else {
    *root = malloc(sizeof(struct tree_node));
    fscanf(file, "%s ", str);
    (*root)->value = atoi(str);
    fscanf(file, "%s ", str);
    ReadTreeFilePreOrder(&(*root)->left, file);
    fscanf(file, "%s ", str);
    ReadTreeFilePreOrder(&(*root)->right, file);
  }
}


/******************************* END FUNCTIONS *******************************/

void Menu() {
  printf("\033[1;36m");
  printf("1 - Insert\n");
  printf("2 - Delete\n");
  printf("3 - Search\n");
  printf("4 - Search Min\n");
  printf("5 - Search Max\n");
  printf("6 - Search Successor\n");
  printf("7 - Search Predecessor\n");
  printf("8 - Write Binary File\n");
  printf("9 - Read Binary File\n");
  printf("10 - Free Tree\n");
}

int main() {
  gfx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "Tree Visualizer");
  struct tree_node *root = NULL;
  int choice = 0;

  do {
    system("clear");
    Menu();
    do {
      printf("\033[1;32mInsert your option: ");
      scanf("%d", &choice);
      if (choice < 0 || choice > 10) {
        printf("\033[1;31mWrong choice...\n");
      }
    } while (choice < 0 || choice > 10);

    TreeHighlightReset(&root);
    gfxReset();

    switch (choice) {
      // Insert
      case 1: {
        printf("\033[1;32mInsert a value: ");
        int value;
        scanf("%d", &value);
        if (SearchNode(root, value) != NULL) {
          printf("\033[1;31mValue already exists...\n");
        } else {
          InsertNode(&root, value);
        }

        VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
        gfx_paint();

      }
        break;
        // Delete
      case 2: {
        printf("\033[1;32mDelete a value: ");
        int value;
        scanf("%d", &value);
        if (SearchNode(root, value) == NULL) {
          printf("\033[1;31mValue does not exist...\n");
        } else {
          DeleteNode(&root, value);
        }

        VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
        gfx_paint();

      }
        break;
        // Search
      case 3: {
        printf("\033[1;32mSearch a value: ");
        int value;
        scanf("%d", &value);
        struct tree_node *temp = SearchNode(root, value);
        if (temp == NULL) {
          printf("\033[1;31mValue does not exist...\n");
        } else {
          printf("\033[1;32mValue found!\n");
          temp->highlighted = true;
        }
        VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
        gfx_paint();
      }
        break;
        // Search Min
      case 4: {
        struct tree_node *temp = MinNode(&root);
        if (temp == NULL) {
          printf("\033[1;31mTree is empty...\n");
        } else {
          printf("\033[1;32mMin value: %d\n", temp->value);
          temp->highlighted = true;
        }
        VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
        gfx_paint();
      }
        break;
        // Search Max
      case 5: {
        struct tree_node *temp = MaxNode(&root);
        if (temp == NULL) {
          printf("\033[1;31mTree is empty...\n");
        } else {
          printf("\033[1;32mMax value: %d\n", temp->value);
          temp->highlighted = true;
        }
        VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
        gfx_paint();
      }
        break;
        // Search Successor
      case 6: {
        printf("\033[1;32mSearch a value: ");
        int value;
        scanf("%d", &value);
        struct tree_node *succeed = SuccessorNode(&root, value);
        if (succeed == NULL) {
          printf("\033[1;31mValue does not exist...\n");
        } else {
          printf("\033[1;32mSuccessor value: %d\n", succeed->value);
          succeed->highlighted = true;
        }
        VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
        gfx_paint();
      }
        break;
        // Search Predecessor
      case 7: {
        printf("\033[1;32mSearch a value: ");
        int value;
        scanf("%d", &value);
        struct tree_node *predecessor = PredecessorNode(&root, value);
        if (predecessor == NULL) {
          printf("\033[1;31mValue does not exist...\n");
        } else {
          printf("\033[1;32mPredecessor value: %d\n", predecessor->value);
          predecessor->highlighted = true;
        }
        VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
        gfx_paint();
      }
        break;
        // Write Binary File
      case 8: {
        char filename[100];
        filename[0] = '\0'; // Reset filename
        do {
          printf("\033[1;32mName of file to write: ");
          scanf("%s", filename);
          if (strlen(filename) == 0) {
            printf("\033[1;31mWrong filename (too short)...\n");
          }
        } while (strlen(filename) == 0);

        if (strcmp(filename, ".btpo") != 0) {
          strcat(filename, ".btpo");
        }
        FILE *file = fopen(filename, "w");

        if (file == NULL) {
          printf("\033[1;31mError opening file...\n");
        } else {
          WriteTreeFilePreOrder(root, file);
          fclose(file);
          gfx_text(14, WINDOW_HEIGHT - 24, "File written!");
        }
      }
        break;
        // Read Binary File
      case 9: {
        char filename[100];
        filename[0] = '\0'; // Reset filename
        do {
          printf("\033[1;32mName of file to read: ");
          scanf("%s", filename);
          if (strlen(filename) == 0) {
            printf("\033[1;31mWrong filename (too short)...\n");
          }
        } while (strlen(filename) == 0);
        if (strcmp(filename, ".btpo") != 0) {
          strcat(filename, ".btpo");
        }
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
          printf("\033[1;31mError opening file...\n");
        } else {
          FreeTree(&root);
          gfxReset();
          ReadTreeFilePreOrder(&root, file);
          fclose(file);
          TreeHighlightReset(&root);
          VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
          gfx_paint();
        }
      }
        break;
        // Free Tree
      case 10: {
        FreeTree(&root);
        printf("\033[1;32mTree freed...\n");
        gfx_clear();
        gfx_paint();
      }
        break;
      default: {
        VisualizeTreePreOrder(root, 0, WINDOW_WIDTH, 24);
        gfx_paint();
      }
        break;
    }
  } while (choice != 0);

  gfx_quit();
  FreeTree(&root);

  return 0;
}