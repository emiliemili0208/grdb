#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

#define MAX 10

/*vertex_t queue[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;*/


vertex_t peek(queue_t q) {
   return q->v[q->front];
}

int isEmpty(queue_t q) {
   return q->itemCount == 0;
}

int isFull(queue_t q) {
   return q->itemCount == MAX;
}

int isInQueue(vertex_t data, queue_t q){
   for (int i=0; i< q->itemCount ;i++){
      if (data == q->v[i]){
         return 1;
      }
   }
   return 0;
}

int size(queue_t q) {
   return q->itemCount;
}  

void insert(vertex_t data, queue_t q) {
   vertex_t * que = q->v;
   if(!isFull(q)) {
	
      if(q->rear == MAX-1) {
         q->rear = -1;            
      }       

      que[++(q->rear)] = data;
      (q->itemCount)++;
   }
}

vertex_t removeData(queue_t q) {
   vertex_t * que = q->v;
   vertex_t data = que[(q->front)++];
	
   if(q->front == MAX) {
      q->front = 0;
   }
	
   (q->itemCount)--;
   return data;  
}

