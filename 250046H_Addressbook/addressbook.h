#ifndef CONTACT_H
#define CONTACT_H
#include<stdio.h>
#include<string.h>

struct Contact
{
    char name[50];
    char phone[15];
    char email[50];
};

struct AddressBook 
{
    struct Contact contacts[100];
    int contactCount;
    int index_record[100];
    int ir_size;
};

void pull_file_data(struct AddressBook *, char *);
void push_data_to_file(struct AddressBook *, char *);
void add_contact(struct AddressBook *addressBook);
void search_contact(struct AddressBook *addressBook);
void edit_contact(struct AddressBook *addressBook);
void delete_contact(struct AddressBook *addressBook);
void list_contacts(struct AddressBook *addressBook);
int mobile_verify(char mobile[]);
int gmail_verify(struct AddressBook *, char gmail[]);
void search_by_name(struct AddressBook *,char name[]);
void search_by_phone(struct AddressBook *,char phone[]);
void search_by_gmail(struct AddressBook *,char gmail[]);
int check_duplicate_mobile(struct AddressBook *, char mobile[]);

#endif
