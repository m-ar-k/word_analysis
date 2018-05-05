#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int unique_words;

// Malloc function that checks for failure
void *emalloc(size_t n){
	void *p;
	p = malloc(n);
	if (p == NULL) {
		printf("%s\n", "Malloc error");
		exit(1);
	}
	return p;
}

// Copy a string represented by a char pointer
char *string_duplicator(char *input) {
	char *copy;
	if (input != NULL) {
		copy = (char *)malloc(sizeof(char) * strlen(input) + 1);
	}
	if (copy == NULL) {
		fprintf(stderr, "error in string_duplicator");
		exit(1);
	}
	strncpy(copy, input, strlen(input)+1);
	return copy;
}

// Define structure for a list node
typedef struct Nameval Nameval;
	struct Nameval {
	char *name;
	int value;
	int count;
	Nameval *next; /* in list */
};


// Create a new list
Nameval *newitem (char *name, int value, int count) {
	Nameval *newp;
	newp = (Nameval *) emalloc(sizeof(Nameval));
	newp->name = name;
	newp->value = value;
	newp->count = count;
	newp->next = NULL;
	return newp;
}

// Add a node to the front of the list
Nameval *addfront(Nameval *listp, Nameval *newp) {
	newp->next = listp;
	return newp;
}

// Add a node to the end of the list
Nameval *addend(Nameval *listp, Nameval *newp) {
	Nameval *p;
	if (listp == NULL) {
		return newp;
	}
	for (p = listp; p->next != NULL; p = p->next);
	p->next = newp;
	return listp;
}

// Free memory from all nodes in a list
void freeall(Nameval *listp) {
	Nameval *next;
	for ( ; listp != NULL; listp = next ) {
		next = listp->next;
		/* assume here the listp->name is freed someplace else */
		free(listp);
	}
}

// Creating a string to lower case function
char *upper_to_lower(char *input) {
  char *ptr = (char *)input;
  while (*ptr) {
     *ptr = tolower((char)*ptr);
      ptr++;
  }
  return input;
}

// Create a linked list with frequency of word length information
Nameval *analysis_list(Nameval *storage_list) {

	Nameval *new_list = NULL;
	Nameval *ptr2 = NULL;

		for (Nameval *ptr = storage_list; ptr != NULL; ptr = ptr->next) {
			int boolean = 0;
			// Initial case where no nodes exist
			if (new_list==NULL && ptr2==NULL && ptr!=NULL) {
				Nameval *newnode = newitem("sample", ptr->value, 0);
				new_list = addend(new_list, newnode);
			}
			// Incrementing count of existing numbers
			for(ptr2 = new_list; ptr2!=NULL; ptr2 = ptr2->next) {
				if (ptr2->value==ptr->value) {
					(ptr2->count)++;
					boolean = 1;
				}

			}
			// Adding a new node for values that do not already exist
			if (boolean==0) {
				Nameval *newnode = newitem("sample", ptr->value, 1);
				new_list = addend(new_list, newnode);
			}
	
		}

	return new_list;
}

// Sorting by word length and frequency
void sort(Nameval *frequency_list, int true) {
    int a, b;

    Nameval *temp1;
    Nameval *temp2;

    for(temp1=frequency_list;temp1!=NULL;temp1=temp1->next) {
        for(temp2=temp1->next;temp2!=NULL;temp2=temp2->next) { 
        	// Case for sorting by word length
          	if (true==0) {
           		if(temp2->value < temp1->value) {
                		a = temp1->value;
                		b = temp1->count;
                		temp1->value = temp2->value;
                		temp1->count = temp2->count;
                		temp2->value = a;
                		temp2->count = b;
              	 	}
          	}

          	// Case for sorting by frequency
          	if (true==1) {
          		if(temp2->count > temp1->count) {
                		a = temp1->value;
                		b = temp1->count;
                		temp1->value = temp2->value;
                		temp1->count = temp2->count;
                		temp2->value = a;
                		temp2->count = b;
              		 }
              	 // Revert to first sorting method if more than one shared frequency
              	 if (temp2->count==temp1->count && temp2->value < temp1->value) {
              	 	a = temp1->value;
                	b = temp1->count;
                	temp1->value = temp2->value;
                	temp1->count = temp2->count;
                	temp2->value = a;
                	temp2->count = b;
              	 }
         }
      }
   }
}

// Sort a list of words alphabetically
void sort_words_alphabetically(Nameval *words_list) {
	char * a;
    Nameval *temp1;
    Nameval *temp2;

    for(temp1=words_list;temp1!=NULL;temp1=temp1->next) {
        for(temp2=temp1->next;temp2!=NULL;temp2=temp2->next){ 
           	if(strncmp(temp1->name, temp2->name, words_list->value)>0){
                a = temp1->name;
                temp1->name = temp2->name;
                temp2->name = a;
            }
        }
    }
}

