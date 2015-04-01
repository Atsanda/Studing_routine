#ifndef _HASH_TABLE_
#define _HASH_TABLE_

typedef struct node{
	int 		data;
	char* 	 	key;
	struct node 	*next;
	char 		ITR_FLAG;//1 if there's an iterator which points at this node, else 0. u can not destroy the node with this flag at 1 position
} node;

typedef struct hash_table{
	node **tbl;
	int  size;
} hash_table;

hash_table* 	create	( int size );
int 		add	( hash_table *hsh_tbl, char* key, int data );//returns -1 in case of error, in case of success returns 0. Elements with the same key can not be added
int* 		find	( hash_table *hsh_tbl, char* key );//returns NULL in case of error or mismatch, in case of success returns data pointer 
int 		delete	( hash_table *hsh_tbl, char* key );//returns NULL in case of error or mismatch, in case of success returns 0. 
void 		destroy	( hash_table *hsh_tbl );
unsigned int	hash	( hash_table *hsh_tbl, char* key );

typedef node* iterator;

iterator	begin	( hash_table *hsh_tbl, char* key );
iterator 	end	();
iterator	next	( iterator i );
int		get_data( iterator i );
char*		get_key	( iterator i );
#endif
