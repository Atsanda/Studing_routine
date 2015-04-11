#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST( name, cond ) do{ \
		printf("TEST\ \t%s\ \t\t%s\n", (cond)?" PASSED":" FAILED", name); \
	}while(0)

int main()
{
	hash_table* a = create(4);
	int j = 0;
	TEST(" A table was created", a != NULL);
	for( j; j < 4 && (a->tbl)[j] == NULL; j++){;}
	TEST(" A table is emty, size = 4", j == 4);
	
	//testing add() function
	TEST(" Usage of add() with wrong parameters, hash_table* = NULL", add(NULL, "Peter", 1) ==  -1);
	TEST(" Usage of add() with wrong parameters, key  = NULL", add( a, NULL , 1) ==  -1);
	
	add( a, "Peter", 1);
	j = 0;
	for( j; j < 4 && (a->tbl)[j] == NULL; j++){;}
	int tmp  = j;
	for( j=j+1; j<4 && (a->tbl)[j] == NULL; j++){;}
	TEST(" add() works correctly when table is empty", tmp  == hash(a, "Peter") && j == 4 && ((a->tbl)[hash(a,"Peter")])->data  == 1);	
	TEST(" element with already existing key can not be added",  add(a, "Peter", 2) == -1);	

	add( a, "Grisha", 2);
	j = 0;
	for( j; j < 4 && (a->tbl)[j] == NULL; j++){;}
	tmp  = j;
	for( j=j+1; j<4 && (a->tbl)[j] == NULL; j++){;}
	TEST(" add() works correctly in case of collision", tmp == hash(a,"Grisha") && (((a->tbl)[tmp])->next)->data  == 2 && j == 4);
	
	//testing find() function
	TEST(" Usage of find() with wrong parameters, hash_table* = NULL", find(NULL, "Peter") ==  NULL);
	TEST(" Usage of find() with wrong parameters, key = NULL", find( a, NULL ) ==  NULL);
	TEST(" find() searchs correctly", *find(a, "Grisha") == 2 && *find(a, "Peter") == 1);


	//testing delete() function
	TEST(" Usage of delete() with wrong parameters, hash_table* = NULL", delete(NULL, "Peter") ==  -1);
	TEST(" Usage of delete() with wrong parameters, key = NULL", delete( a, NULL ) ==  -1);
	delete(a, "Peter1");
	delete(a, "Grisha");
	for( j=0 ; j < 4 && (a->tbl)[j] == NULL; j++){;}

	TEST(" delete() worls correctly with normal parameters", find(a, "Grisha") == NULL && find(a, "Peter") == NULL && j == 4);
	TEST(" delete() can not delete  non-existing elem", delete(a, "MRSMB") == -1);
	
	char ch = 'a';
	for(ch; ch < 'z'; ch++)
		add( a, &ch, ch);
	for(ch = ch; ch >= 'a'; ch--)
		delete( a, &ch);
	for( j=0 ; j < 4 && (a->tbl)[j] == NULL; j++){;}

	TEST(" test with delete and add", j == 4);

	//testing iterator 
	add( a, "Peter", 1);
	add( a, "Grisha", 2);
	add( a, "Artyom", 3); 	
	int sum = 0;	
	
	iterator it = begin(a);
	TEST(" Iterator.begin() works correctly", get_data(it) == 1 && !strcmp("Peter", get_key(it)) );
	it = next(a, it);
	iterator it2 = next(a, it);	
	TEST(" Iterator.next() works correctly", get_data(it) == 2 && !strcmp("Grisha", get_key(it)) && get_data(it2) == 3 && !strcmp("Artyom", get_key(it2))); 
	iterator i;
	for( i = begin(a);i != end(); i = next(a, i) ){
		sum += get_data(i);
	}

	TEST(" Iterator detours correctly", sum == 6);

	i = begin(a);

	TEST(" get_key works correctly", !strcmp(get_key(i),"Peter"));
	TEST(" Impossible to delete when iterator points(case1)", delete(a, "Peter") == -1);
	i = next( a,i );
	TEST(" Impossible to delete when iterator points(case2)", delete(a, "Grisha") == -1);
	i = next( a, i);
	TEST(" Possible to delete when iterator does not point", delete( a, "Grisha") != -1  && find(a, "Grisha") == NULL);

	hash_table* b = create(55);
	for(ch = 'C'; ch < 'Z'; ch++)
		add( b, &ch, ch-'C'+1);

	iterator k = begin(b);
	ch = 'C';
	for( k, ch; ch < 'Z' && k != end() ; ch++, k = next(b, k))
		sum += get_data(k);
	
	TEST(" Iterator works correcly", sum == 282);
	
	destroy(a);
	destroy(b);
	
	return 0;
}
