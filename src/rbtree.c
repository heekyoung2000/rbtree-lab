#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) { //새로운 rbtree를 선언, nil 노드를 root 노드로 설정
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); //포인터 배열 할당
  node_t *nil = (node_t *)calloc(1,sizeof(node_t)); //node_t에 배열 할당
  
  nil->color=RBTREE_BLACK; //nil의 색깔은 black으로 지정
  p-> root = nil; // root 노드를 nil로 선언 ( tree가 빈 경우에는 root 노드가 nill 노드이여야 하기 때문)
  p-> nil = nil; // nil을 nil로 선언
  // TODO: initialize struct if needed
  return p;
}
//각 노드와 그 자식 노드들의 메모리를 반환하는 함수
void free_node(rbtree *t,node_t *x){
  if(x->left != t->nil){
    free_node(t,x->left);
  }
  if(x->right!=t->nil){
    free_node(t,x->right);
  }
  //후위 순위를 통해 노드 메모리 반환
  free(x);
  x=NULL;
}
//트리를 순회하면서 각 노드의 메모리를 반환하는 함수
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  //노드를 하나씩 찾아가면서 삭제 해줘야 함
  if(t->root!=t->nil){
    free_node(t,t->root);
  }
  //nill 노드와 트리의 구조체를 반환
  free(t->nil);
  free(t);//트리 free
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t->nil; //y는 nil 노드
  node_t *x = t->root; //x는 루트노드
  node_t *z =(node_t *)calloc(1,sizeof(node_t)); //새 노드 생성
  z -> key = key; //새 노드의 키값을 저장 
  

  while(x != t->nil){ //루트노드가 nil이 아닐때 -트리 내에서 삽입할 위치 탐색하는 과정
    y=x; 
    if (z->key < x->key){ // 현재 탐색 하고 있는 키 값이 x노드의 키 값보다 작을때
      x=x->left; // x의 왼쪽 노드에 넣어줌
    }
    else{ // 만약 x노드의 키 값보다 클때
      x=x->right; // x의 오른쪽 노드에 넣어줌
      }
    }
  z->parent =y; // z의 부모노드는 y로 지정 - 새 노드의 부모를 지정하는 과정
  if (y==t->nil){ //만약 y가 nil일 경우
    t->root = z; //루트 노드는 z
  }
  else if(z->key < y->key){ // z의 key값이 y의 키값보다 작을 경우
    y->left = z; //y의 왼쪽 노드에 z를 삽입한다.
  }
  else{ //z의 key 값이 y의 키값보다 클경우
    y->right =z; //y의 오른쪽 노드에 z 삽입
  }
  z->left = t->nil; //새로 삽입한 노드의 왼쪽이랑 오른쪽은 nil로 설정
  z->right = t->nil;
  z ->color = RBTREE_RED; // z노드의 색을 레드로 설정
  rb_insert_fixup(t, z); //불균형 복구
  
  return z;
}


