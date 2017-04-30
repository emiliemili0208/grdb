#ifndef __CVERT_H
#define __CVERT_H

//include file that would be use in my code
#include "graph.h"
#include "schema.h"
#include "tuple.h"
#include "vertexid.h"
#include "queue.h"

struct attrVertex{
	attribute_t *v;
	int number;
};

typedef struct attrVertex *attrVertex_t;


//找到最小id的vertex,加入cst	
//從neighbor找到與此vertex的最大vertex schema的點,加入cst(g(new graph))
//兩點間的edge schema
//project g out of G
//再繼續找有相關schema的neighbor
queue_t getQueue(graph_t g, vertex_t root);
attrVertex_t getVertexAttr(vertex_t root);
schema_t largest_common_vertex(graph_t g, queue_t q, vertex_t root);
void cst_grow(graph_t g, schema_t s);

#endif

/*attribute_t* getVertexAttr(vertex_t root){
	int i, offset, val;
	float fval;
	double dval;
	tuple_t t = root->tuple;
	attribute_t attr;
	attribute_t* rootAttr = (attribute_t *)malloc(MAX);
	int index = 0;
	for (attr = t->s->attrlist; attr != NULL; attr = attr->next) {
		offset = tuple_get_offset(t, attr->name);
		if (offset >= 0) {
			switch (attr->bt) {
			case CHARACTER:
				if (tuple_get_char(t->buf + offset)!=0){
					rootAttr[index] = attr;	
				}
				break;

			case VARCHAR:
				if (((char *)(t->buf + offset)) != ""){
					rootAttr[index] = attr;
				}
				break;

			case BOOLEAN:
				val = tuple_get_bool(t->buf + offset);
				if (val != 0){
					rootAttr[index] = attr;
				}
				break;

			/*case ENUM:
				if (tuple_print_enum(t, offset, el) != 0){
					rootAttr[index] = attr;
				}
				break;*/

			/*case INTEGER:
				i = tuple_get_int(t->buf + offset);
				if (i != 0){
					rootAttr[index] = attr;
				}
				break;

			case FLOAT:
				fval = tuple_get_float(t->buf + offset);
				if (fval != 0.0){
					rootAttr[index] = attr;
				}
				break;

			case DOUBLE:
				dval = tuple_get_double(t->buf + offset);
				if (dval != 0.0){
					rootAttr[index] = attr;
				}
				break;

			case DATE:
				break;
			
			case TIME:
				break;

			case BASE_TYPES_MAX:
				break;
			}
			printf ("%s\n",rootAttr[index]);
			printf ("\n");
		}
		index++;
	}
	printf (rootAttr[0]);
	return rootAttr;
}*/
