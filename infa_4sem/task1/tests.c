#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST( name, cond ) do{ \
		printf("TEST\ \t%s\ \t\t%s\n", (cond)?" PASSED":" FAILED", name); \
	}while(0)

int main()
{
	hash_table* a = create(4);
	TEST(" A table was created with size = 4", a->size == 4);
	
	//testing add() function
	TEST(" Usage of add() with wrong parameters, hash_table* = NULL", add(NULL, "Peter", 1) ==  -1);
	TEST(" Usage of add() with wrong parameters, key  = NULL", add( a, NULL , 1) ==  -1);
	add( a, "Peter", 1);
	TEST(" add() works correctly when table is empty", ((a->tbl)[hash(a,"Peter")])->data  == 1);
	TEST(" element with already existing key can not be added",  add(a, "Peter", 2) == -1);
	add( a, "Grisha", 2);
	TEST(" add() works correctly in case of collision", (((a->tbl)[hash(a,"Peter")])->next)->data  == 2);
	
	//testing find() function
	TEST(" Usage of find() with wrong parameters, hash_table* = NULL", find(NULL, "Peter") ==  NULL);
	TEST(" Usage of find() with wrong parameters, key = NULL", find( a, NULL ) ==  NULL);
	TEST(" find() searchs correctly", *find(a, "Grisha") == 2 && *find(a, "Peter") == 1);


	//testing delete() function
	TEST(" Usage of delete() with wrong parameters, hash_table* = NULL", delete(NULL, "Peter") ==  -1);
	TEST(" Usage of delete() with wrong parameters, key = NULL", delete( a, NULL ) ==  -1);
	add(a, "Artyomka", 3);
	delete(a, "Artyomka");
	delete(a, "Peter");
	delete(a, "Grisha");
	TEST(" delete() worls correctly with normal parameters", find(a, "Grisha") == NULL && find(a, "Peter") == NULL && find(a, "Artyomka") == NULL);
	TEST(" delete() can not delete  non-existing elem", delete(a, "MRSMB") == -1);


	//testing iterator 
	add( a, "Peter", 1);
	add( a, "Grisha", 2);	
	int sum = 0;	

	iterator i;
	for( i = begin(a,"Grisha");i != end(); i=next(i) )
		sum += get_data(i);
	
	TEST(" Iterator detours correctly", sum == 3);

	i = begin(a, "Grisha");

	TEST(" get_key works correctly", !strcmp(get_key(i),"Peter"));
	TEST(" Impossible to delete when iterator points(case1)", delete(a, "Peter") == -1);
	next(i);
	TEST(" Impossible to delete when iterator points(case2)", delete(a, "Grisha") == -1);
	
	destroy(a);
	
	return 0;
}