//새로 삽입된 노드는 항상 RED 색상으로 삽입 되는데, 새로 삽입된 노드의 부모 노드가 RED인경우, 규칙위반이므로 불균형
//따라서 3가지로 나눠서 CASE 해결
void rb_insert_fixup(rbtree *t,node_t *z){
  node_t *uncle; //삼촌 노드 설정
  //새로 삽입돈 노드의 부모 노드가 RED 인 경우 - 루트노드까지 불균형 삽입이 없는지 확인해야 하므로 while 문 사용
  while(z->parent->color == RBTREE_RED) {
    //z의 부모가 조부모의 왼쪽 서브트리일 경우
    if(z->parent == z->parent->parent->left){
      uncle=z->parent->parent->right;
      //CASE 1: 노드 z의 삼촌 y가 적색인 경우
      if(uncle->color ==RBTREE_RED){
        z->parent->color =RBTREE_BLACK; //z의 부모노드의 색깔은 black으로 바꿔줌
        uncle->color = RBTREE_BLACK; // 삼촌 노드의 색깔을 black으로 바꿔줌
        z -> parent -> parent ->color =RBTREE_RED; //조부모의 색깔을 red로 바꿔줌
        z = z ->parent->parent; // z의 위치를 조부모의 위치로 옮겨줌
      }
      // case 2 : z의 삼촌 uncle이 흑색이며 z가 오른쪽 자식 인 경우
      else{
        if(z == z->parent->right){ 
            z = z->parent; //z의 위치를 부모 노드의 위치로 옮겨주고
            left_rotate(t,z); //왼쪽 회전 
        }
        //case 3 : z이 삼촌 y가 흑색이며 z의 왼쪽 자식 인 경우
        z->parent->color = RBTREE_BLACK; // 부모노드의 색깔을 black이라 설정
        z->parent->parent->color = RBTREE_RED; // 조부모의 색깔을 red라 설정
        right_rotate(t,z->parent->parent); //오른쪽 회전
      }
    }
    //z의 부모가 조부모의 오른쪽 서브 트리 일 경우 ( 위에 경우와 거의 일치)
    else{
      uncle=z->parent->parent->left;//삼촌노드는 z의 조부모의 왼쪽 노드이다.
      //case 4 : 노드 z의 삼촌 uncle이 적색인 경우
      if(uncle->color == RBTREE_RED){
        z->parent->color =RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z -> parent -> parent ->color =RBTREE_RED;
        z = z ->parent->parent;
      }
      //case 5 : z의 삼촌 y가 흑생이며 z가 오른쪽 자식인 경우
      else{
        if(z == z->parent->left){
            z = z->parent;
            right_rotate(t,z);
        }
        //case 6 : z의 삼촌 y가 흑색이며 z가 왼쪽 자식일 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }

    }
  }
  t->root->color = RBTREE_BLACK; // 마지막으로 루트 노드의 색깔을 black으로 설정

  

}

//키에 해당하는 노드를 반환하는 함수
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *current = t->root;
  while(current!= t->nil){ //루트노드가 nil이 아닐때 까지
    if (current->key == key){ //key가 현재 노드랑 같다면
      return current; //current 리턴
    }
     //key가 현재 노드랑 다르다면
    if(current->key < key){ //  key값이 현재 가르키고 있는 루트노드보다 작다면
      current = current->right; //왼쪽 노드로 이동
    }
    else{
      current = current->left; // 아니라면 오른쪽 노드로 이동
    }
    
    
  }

   //키값을 가진 노드가 없을 경우 NULL 반환
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  
  if (t->root == t->nil){
    return NULL;
  }
  node_t *current = t->root;

  while(current->left != t->nil){ //current left노드가 nil이 아니라면
    current = current ->left; //left 노드의 최소값을 찾아 저장
  }
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TDO: implement find
  if(t->root == t->nil){
    return NULL;
  }
  node_t *current = t->root;
  while(current->right != t->nil){
    current = current ->right;
  }
  return current;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }
  else if(u==u->parent->left){
    u->parent->left =v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y;
  node_t *x;
  color_t y_original_color;
  y=p; //
  y_original_color = y->color; // y의 original color에 현재 y의 color를 저장한다.
  if(p->left==t->nil){ // 
    x = p->right;
    rb_transplant(t,p,p->right); // 
  }
  else if(p->right == t->nil){
    x=p->left;
    rb_transplant(t,p,p->left);
  }
  else{
    y=p->right;
    while(y->left!=t->nil){
      y=y->left;
    }
    y_original_color = y->color;
    x=y->right;
    if(y->parent==p){
      x->parent=y;
    }
    else{
      rb_transplant(t,y,y->right);
      y->right = p->right;
      y->right->parent =y;
    }
    rb_transplant(t,p,y);
    y->left = p->left;
    y->left->parent =y;
    y->color = p->color;
  }
  if(y_original_color==RBTREE_BLACK){
    rbtree_delete_fixup(t,x);
  }

  free(p);
  return 0;
}

