#include "rbtree.h"

#include <stdlib.h>

void rotate_left(rbtree *t, node_t *x);
void rotate_right(rbtree* t, node_t *x);

void rbtree_delete_recursive(rbtree *t, node_t *node);

void insert_fixup(rbtree *t, node_t *z);
void delete_fixup(rbtree *t, node_t *z);

void rbtree_transplant(rbtree *t, node_t *u, node_t *v);

node_t *find_min_node(rbtree *t, node_t *current);
node_t *get_successor(const rbtree *t, node_t *node);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if(p == NULL) return NULL;  // 할당 실패했으면 null return
  
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  if(new_node == NULL){
    free(p);
    return NULL;
  } 
  
  new_node->color = RBTREE_BLACK;
  new_node->left = new_node;
  new_node->right = new_node;
  new_node->parent = new_node;
  
  p->nil = new_node;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  if (!t) return;
  
  rbtree_delete_recursive(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *x = t->root;
  node_t *y = t->nil;
  node_t *z = (node_t *)malloc(sizeof(node_t));
  if(z == NULL) return NULL;

  z->key = key; 
  z->color = RBTREE_RED;
  z->left = t->nil;
  z->right = t->nil;
  z->parent = t->nil;

  if(t->root == t->nil){
    z->color = RBTREE_BLACK;
    t->root = z;
    return t->root;
  }

  while(x != t->nil){
    y = x;
    if(z->key < x->key){
      x = x->left;
    }else {
      x = x->right;
    }
  }
  
  z->parent = y;
  if(y == t->nil){
    t->root = z;
  }else if(z->key < y->key){
    y->left = z;
  }else{
    y->right = z;
  }
 
  insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  if(t->root == t->nil) return NULL;
  node_t *current = t->root;
  
  while(current != t->nil){
    
    if(key < current->key){
      current = current->left;
    }else if(key > current->key){
      current = current->right;
    }else{
      return current;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *current = t->root;
  while(current->left != t->nil){
    current = current->left;
  }
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *current = t->root;
  while(current->right != t->nil){
    current = current->right;
  }
  return current;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if(!p) return -1;
  node_t *y = p;
  node_t *x = t->nil;
  color_t y_origin_color = y->color;
  if(p->left == t->nil){        // 삭제할 노드의 왼쪽 자식이 비어있을 때 : 
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }else if(p->right == t->nil){ // 삭제할 노드의 오른쪽 자식이 비어있을 때
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }else{                        // 자식이 둘다 존재할 때
    y = find_min_node(t, p->right);   // 선임자 찾기
    y_origin_color = y->color;
    x = y->right;
    if(y != p->right){
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }else{
      x->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if(y_origin_color == RBTREE_BLACK){
    delete_fixup(t, x);
  }

  free(p);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *current = rbtree_min(t);
  int i = 0;
  while(current != t->nil && i < n){
    arr[i] = current->key;
    current = get_successor(t, current);
    i += 1;
  }
  return 0;
}

void insert_fixup(rbtree *t, node_t *z){
  while(z->parent->color == RBTREE_RED){
    if(z->parent == z->parent->parent->left){
      node_t *y = z->parent->parent->right; // 삼촌 노드
      if(y->color == RBTREE_RED){   // 삼촌이 레드일때 : case 1 : 조부모와 left, right color change
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{                        // 삼촌이 블랙일때
        if(z == z->parent->right){
          z = z->parent;
          rotate_left(t, z);        // case 2 : LR or RL rotate
        }
        z->parent->color = RBTREE_BLACK; 
        z->parent->parent->color = RBTREE_RED;
        rotate_right(t, z->parent->parent);  // case 3 : LL or RR rotate
      }
    }else{      // 반대쪽 노드일때
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        if(z == z->parent->left){
          z = z->parent;
          rotate_right(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotate_left(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK; // 속성 2
}

void delete_fixup(rbtree *t, node_t *z) {
  while(z != t->root && z->color == RBTREE_BLACK){
    if(z == z->parent->left){   // 현재 노드가 부모의 왼쪽자식일 때
      node_t *sibling = z->parent->right;
      if(sibling->color == RBTREE_RED){   // case 1 : 형제가 레드일 때
        z->parent->color = RBTREE_RED;    // 현재 노드쪽으로 회전 후 다음 case 확인
        sibling->color = RBTREE_BLACK;
        rotate_left(t, z->parent);
        sibling = z->parent->right;       // 회전 후 z의 위치가 변경 되었기 때문에 형제 노드 갱신
      }
      if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){ // case : 2
        sibling->color = RBTREE_RED;
        z = z->parent;
      }else {
        if(sibling->right->color == RBTREE_BLACK){ // case 3: 형제가 블랙이고 가까운 자식이 레드
          sibling->left->color = RBTREE_BLACK;    // 회전 전에 부모자식 색 변경
          sibling->color = RBTREE_RED;
          rotate_right(t, sibling);
          sibling = z->parent->right;
        }
        sibling->color = z->parent->color;      // case 4 : 형제가 블랙이고 먼 자식이 레드일때 현재 노드의 부모와 형제의 색을 서로 바꾼다.
        z->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;   // 형제의 먼 자식도 블랙으로 변경
        rotate_left(t, z->parent);              // RR rotate
        z = t->root;
      }
    }else{    
      node_t *sibling = z->parent->left;
      if(sibling->color == RBTREE_RED){
        z->parent->color = RBTREE_RED;
        sibling->color = RBTREE_BLACK;
        rotate_right(t, z->parent);
        sibling = z->parent->left;
      }
      if(sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        z = z->parent;
      }else{
        if(sibling->left->color == RBTREE_BLACK){
          sibling->right->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          rotate_left(t, sibling);
          sibling = z->parent->left;
        }
        sibling->color = z->parent->color;
        z->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        rotate_right(t, z->parent);
        z = t->root;
      }
    }
  }

  // 종료전
  if(z != t->nil){
    z->color = RBTREE_BLACK; //타겟을 BLACK으로 변경
  }
}

void rotate_right(rbtree* t, node_t *x){
  node_t *y = x->left;
  if(y == t->nil) return;

  x->left = y->right;

  if(y->right != t->nil){
    y->right->parent = x;
  }

  y->parent = x->parent;

  if(x->parent == t->nil){
    t->root = y;
  }else if(x == x->parent->right){
    x->parent->right = y;
  }else{
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
}

void rotate_left(rbtree *t, node_t *current){
  if (current == t->nil || current->right == t->nil) return;
  node_t *right_child = current->right; // 오른쪽 자식을 변수에 저장

  current->right = right_child->left; // 오른쪽 자식의 왼쪽자녀를 현재 노드의 오른쪽에 추가

  if(right_child->left != t->nil){    // 위 코드와 같이 부모 자식간 연결
    right_child->left->parent = current;
  }

  right_child->parent = current->parent;  // 현재 노드의 부모를 오른쪽 자식의 부모로 연결

  if(current->parent == t->nil){  // 부모가 Nil이면 루트이므로 오른쪽 자식을 루트로 변경
    t->root = right_child;
  }else if(current == current->parent->left){ // 현재 노드가 부모의 왼쪽자식이라면
    current->parent->left = right_child;      // 부모의 왼쪽 자식을 회전 후의 부모로 변경
  }else{                                      // 현재 노드가 부모의 오른쪽 자식이라면
    current->parent->right = right_child;     // 부모의 오른쪽 자식을 회전 후의 부모로 변경
  }

  right_child->left = current;                // 부모 노드 변경 이후 현재 노드를 변경한 자식 노드와 연결
  current->parent = right_child;              // 회전 후의 부모값 적용
}


void rbtree_delete_recursive(rbtree *t, node_t *node){
  if(node == t->nil) return;
  
  rbtree_delete_recursive(t, node->left);
  rbtree_delete_recursive(t, node->right);

  free(node);
}

node_t *find_min_node(rbtree *t, node_t *current){
  while(current->left != t->nil){
    current = current->left;
  }
  return current;
}

node_t *get_successor(const rbtree *t, node_t *node){
  if (node->right != t->nil) {
      node_t *cur = node->right;
      while (cur->left != t->nil) cur = cur->left;
      return cur;
  }

  node_t *p = node->parent;
  while (p != t->nil && node == p->right) {
      node = p;
      p = p->parent;
  }
  return p;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }else if(u == u->parent->left){
    u->parent->left = v;
  }else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

