#include <stdio.h>
#include <string.h>
#include "headers/hashmap.h"
#include "src/headers/person.h"

HASHMAP_FUNCS_CREATE(people, const char, person);

struct hashmap family_tree;

int main(int argc, char* argv[]) {
    printf("Hello World!\n");

    hashmap_init(&family_tree, hashmap_hash_string,hashmap_compare_string,0);
    intializeTree();
    return 0;
}


void killPerson(const char *key){
    person* p = people_hashmap_get(&map,key);
    if(p){
        people_hashmap_remove(&map,key);
        free(p);
    }
}

void giveBirth(const person *person){
    people_hashmap_put(&family_tree,person->pin,person);
}

void intializeTree(){
    char *parent_PIN_2[5] = {
        "4301044022",
        "6908151488",
        "8904201212",
        "6702204444",
        "7205098888"
    };
    char *parent_PIN_1[5] = {
        "4301041199",
        "6908151353",
        "8904201313",
        "6702206663",
        "7205096665"
    };
    char *ime[10]={
        "pesho tramvaq","gosho topliq", "kiro svejiq","dinko picha","ico transformyra",
        "Penka Trybyta", "Lisa Ann", "Dora Dylbokata", "Mariq Gyrloto", "Chervenata Shapchica"
    };
    char *date_of_birth[5]={"04/01/1643","15/08/1769","20/04/1889","20/02/1967","09/05/1972"};

    int i;
    person* p;
    for(i=0;i<10;i++){
        p = (person*) malloc(sizeof(person));
        p->name = (char*) malloc(sizeof(char)*strlen(ime[i])+1);
        strcpy(p->name, ime[i]);
        strcpy(p->date_of_birth, date_of_birth[i%5]);
        p->killed = 0;
        if (i > 1 && i%5>1) {
            strcpy(p->parent_PIN[0], parent_PIN_1[i%5-1]);
            strcpy(p->parent_PIN[1] , parent_PIN_2[i%5-1]);
        }else {
            strcpy(p->parent_PIN[0], "2201044022");
            strcpy(p->parent_PIN[1] , "2201044011");
        }

        if(i<5){
            strcpy(p->pin, parent_PIN_1[i]);
            people_hashmap_put(&family_tree,p->pin,p);
            printf("INFO: inserted %s\n",p->pin);
        }else{
            strcpy(p->pin ,parent_PIN_2[i%5]);
            people_hashmap_put(&family_tree, p->pin,p);
            printf("INFO: inserted %s\n",p->pin);
        }
    }
    struct hashmap_iter *iter;
    iter = hashmap_iter(&family_tree);
    while(iter){
        person *pesho = people_hashmap_iter_get_data(iter);
        const char *kluch = people_hashmap_iter_get_key(iter);
        iter = hashmap_iter_next(&family_tree, iter);
    }

    person* pesho = people_hashmap_get(&family_tree,parent_PIN_1[1]);
    printf("%s\n", pesho->name);
}
