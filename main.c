#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx/gfx.h"

/******************************************************************************/
/*                                   GLOBAL                                   */
/******************************************************************************/

static const size_t WINDOW_WIDTH = 640;
static const size_t WINDOW_HEIGHT = 480;

static const size_t NODE_RADIUS = 17;

/******************************************************************************/

/******************************************************************************/
/*                                    STRUCT                                  */
/******************************************************************************/
struct tree_node {
  int value;
  struct tree_node *left;
  struct tree_node *right;
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
  } else {
    if (value < (*root)->value) {
      InsertNode(&(*root)->left, value);
    } else {
      InsertNode(&(*root)->right, value);
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
      } else if ((*root)->right == NULL) {
        struct tree_node *temp = *root;
        *root = (*root)->left;
        free(temp);
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
    gfx_set_color(187, 155, 255);
    gfx_filled_ellipse(x, y, NODE_RADIUS, NODE_RADIUS);
    char str[10];
    sprintf(str, "%d", node->value);
    gfx_set_color(54, 54, 54);
    gfx_text(x - 9, y - 5, str);
  }
}

void VisualizeTreePreOrder(struct tree_node *root, size_t level, size_t x, size_t y) {
  if (root != NULL) {
    DrawNode(root, x, y);
    VisualizeTreePreOrder(root->left, level + 1, x / 2, y + NODE_RADIUS * 2);
    VisualizeTreePreOrder(root->right, level + 1, x + (x / 2), y + NODE_RADIUS * 2);
    printf("Level %lu: x = %lu\n", level, x + (x / 2));
  }
  return;
}

/******************************* END FUNCTIONS *******************************/

int main() {
  gfx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "Tree Visualizer");
  struct tree_node *root = NULL;
  int choice = 0;

  do {
    int value;
    printf("Enter a value to insert: ");
    scanf("%d", &value);
    InsertNode(&root, value);

    VisualizeTreePreOrder(root, 0, WINDOW_WIDTH / 2, 24);
    gfx_paint();

    printf("Want to insert more? (1 = yes, 0 = no): ");
    scanf("%d", &choice);
    gfx_clear();
  } while (choice);

  gfx_quit();
  FreeTree(&root);

  return 0;
}