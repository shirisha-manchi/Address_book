#include "addressbook.h"

//Reads contact data from CSV file and loads it into the AddressBook structure.
void pull_file_data(struct AddressBook * addressBook, char * file_name)
{
    FILE *fp = fopen(file_name, "r");
    int i=0;
    addressBook->contactCount = 0;
    fseek(fp,0,SEEK_END); //moving offset to EOF
    if(ftell(fp) != 0)//IF file is not having any content 
    {
        rewind(fp);//moving offset to index 0 and inserting the data
        while(!feof(fp))
        {
            fscanf(fp,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            i++;
        }
    }
    addressBook->contactCount = i;

    fclose(fp);
}

//Writes all contacts from AddressBook into CSV file
void push_data_to_file(struct AddressBook * addressBook, char * file_name)
{
    FILE *fp = fopen(file_name, "w");
    for(int i=0; i< addressBook->contactCount; i++)
    {
        fprintf(fp, "%s,", addressBook->contacts[i].name);
        fprintf(fp, "%s,", addressBook->contacts[i].phone);
        fprintf(fp, "%s\n", addressBook->contacts[i].email);
    }

    fclose(fp);
}

// Adds a new contact after validating mobile number and email.
void add_contact(struct AddressBook *addressBook)
{
    char name[50], mobile[30], gmail[50];
    printf("Enter the name: ");
    scanf(" %[^\n]", name);

    printf("Enter the mobile.no: ");
    int ret;
    do
    {
        scanf("%s",mobile);
        ret = (mobile_verify(mobile) && check_duplicate_mobile(addressBook, mobile));
        if(!ret)
        {
        printf("Invalid mobile\n");
        printf("Enter correct mobile no: ");
        }

    }while(ret != 1);

    printf("Enter the gmail: ");
    do
    {
        scanf("%s",gmail);
        ret = gmail_verify(addressBook, gmail);
        if(!ret)
        {
            printf("Invalid gmail\n");
            printf("Enter correct gmail: ");
        }
    }while(ret != 1);
    
    //pushing data into arr of structure

    int i = addressBook->contactCount;
    strcpy(addressBook->contacts[i].name,name);
    strcpy(addressBook->contacts[i].phone,mobile);
    strcpy(addressBook->contacts[i].email,gmail);

     // Increase contact count
    (addressBook->contactCount)++;

}

//Displays search menu and calls appropriate search function.
void search_contact(struct AddressBook *addressBook)
{
    
    if(addressBook->contactCount == 0)
    {
        printf("No data available to search\n");
        return;
    }
    
    int choice;
    // Reset index record size before every search
    addressBook->ir_size = 0;

	printf("\nSearching Menu:\n");
	printf("1. Search by name\n");
	printf("2. Search by mobile.no\n");
	printf("3. Search by email\n");
	printf("Enter your choice: ");
        
	scanf("%d", &choice);

	switch (choice) 
	{
	    case 1:
        char name[50];
        printf("Enter the name: ");
        scanf("%s",name);
		search_by_name(addressBook, name);
		break;
	    case 2:
        char phone[20];
        printf("Enter the phone no: ");
        scanf("%s",phone);
        // Validate mobile number before search
        if(mobile_verify(phone))
        {
		    search_by_phone(addressBook, phone);
        }
        else
        {
            printf("Invalid phone no\n");
        }
		break;
	    case 3:
        char gmail[50];
        printf("Enter the gmail: ");
        scanf("%s",gmail);

        // Validate gmail number before search
        if(gmail_verify(addressBook, gmail))
        {
		    search_by_gmail(addressBook, gmail);
        }
        else
        {
            printf("Invalid gmail\n");
        }
		break;
	    default:
		printf("Invalid choice. Please try again.\n");
	}
}

//Allows editing of a selected contact after search.
void edit_contact(struct AddressBook *addressBook)
{
    if(addressBook->contactCount == 0)
    {
        printf("No data available to edit\n");
        return;
    }
    int choice;
   
    search_contact(addressBook);
    // If no contact found, stop editing
    if(addressBook->ir_size == 0)
        return;

    int sno;
    printf("Enter one s.no contact: ");
    scanf("%d",&sno);

    // Validate serial number
    if (sno < 1 || sno > addressBook->ir_size)
    {
        printf("Invalid serial number\n");
        return;
    }
    // Display selected contact
    printf("Selected contact : \n");
    printf("%d.%s,%s,%s\n",sno,addressBook->contacts[addressBook->index_record[sno-1]].name, addressBook->contacts[addressBook->index_record[sno-1]].phone, addressBook->contacts[addressBook->index_record[sno-1]].email);

    do
    {
        printf("\nEditing Menu:\n");
	    printf("1. Edit name\n");
	    printf("2. Edit mobile.no\n");
	    printf("3. Edit email\n");
	    printf("4. Exit\n");
	    printf("Enter your choice: ");
        
	    scanf("%d", &choice);
        int ret;
        switch(choice)
        {
            case 1:
            char new_name[50];
            printf("Enter the new name: ");
            scanf("%s",new_name);
            strcpy(addressBook->contacts[addressBook->index_record[sno-1]].name, new_name);
            break;

            case 2:
            char new_num[15];
            do
            {
                printf("Enter the new num: ");
                scanf("%s",new_num);

                ret = (mobile_verify(new_num) && check_duplicate_mobile(addressBook, new_num));
                if(ret)
                {
                    strcpy(addressBook->contacts[addressBook->index_record[sno-1]].phone, new_num);
                }
                else
                {
                    printf("Invalid no\n");
                }
                
            }while(!ret);
            break;

            case 3:
            char new_gmail[15];
            do
            {
            printf("Enter the new gmail: ");
            scanf("%s",new_gmail);
            ret = gmail_verify(addressBook, new_gmail);
            if(ret)
            {
                strcpy(addressBook->contacts[addressBook->index_record[sno-1]].email, new_gmail);
            }
            else
            {
                printf("Invalis email\n");
            }
            }while(!ret);
            break;

            case 4:
            printf("Exiting...");
            break;

            default :
            printf("Invalid Choice\n");
        }
    }while(choice!=4);
}

//Deletes selected contact after confirmation.
void delete_contact(struct AddressBook *addressBook)
{
    if(addressBook->contactCount == 0)
    {
        printf("No data available to delete\n");
        return;
    }
    int choice;
    search_contact(addressBook);

    // If no contact found
    if (addressBook->ir_size == 0)
        return;
    
    int sno;
    char ch;
    printf("Enter one s.no contact: ");
    scanf("%d",&sno);
    if (sno < 1 || sno > addressBook->ir_size)
    {
        printf("Invalid serial number\n");
        return;
    }
    printf("Selected contact : \n");
    printf("%d.%s,%s,%s\n",sno,addressBook->contacts[addressBook->index_record[sno-1]].name, addressBook->contacts[addressBook->index_record[sno-1]].phone, addressBook->contacts[addressBook->index_record[sno-1]].email);
    
    // If user confirms deletion
    printf("Are you sure?\n y / n ? ");
    scanf(" %c",&ch);
    if(ch=='n' || ch=='N')
        return;
    else if(ch == 'y' || ch =='Y')
    {
        // Shift contacts left to remove selected one
        for(int i= addressBook->index_record[sno-1];i<(addressBook->contactCount)-1;i++)
        {
            addressBook->contacts[i]=addressBook->contacts[i+1];
        }
        addressBook->contactCount--;
    }
    else
    {
        printf("Invalid Choice\n");
        return;
    }
}

//Displays all contacts.
void list_contacts(struct AddressBook *addressBook)
{
    for(int i=0;i<addressBook->contactCount;i++)
    {
        printf("%s,%s,%s\n",addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
}

//mobile verification
int mobile_verify(char mobile[])
{
    if(strlen(mobile) !=10)
        return 0;
    if(mobile[0]<'5' || mobile[0]>'9')
        return 0;
    int i=0;
    while(mobile[i])
    {
        if(mobile[i]>'9' || mobile[i]<'0')
        {
            return 0;
        }
        i++;
    }
    return 1;
}

//email verification
int gmail_verify(struct AddressBook * addressbook, char gmail[])
{
    
    
    if(gmail[0]==' ' || gmail[0] == '\t' || gmail[0]=='@')
        return 0;
    
    char * cptr = strstr(gmail, "@gmail.com");
    if(cptr == NULL)
        return 0;
    else
    {
        if(strcmp("@gmail.com",cptr) != 0)
        return 0;

    }
    for(int i=0;i<addressbook->contactCount;i++)
    {
        if(strcmp(gmail,addressbook->contacts[i].email)==0)
            return 0;
    }
    return 1;
}

//SEARCHING BY NAME
void search_by_name(struct AddressBook * addressBook,char name[])
{
    
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if((strcmp(addressBook->contacts[i].name,name))==0)
        {
            addressBook->index_record[addressBook->ir_size++]=i;
        }    
    }

    if( addressBook->ir_size == 0 )
    {
	    printf("Contact not found\n");
    }
    else
    {
        for(int i=0;i<addressBook->ir_size;i++)
        {
            printf("%d.%s,%s,%s\n",i+1,addressBook->contacts[addressBook->index_record[i]].name, addressBook->contacts[addressBook->index_record[i]].phone, addressBook->contacts[addressBook->index_record[i]].email);
        }
    }
}

//SEARCH BY MOBILE
void search_by_phone(struct AddressBook * addressBook,char phone[])
{
    
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if((strcmp(addressBook->contacts[i].phone,phone))==0)
        {
            addressBook->index_record[addressBook->ir_size++]=i;
            break;
        }    
    }

    if( addressBook->ir_size == 0 )
	    printf("Contact not found\n");
    else
    {
        for(int i=0;i<addressBook->ir_size;i++)
        {
            printf("%d.%s,%s,%s\n",i+1,addressBook->contacts[addressBook->index_record[i]].name, addressBook->contacts[addressBook->index_record[i]].phone, addressBook->contacts[addressBook->index_record[i]].email);
        }
    }
}


//SEARCH BY GMAIL
void search_by_gmail(struct AddressBook * addressBook,char gmail[])
{   
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if((strcmp(addressBook->contacts[i].email,gmail))==0)
        {
            addressBook->index_record[addressBook->ir_size++]=i;
            break;
        }    
    }

    if( addressBook->ir_size == 0 )
	    printf("Contact not found\n");
    else
    {
        for(int i=0;i<addressBook->ir_size;i++)
        {
            printf("%d.%s,%s,%s\n",i+1,addressBook->contacts[addressBook->index_record[i]].name, addressBook->contacts[addressBook->index_record[i]].phone, addressBook->contacts[addressBook->index_record[i]].email);
        }
    }

}

//WHILE EDITING AND ADDING MAKING SURE THAT ENTERED NUM IS NOT EXISTING IN DATABASE
int check_duplicate_mobile(struct AddressBook * addressBook, char mobile[])
{
    for(int i=0;i<addressBook->contactCount;i++)
        {
            if(strcmp(mobile,addressBook->contacts[i].phone)==0)
                return 0;
        }

    return 1;
}