void rbtree_delete_fixup(rbtree *t, node_t *x){
  node_t *w;
  while(x!=t->root && x->color == RBTREE_BLACK){ //x노드가 루트노드가 아닐때, 색깔이 black이 아닐때 
    //case 1~4 : 왼쪽
    if(x==x->parent->left){// x의 부모노드가 왼쪽일때
      w=x->parent->right; // w는 x의 부모노드의 오른쪽
      
      //case 1 : x의 형제 w가 적색인 경우
      if(w->color == RBTREE_RED){ //
        w->color =RBTREE_BLACK; // 형제노드의 색깔을 black으로 설정
        x->parent->color = RBTREE_RED; //x 부모의 색깔이 red로 설정
        left_rotate(t,x->parent); //왼쪽 회전
        w=x->parent->right; // 형제 노드는 오른쪽에 위치
      }

      //case 2 : x의 형제 w는 흑색이고 w의 두 자식이 모두 흑색인 경우
      if (w->left->color==RBTREE_BLACK && w->right->color ==RBTREE_BLACK)
      {
        w->color = RBTREE_RED; //형제노드의 색깔은 red
        x=x->parent; //x의 위치를 x의 부모노드로 이동
      }
      //case 3 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else{
        if(w->right->color == RBTREE_BLACK){ // 형제노드의 오른쪽 색깔은 black일때
          w->left->color = RBTREE_BLACK;//형제노드의 왼쪽 색깔은 black
          w->color = RBTREE_RED;//형제노드의 색깔을 red로 설정
          right_rotate(t,w); // 오른쪽 회전
          w=x->parent->right; //형제노드는 부모노드의 오른쪽
        }
        //case 4 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color; // 형제노드의 색깔을 부모노드의 색깔과 같게 함
        x->parent->color = RBTREE_BLACK; // 형제노드의 색깔을 black으로 함
        w->right->color = RBTREE_BLACK; //오른쪽 색깔을 black으로 함
        left_rotate(t,x->parent);//왼쪽으로 회전
        x=t->root;//x는 루트노드로 설정
      }
      
    }

    //case 5~8
    else{
      w=x->parent->left;
      if(w->color == RBTREE_RED){
        w->color =RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w=x->parent->left;
      }
      if (w->right->color==RBTREE_BLACK && w->left->color ==RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x=x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w=x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x=t->root;
      }
      
    }
  }
  x->color = RBTREE_BLACK;
}

void subtree_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *count){
  if(curr == t->nil){
    return;
  }

  subtree_to_array(t,curr->left,arr,n,count);
  if(*count <n){
    arr[(*count)++] = curr->key;
  }
  else return;
  subtree_to_array(t,curr->right,arr,n,count);
}
  
  


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root==t->nil){
    return 0;
  }
  size_t cnt=0;
  subtree_to_array(t,t->root,arr,n,&cnt);

  return 0;
}

void left_rotate(rbtree *t, node_t *x){
  
  // node_t *parent = x->parent;
  // node_t *left = x->left;
  node_t *y = x->right;

  x->right = y->left;
  if (y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  }
  else if(x==x->parent->left){
    x->parent->left = y;
  } 
  else {
    x->parent->right = y;
  }
  y->left =x;
  x->parent=y;
  
}

void right_rotate(rbtree *t, node_t *y){
  // node_t *parent = y->parent;
  // node_t *right = y->right;
  node_t *x = y->left;

  y->left = x->right; //x의 오른쪽 노드가 y의 왼쪽 노드에 옯겨짐
  if(x->right != t->nil){ //x의 오른쪽 노드가 nil일 경우
    x->right->parent=y;
  }
  x->parent = y->parent;
  if(y->parent == t->nil){ //y의 부모 노드가 nill일 경우
    t->root =x;
  }
  else if(y==y->parent->left){ //y가 y의 부모노드가 왼쪽 노드이면
    y->parent->left =x;
  }
  else{
    y->parent->right = x;
  }
  x->right=y;
  y->parent = x;
  

}
