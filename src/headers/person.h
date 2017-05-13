#ifndef PERSON_H
#define PERSON_H
typedef struct person{
    char pin[10];
    char *name;
    char date_of_birth[11];
    char parent_PIN[2][10];
    short killed;
}person;

void killPerson();
void giveBirth();
void intializeTree();

#endif // PERSON_H
