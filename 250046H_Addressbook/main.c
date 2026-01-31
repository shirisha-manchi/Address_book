/*
NAME		:   Shirisha Manchi
DATE		:   05/01/2026
DESCRIPTION	:   -> The Address Book Management System is a menu-driven C application designed to manage people’s contact information efficiently. 
				-> The project allows users to perform basic operations such as adding, searching, editing, deleting, and listing contacts. 
				-> Each contact record consists of three fields: Name, Mobile Number, and Email ID.
			    -> The application is implemented using core C programming concepts including structures, arrays, functions, pointers, 
				   conditional statements, loops, switch-case, and file handling. 
				-> Contact data is stored persistently in a CSV file (database.csv) using File I/O operations. 
				-> When the program starts, existing data is loaded from the file into an array of structures, and before exiting, all updated data 
				   is written back to the file to ensure data consistency.
			    -> The project is modular in design, with separate files for the main execution (main.c), function implementations (addressbook.c), 
				   and function declarations and structure definitions (addressbook.h). 
				-> Input validation is performed for mobile numbers and email addresses to ensure data correctness and avoid duplicates.
			    -> Overall, this project demonstrates effective use of structured programming, data validation, modular coding practices, and 
				   persistent data storage using files in C.
*/
#include "addressbook.h"

int main() 
{
    int choice;
    struct AddressBook addressBook;
	pull_file_data(&addressBook, "database.csv");
    do 
    {
		printf("\nAddress Book Menu:\n");
		printf("1. Add/Create contact\n");
		printf("2. Search contact\n");
		printf("3. Edit contact\n");
		printf("4. Delete contact\n");
		printf("5. List all contacts\n");
		printf("6. Exit\n");
		printf("Enter your choice: ");

		scanf("%d", &choice);

		switch (choice) 
		{
	    	case 1:
			add_contact(&addressBook);
			break;
	    	case 2:
			search_contact(&addressBook);
			break;
	    	case 3:
			edit_contact(&addressBook);
			break;
	    	case 4:
			delete_contact(&addressBook);
			break;
	    	case 5:
			list_contacts(&addressBook);
			break;
	    	case 6:
			printf("Saving and Exiting...\nThankyou😊\n");
			push_data_to_file(&addressBook, "database.csv");
			break;
	    	default:
			printf("❌Invalid choice. Please try again.\n");
		}
    } while (choice != 6);
	
	return 0;
}


