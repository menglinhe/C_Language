/*
 ============================================================================
 Name        : sdb.c
 Author      : Menglin HE, Student ID: 260863025
 Version     :
 Copyright   : Your copyright notice
 Description : A simple database program using binary tree for record storage.
 	 	 	   And sorting the data based on the user command.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define MAXLEN 100

// student record node
struct StudentRecord{
	char first [MAXLEN];	// store first name
	char last [MAXLEN];		// store last name
	int ID;					// store student ID
	int marks;				// store marks

	struct StudentRecord *left;		// left tree
	struct StudentRecord *right;	// right tree
};

// a function to add the data into the binary tree, type is to check the input is an int(-> ID) or a char(-> last name)
void addNode(struct StudentRecord** root, struct StudentRecord* input, int type){

	// check if root is null
	if ((*root) == NULL){
		// create a root with input value
		(*root) = (struct StudentRecord*)malloc(sizeof(struct StudentRecord));	// dynamic allocation of memory
		(*root)->ID = input -> ID;			// for the int, we can directly store the data in binary tree
		(*root)->marks = input -> marks;
		strcpy((*root)->first, input -> first);	// for the char, need to use strcpy() to copy the string to the tree
		strcpy((*root)->last, input -> last);

		(*root)->left = NULL;	// initialize the left and right tree
		(*root)->right = NULL;
	}
	else{		// check the input is a number or a string
		if (type == 1){
			if (input->ID < (**root).ID){		// input less than root, add to the left tree
				addNode(&((*root)->left), input, type);
			}
			else{								// else, add to the right tree
				addNode(&((*root)->right), input, type);
			}
		}
		if (type != 1){		// strcmp() -> compare 2 strings
			if (strcmp(input->last,(**root).last) < 0){	// <0 -> input string "less than" root, add to left tree
				addNode(&((*root)->left), input, type);
			}
			else{
				addNode(&((*root)->right), input, type);
			}
		}
	}
}

// inorder traverse the tree
void traverse(struct StudentRecord* root){
	// if there is left tree traverse left
	if (root->left != NULL){
		traverse(root->left);
	}
	// print the middle
	printf("%s, %s\t\t\t%d\t%d\n", root->first, root->last, root->ID, root->marks);
	// if there is right tree traverse right
	if (root->right != NULL){
		traverse(root->right);
	}
}

// need two separate search function because the way to handle the string and int are different
// for string, need to compare the 2 string using strcmp()
// function to search the given last name
struct StudentRecord* searchName(struct StudentRecord* root, char* keyName){
	if (root == NULL || strcasecmp(root->last,keyName) == 0){	// root is null or happen to be the name we are searching
		return root;
	}
	if	// strcasecmp() is case insensitive, do not care about the lower/upper case
	(strcasecmp(keyName, root->last) < 0){	// key name is "less than" the root, search left tree
		return searchName(root->left, keyName);
	} else										// else, search right tree
	{
		return searchName(root->right, keyName);
	}
}

// function to search the given student ID
struct StudentRecord* searchID(struct StudentRecord* root, int keyID)
{
	if (root==NULL || root->ID == keyID){
		return root;
	}
	if (keyID < root->ID ){
		return searchID(root->left, keyID);
	} else
	{
		return searchID(root->right, keyID);
	}
}

int main(int argc, char *argv[]){
	struct StudentRecord data, *rootName, *rootID;
	FILE *NamesIDs;
	FILE *Marks;

	if ((NamesIDs = fopen(argv[1], "r")) == NULL){			// open NamesIDs file
		printf("Can't open %s\n", argv[1]);
		return -1;
	}

	if ((Marks = fopen(argv[2], "r")) == NULL){				// open Marks file
		printf("Can't open %s\n", argv[2]);
		return -2;
	}

	rootName = NULL;	// initialize two binary trees
	rootID = NULL;
	int numrecords = 0;
	printf("Building database...\n");

	while 				// scan record into structure
		(fscanf(NamesIDs, "%s%s%d", &(data.first[0]), &(data.last[0]), &(data.ID))!= EOF){
		fscanf(Marks, "%d", &(data.marks));			// marks too
		numrecords++;

		addNode(&rootName, &data,2);				// copy to binary tree sorted by last
		if (rootName == NULL)
		{
			printf("Error creating name B-Tree, aborted. \n");
			return -3;
		}
		addNode(&rootID, &data, 1);					// copy to binary tree sorted by ID
		if (rootID == NULL)
		{
			printf("Error creating ID B-Tree, aborted. \n");
			return -4;
		}
	}
	fclose(NamesIDs);
	fclose(Marks);
	printf("Finished...\n\n\n");

	char command[100];	// a string to store the user input
	printf("LN\t List all the records in the database ordered by last name.\nLI\t List all the records in the database ordered by student ID\n"
			"FN\t Prompts for a name and lists the record of the student with the corresponding name.\nFI\t Prompts for a name and lists the "
			"record of the student with corresponding ID.\nHELP\t Prints this list.\n?\t Prints this list\nQ\t Exits the program.\n");

	printf("\nPlease enter the command to sort the Student Record Database. \n");

	// need to stay in the while loop until user wants to exit the program
	while (1){
		scanf("%s", command);	// scan user input
		//		printf("%s",command);
		printf("\n\n");

		if (strncasecmp(command, "LN", 2) == 0)	// if the user input is LN, traverse the tree by last name
		{
			printf("Student Record Database sorted by Last Name.\n");
			traverse(rootName);
		}

		else if (strncasecmp(command, "LI", 2) == 0)	// traverse the tree by ID
		{
			printf("Student Record Database sorted by Student ID.\n");
			traverse(rootID);
		}
		else if (strncasecmp(command, "FN", 2) == 0)	// search the tree by corresponding last name
		{
			char search[100];
			struct StudentRecord *s;
			printf("Enter last name to search: \n");
			scanf("%s", search );
			s = searchName(rootName, search);	// call searchName function to search corresponding student name
			if (s!= NULL){		// print the result if the last name is in the binary tree
				printf("Student Name: \t%s %s\nStudent ID: \t%d\nTotal Grade: \t%d\n",s->first, s->last, s->ID, s->marks);
			}
			else{				// the last name does not exist in the binary tree
				printf("There is no student with that name.\n");
			}
		}

		else if (strncasecmp(command, "FI",2) == 0)		// search the tree by corresponding student ID
		{
			int search = 0;
			struct StudentRecord *s;
			printf("Enter ID to search: \n");
			scanf("%d",&search);
			s = searchID(rootID, search);
			if (s != NULL){
				printf("Student name: \t%s %s\nStudent ID: \t%d\nTotal Grade: \t%d\n", s->first, s->last, s->ID, s->marks);
			}
			else{
				printf("There is no student with that ID.\n");
			}

		}
		// combine the printf for HELP and ?, make command HELP case insensitive with lower/upper case letter and #of letters types
		else if ((strncasecmp(command, "HELP", 1) == 0) || (strncasecmp(command, "HELP", 2) == 0) ||  (strncasecmp(command, "HELP", 3) == 0) ||
				(strncasecmp(command, "HELP", 4) == 0) ||(strcmp(command, "?") == 0))
		{
			printf("LN\t List all the records in the database ordered by last name.\nLI\t List all the records in the database ordered by student ID\n"
					"FN\t Prompts for a name and lists the record of the student with the corresponding name.\nFI\t Prompts for a name and lists the "
					"record of the student with corresponding ID.\nHELP\t Prints this list.\n?\t Prints this list\nQ\t Exits the program.\n");

		}
		else if (strcasecmp(command, "Q") == 0)	// exit the program
		{
			printf("Program terminated...\n");
			exit(0);
		}

		else		// if user input is different from the defined command, error message
		{
			printf("Error, invalid command. \n");
		}
		printf("\nPlease enter the command to sort the Student Record Database. \n");

	}

	return 0;
}



