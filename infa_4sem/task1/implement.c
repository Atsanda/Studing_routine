#include "hash_table.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

hash_table* create(int size)
{
	assert(size >= 0);
	hash_table* new_tbl 	= calloc(1, sizeof(hash_table));
	new_tbl->tbl 		= calloc(size, sizeof(node*));
	new_tbl->size		= size;
}

void destroy(hash_table *hsh_tbl)
{
	assert(hsh_tbl != NULL);

	int i;
	node *tmp_nd;
	node *tbe_del;

	for(i=0; i < hsh_tbl->size; i++)
	{
		tmp_nd = (hsh_tbl->tbl)[i];

		if( tmp_nd == NULL )
			continue;
	
		while( tmp_nd->next != NULL)
		{
			tbe_del = tmp_nd;
			tmp_nd 	= tmp_nd->next;
			free(tbe_del->key);
			free(tbe_del);
		}
		free(tmp_nd->key);
		free(tmp_nd);
	}
	free(hsh_tbl->tbl);
	free(hsh_tbl);
}

int add( hash_table *hsh_tbl, char* key, int data )
{
	if(key == NULL)
        	return -1;
	if(hsh_tbl == NULL)
		return -1;
	if(find( hsh_tbl, key))
		return -1;
	
	unsigned int index = hash(hsh_tbl, key);

	node *tmp_nd = (hsh_tbl->tbl)[index];

	if( tmp_nd == NULL )
	{
		(hsh_tbl->tbl)[index] = calloc( 1, sizeof(node));
		tmp_nd = (hsh_tbl->tbl)[index];
		tmp_nd->data = data;
		tmp_nd->key  = calloc(1,strlen(key));
	        strcpy(tmp_nd->key, key);
		return 0;
	}

	while( tmp_nd->next !=  NULL )
		tmp_nd = tmp_nd->next;

	tmp_nd->next = calloc(1, sizeof(node));
	tmp_nd	     = tmp_nd->next;
	tmp_nd->data = data;
	tmp_nd->key  = calloc(1,strlen(key));
	strcpy(tmp_nd->key, key);

	return 0;
}

int* find(hash_table *hsh_tbl, char* key)
{
	if( key == NULL )
	        return NULL;
	if( hsh_tbl == NULL )
		return NULL;

	unsigned int index = hash(hsh_tbl, key);

	node *tmp_nd = (hsh_tbl->tbl)[index];

	if( tmp_nd == NULL)
		return NULL;	

	for(tmp_nd; tmp_nd != NULL; tmp_nd = tmp_nd-> next)
	{
		if( !strcmp(tmp_nd->key, key) )
			return &(tmp_nd->data);
	}

	return NULL;
}

int  delete(hash_table *hsh_tbl, char* key)
{
	if( key == NULL )
	        return -1;
	if( hsh_tbl == NULL )
		return -1;

	unsigned int index = hash(hsh_tbl, key);

	node *tmp_nd = (hsh_tbl->tbl)[index];
	node *tbe_del = NULL;
	
	if( tmp_nd == NULL )
		return -1;

	if( !strcmp(tmp_nd->key , key) ) 
	{
		if( tmp_nd->ITR_FLAG )
			return -1;

		(hsh_tbl->tbl)[index] = tmp_nd->next;
		free(tmp_nd->key);
		free(tmp_nd);
		return 0;
	}
	
	while( tmp_nd->next != NULL )
	{
		if( !strcmp((tmp_nd->next)->key , key) )
		{
			tbe_del 	= tmp_nd->next;
			if( tbe_del->ITR_FLAG )
				return -1;
			tmp_nd->next= (tmp_nd->next)->next;
			free(tbe_del->key);
			free(tbe_del);
			return 0;
		}
		tmp_nd = tmp_nd->next;
	}
	return -1;
}

unsigned int hash( hash_table *hsh_tbl, char* key)
{
	assert(hsh_tbl != NULL);
 	assert(key     != NULL);

	unsigned int hash = 0;

    	for(; *key; key++)
    	{
        	hash += (unsigned char)(*key);
        	hash += (hash << 10);
        	hash ^= (hash >> 6);
    	}
    	hash += (hash << 3);
    	hash ^= (hash >> 11);
    	hash += (hash << 15);

    	return hash%(hsh_tbl->size);
}

//iterator implemention
iterator begin( hash_table *hsh_tbl )
{
	assert(hsh_tbl != NULL);
	int j;
	for( j = 0; (hsh_tbl->tbl)[j] == NULL && j < (hsh_tbl->size) - 1 ; j++){;}

	(hsh_tbl->tbl)[j] -> ITR_FLAG = 1;
	return (hsh_tbl->tbl)[j];
}

iterator end()
{
	return NULL;
}

iterator next( hash_table *hsh_tbl, iterator i )
{
	assert(i != NULL);
	
	if( i->next != NULL )
	{
		(i->next)->ITR_FLAG = 1;
		i->ITR_FLAG = 0;
		return i->next;
	}
	if( i->next == NULL && hash( hsh_tbl, get_key(i)) < (hsh_tbl->size)-1 )
	{
		int j;
		for( j = hash( hsh_tbl, get_key(i)) + 1; (hsh_tbl->tbl)[j] == NULL && j < (hsh_tbl->size) - 1 ; j++){;}
		if( j == (hsh_tbl->size) -1 )
			return NULL;
		if( ((hsh_tbl->tbl)[j]) -> next  != NULL )
			((hsh_tbl->tbl)[j] -> next) -> ITR_FLAG = 1;
		i->ITR_FLAG = 0;

		return (hsh_tbl->tbl)[j];
	}
	else
	{
		i->ITR_FLAG = 0;
		return NULL;
	}
}

int get_data( iterator i )
{
	assert(i != NULL);
	return i->data;
}

char* get_key( iterator i )
{
	assert(i != NULL);
	return i->key;
}
