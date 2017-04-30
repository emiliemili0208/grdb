#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "schema.h"
#include "tuple.h"
#include "vertexid.h"
#include "cvert.h"
#include "queue.h"

#define MAX 256

queue_t getQueue(graph_t g, vertex_t root){
	queue_t q  = (queue_t) malloc(sizeof(struct queue));
	
	edge_t edge;
	q->v = (vertex_t *)malloc(MAX);
	//printf("yayayaya");
	q->front = 0;
	q->rear = -1;
	q->itemCount = 0;

	//add the neighbor node around root to queue 
	edge = g->e;
	//printf("edge = %llu <--> %llu\n", edge->id1, edge->id2);
	while (edge != NULL){
		//printf("edge = %llu <--> %llu\n", edge->id1, edge->id2);
		//compare if edge include root
		if (edge->id1 == root->id){
			vertex_t node = graph_find_vertex_by_id(g, edge->id2);
			//printf("node = %llu\n", node->id);
			if (!isInQueue(node, q)){
				insert(node, q);
			}
		}
		else if (edge->id2 == root->id){
			vertex_t node = graph_find_vertex_by_id(g, edge->id1);
			if (!isInQueue(node, q)){
				insert(node, q);
			}
		}
		edge = edge->next;
	}
	return q;
}

//return arraylist of attribute of a vertex
attrVertex_t getVertexAttr(vertex_t root){
	int i, offset, val;
	float fval;
	double dval;
	attrVertex_t attrList = (attrVertex_t)malloc(sizeof(struct attrVertex));
	attrList->v = (attribute_t *)malloc(MAX);
	attrList->number = 0;

	attribute_t *rootAttr = attrList -> v;

	tuple_t t = root->tuple;
	attribute_t attr;
	
	int index = 0;
	for (attr = t->s->attrlist; attr != NULL; attr = attr->next) {
		offset = tuple_get_offset(t, attr->name);
		if (offset >= 0) {
			switch (attr->bt) {
			case CHARACTER:
				if (tuple_get_char(t->buf + offset)!=0){
					rootAttr[index] = attr;	
					index++;
				}
				break;

			case VARCHAR:
				if (((char *)(t->buf + offset)) != ""){
					rootAttr[index] = attr;
					index++;
				}
				break;

			case BOOLEAN:
				val = tuple_get_bool(t->buf + offset);
				if (val != 0){
					rootAttr[index] = attr;
					index++;
				}
				break;

			case INTEGER:
				i = tuple_get_int(t->buf + offset);
				if (i != 0){
					rootAttr[index] = attr;
					index++;
				}
				break;

			case FLOAT:
				fval = tuple_get_float(t->buf + offset);
				if (fval != 0.0){
					rootAttr[index] = attr;
					index++;
				}
				break;

			case DOUBLE:
				dval = tuple_get_double(t->buf + offset);
				if (dval != 0.0){
					rootAttr[index] = attr;
					index++;
				}
				break;

			case DATE:
				break;
			
			case TIME:
				break;

			case BASE_TYPES_MAX:
				break;
			}
		}
	}
	attrList->number = index;
	return attrList;
}