// Create list of unique words of a certain length to be printed
Nameval *print_words(Nameval *storage_list, int word_length) {
	Nameval *word_list = NULL;
	Nameval *ptr2 = NULL;
		for (Nameval *ptr = storage_list; ptr != NULL; ptr = ptr->next) {
			int boolean = 0;
			// Create first node for empty list
			if (word_list==NULL && ptr2==NULL && ptr!=NULL && ptr->value==word_length) {
				Nameval *newnode = newitem(ptr->name, ptr->value, 1);
				word_list = addend(word_list, newnode);
				unique_words++;
			}
			
			// Check if word already exists
			for(ptr2 = word_list; ptr2!=NULL && strlen(ptr2->name)==strlen(ptr->name); ptr2 = ptr2->next) {
				if (strncmp(ptr2->name, ptr->name, strlen(ptr->name))==0) {
					boolean = 1;
				}
			}
			
			// Create new node with new word if unique 
			if (boolean==0 && ptr->value==word_length) {
				Nameval *newnode = newitem(ptr->name, ptr->value, 1);
				word_list = addend(word_list, newnode);
				unique_words++;
			}
		}

		return word_list;
}

// Print out unique words in formatted form
void print_word_information(Nameval *wordlist) {
	sort_words_alphabetically(wordlist);
	printf("%s", " (words: ");
	while (wordlist!=NULL) {

		if (wordlist->next==NULL) {
			printf("%s%s%s","\"",wordlist->name,"\"");
			break;
		}
		else if (wordlist->next->next==NULL) {
			printf("%s%s%s","\"",wordlist->name,"\" and ");
			wordlist = wordlist->next;
			printf("%s%s%s","\"",wordlist->name,"\"");
			break;
		}
		printf("%s%s%s","\"",wordlist->name,"\", ");
		wordlist = wordlist->next;
	}
	printf("%s\n",")");
}
// Print output without --sort command, sorted by length lowest to highest
void words_analyzer(Nameval *storage_list, int words_true, int sort_type) {
	Nameval *frequency_list = analysis_list(storage_list);
	sort(frequency_list, sort_type);
	for (;frequency_list !=NULL;) {
		if (frequency_list->value>0) {
			printf("%s%02d%s%02d%s","Count[",frequency_list->value,"]=",frequency_list->count,";");
			if (words_true==1) {
				Nameval *wordlist = print_words(storage_list, frequency_list->value);
				print_word_information(wordlist);
				freeall(wordlist);
			}
			else{
				print_words(storage_list, frequency_list->value);
				printf("\n");
			} 
		}
		// Free nodes of list
		Nameval *temp = frequency_list;
		frequency_list = frequency_list->next;
		if(temp) free(temp);
	}
	freeall(frequency_list);
}

// A program that analyzes word length frequencies
// and prints out unique words
int main(int argc, char **argv) {
	FILE *infile = NULL; 

	if (argc < 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}

	int in_file_true = 0;
	int sort_true = 0;
	int print_words_true = 0;
	int file_location = 0;

	for (int i=1; i<argc; i++) {
		// Makes sure that if --infile is at end that program doesn't try to read outside the array
		if (strcmp(argv[i], "--infile")==0 && ((i+1<argc)&&(argv[i+1]!=NULL))) {
			in_file_true = 1;
			file_location = i+1;
			// If infile is in command then the input file name must come next
		}
		else if (strcmp(argv[i], "--sort")==0) {
			sort_true = 1;
			// sort the printout = true
		}
		else if (strcmp(argv[i], "--print-words")==0) {
			print_words_true = 1;
			// print out word information = true
		}
	}
	// Need to look out make sure it works at maximum arguments
	if (in_file_true==1) {
		infile = fopen(argv[file_location], "r");
		if (infile == NULL) {
			fprintf(stderr, "%s: cannot open %s", argv[file_location-1], argv[file_location]);
			exit(1);
		}
	}
	// Case for executing program with no arguments
	else {
		printf("%s\n","Invalid input");
		return 1;
	}
		
	// Creates 2D int array storing string lengths in row 0
	// And ASCII numeric representations of individual chars
	// that make up the strings in the rest of the array

	// Index of second element 0 references string length
	// Index of second element 1 references first char of string
	Nameval *storage_list = NULL;

	char * line = NULL;
	char * tokenized = NULL;
	size_t len = 0;
	size_t read;	

	// Read from file and split into string and put into linked list
	while ((read = getline(&line, &len, infile)) != -1) {
		tokenized = strtok(line, " ,.;()");
		// Set upper case letters to lower case
		upper_to_lower(tokenized);
		// Remove newline characters
		char *newline = strchr(tokenized, '\n');
		if (newline)*newline = 0;	
		Nameval *newnode = newitem(string_duplicator(tokenized), strlen(tokenized), 1);
		storage_list = addend(storage_list, newnode);
		while(tokenized!=NULL) {
			tokenized = strtok(NULL, " ,.;()");
			if (tokenized==NULL) break;
			// Set upper case letters to lower case
			upper_to_lower(tokenized);
			// Remove newline characters
			char *newline = strchr(tokenized, '\n');
			if (newline)*newline = 0;	
			// Add string to node and add to list
			Nameval *newnode = newitem(string_duplicator(tokenized), strlen(tokenized), 1);
			storage_list = addend(storage_list, newnode);
		}
	}

	if (line) free(line);

	// Case for sorting by least to greatest word length
	if (in_file_true==1){
		if (print_words_true==1) words_analyzer(storage_list, 1, sort_true);
	    else words_analyzer(storage_list, 0, sort_true);
	}
	
	// Free all memory from storage list
	while(storage_list!=NULL) {
		Nameval *temp = storage_list;
		storage_list = storage_list->next;
		free(temp->name);
		temp->value = 0;
		temp->count = 0;
		free(temp);
	}

	fclose(infile);
	printf("%s", "Number of unique words: ");
	printf("%d", unique_words);
	printf("\n");
	return 0;
}