schema_t largest_common_vertex(graph_t g, queue_t q, vertex_t root){
	//attrvertex of root
	attrVertex_t rootAttr = getVertexAttr(root);
	//schema array of root
	attribute_t *a = rootAttr->v;
	//create schema for the result
	schema_t largestSchema = (schema_t)malloc(sizeof(struct schema));
	attrVertex_t largestAttrVertex = (attrVertex_t)malloc(sizeof(struct attrVertex));
	largestAttrVertex->v = (attribute_t *)malloc(MAX);
	largestAttrVertex->number = 0;
	attribute_t *b = largestAttrVertex->v;
	//create a list to store the node with largest schema
	//graph_t cst  = (graph_t) malloc(sizeof(struct graph));
	int maximum = 0;
	//schema_t possibleSchema = (schema_t)malloc(sizeof(struct schema));
	schema_t possibleSchema = NULL;
	schema_init(&possibleSchema);
	attrVertex_t possibleAttrVertex = (attrVertex_t)malloc(sizeof(struct attrVertex));
	
	possibleAttrVertex->v = (attribute_t *)malloc(MAX);
	possibleAttrVertex->number = 0;
	attribute_t *possibleAttr = possibleAttrVertex->v;
	
	while (!isEmpty(q)){
		int index = 0;
		vertex_t nodeInQ = peek(q);
		attrVertex_t nodeAttrVertex = getVertexAttr(nodeInQ);
		attribute_t *nodeAttr = nodeAttrVertex->v;
		//comparing attributes in root and nodes in q
		for (int i=0; i< rootAttr->number; i++){
			for (int j=0; j< nodeAttrVertex->number; j++){
				if (strcmp(a[i]->name, nodeAttr[j]->name)==0){
					//possibleAttr[index] = a[i];
					index++;
				}
			}
		}
		if (index > maximum){
			maximum = index;
			int k=0;
			for (int i=0; i< rootAttr->number; i++){
				for (int j=0; j< nodeAttrVertex->number; j++){
					if (strcmp(a[i]->name, nodeAttr[j]->name)==0){
						possibleAttr[k]= nodeAttr[j];
						//schema_attribute_insert(possibleAttr, nodeAttr[j]);
						//schema_attribute_print(possibleAttr[k]);
						k++;
					}
				}
			}
		}
		removeData(q);
	}
	possibleAttrVertex->number = maximum;
	for (int i=0; i< possibleAttrVertex->number; i++){
		printf ("The attributes that are in largest common -> ");
		schema_attribute_print(possibleAttr[i]);
		schema_attribute_insert(possibleSchema, possibleAttr[i]);
		printf ("\n");
	}
	//schema_print(possibleSchema);
	//printf ("\n");
	return possibleSchema;
}

void cst_grow(graph_t g, schema_t s){
	vertex_t root = g->v;
	queue_t q = getQueue(g, root);
	graph_t subTree = (graph_t) malloc(sizeof(struct graph));
	graph_init(subTree);
	
	//insert root into subtree
	graph_insert_vertex(subTree, root);
	
	//dealing with root first 
	attrVertex_t rootAttrVertex = getVertexAttr(root);
	attribute_t *rootAttr = rootAttrVertex->v;
	//largest schema
	attribute_t largestSchemaAttr;
	attribute_t attr;
	int count = 0;
	int numberOfS = 0;
	for (attr = s->attrlist; attr != NULL; attr = attr->next){
		//schema_attribute_print(attr);
		numberOfS++;
	}
	//printf ("\n");

	for (int i=0; i< rootAttrVertex->number; i++){
		for (largestSchemaAttr = s->attrlist; largestSchemaAttr != NULL; largestSchemaAttr = largestSchemaAttr->next){
			if (strcmp(rootAttr[i]->name, largestSchemaAttr->name)==0){
				count++;
				
				if (count == numberOfS){
					subTree->sv = s;
				}
			}
		}
	}
	
	//printf ("%d", count);
	/*if (count == numberOfS){
		subTree->sv = s;
	}*/

	
	while (!isEmpty(q)){
		int c = 0;
		int index = 0;
		
		vertex_t nodeInQ = peek(q);
		attrVertex_t nodeAttrVertex = getVertexAttr(nodeInQ);
		attribute_t *nodeAttr = nodeAttrVertex->v;
		//comparing attributes in root and nodes in q
		//q裡的點
		attribute_t node;
		for (int i=0; i< nodeAttrVertex->number; i++){
			for (node = s->attrlist; node != NULL; node = node->next){
				if (strcmp(nodeAttr[i]->name, node->name)==0){
					c++;
					if (c == numberOfS){
						edge_t e = (edge_t) malloc(sizeof(struct edge));
						subTree->sv = s;
						graph_insert_vertex(subTree, nodeInQ);
						edge_set_vertices(e, root->id, nodeInQ->id);
						graph_insert_edge(subTree, e);
					}
	
				}
			}
		}
		/*if (c == numberOfS){
			edge_t e = (edge_t) malloc(sizeof(struct edge));
			subTree->sv = s;
			
			graph_insert_vertex(subTree, nodeInQ);
			edge_set_vertices(e, root->id, nodeInQ->id);
			graph_insert_edge(subTree, e);
		}*/
		
		removeData(q);
	}
	graph_print(subTree, 0);
	printf ("\n");

}
