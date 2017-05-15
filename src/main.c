#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "headers/hashmap.h"
#include "src/headers/person.h"

HASHMAP_FUNCS_CREATE(people, const char, person);

#define STOP 0
#define INSERT_PERSON 1
#define KILL_PERSON 2

struct hashmap family_tree;

/* Message used for inserting new person into tree */
typedef struct insert_person_message {
    long int my_msg_type;
    person person;
}insert_person_msg;

/* Message used for killing person with PIN */
typedef struct kill_person_message{
    long int my_msg_type;
    char pin[10];
}kill_person_msg;

/* Message specifying next operation */
typedef struct generic_message{
    long int my_msg_type;
    char operation[1];
}generic_msg;


int main(void)
{
    hashmap_init(&family_tree, hashmap_hash_string,hashmap_compare_string,0);
    intializeTree();

    int running = 1;
    int msgid;
    generic_msg generic_messages;
    insert_person_msg insert_person_message;
    kill_person_msg kill_person_message;

    long int msg_to_recieve = 0;


    char* operations[3]={
        "Stop server",
        "Insert person",
        "Kill person"
    };

    /* Let us set up the message queue */
    msgid = msgget(ftok("pesho",(key_t)1234), 0600| IPC_EXCL | IPC_CREAT);

    if (msgid == -1) {
        perror("msgget failed with error");
        exit(EXIT_FAILURE);
    }

    /* Then the messages are retrieved from the queue, until an end message is
     * encountered. lastly the message queue is deleted
     */

    while(running) {
        fprintf(stderr,"INFO: Server has started.");
        /* Read operation which has to be executed */
        if (msgrcv(msgid, &generic_messages, sizeof(generic_msg),
                   msg_to_recieve, 0) == -1) {
            perror("msgcrv failed with error");
            exit(EXIT_FAILURE);
        }
        printf("INFO: %s", operations[generic_messages.my_msg_type]);

        switch (generic_messages.my_msg_type) {
        case INSERT_PERSON:
            /* Read send from client message */
            if (msgrcv(msgid, &insert_person_message, sizeof(insert_person_msg),
                       msg_to_recieve, 0) == -1) {
                perror("msgcrv failed with error");
                exit(EXIT_FAILURE);
            }

            give_birth(&insert_person_message.person);

            /* Override memmory of insert_person in order not to messup on next query */
            memset(insert_person_message.person.name,0,sizeof(insert_person_message.person.name));
            memset(insert_person_message.person.date_of_birth,0,sizeof(insert_person_message.person.date_of_birth));
            memset(insert_person_message.person.parent_PIN[1],0, sizeof(insert_person_message.person.parent_PIN[1]));
            memset(insert_person_message.person.parent_PIN[0],0, sizeof(insert_person_message.person.parent_PIN[0]));
            memset(insert_person_message.person.pin,0, sizeof(insert_person_message.person.pin));


            break;
        case KILL_PERSON:
            /* Read send from client message */
            if (msgrcv(msgid, &kill_person_message, sizeof(kill_person_msg),
                       msg_to_recieve, 0) == -1) {
                perror("msgcrv failed with error");
                exit(EXIT_FAILURE);
            }

            kill_person(kill_person_message.pin);
            /* Override memmory of insert_person in order not to messup on next query */
            memset(kill_person_message.pin,0,sizeof(kill_person_message.pin));

            break;
        case STOP:
            running = 0;
        default:
            break;
        }

    }

    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        perror("msgctl(IPC_RMID) failed");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}


void kill_person(const char *key){
    person* p = people_hashmap_get(&family_tree,key);
    if(p){
        people_hashmap_remove(&family_tree,key);
        free(p);
    }
}

void give_birth(person *person){
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
//    struct hashmap_iter *iter;
//    iter = hashmap_iter(&family_tree);
//    while(iter){
//        person *pesho = people_hashmap_iter_get_data(iter);
//        const char *kluch = people_hashmap_iter_get_key(iter);
//        iter = hashmap_iter_next(&family_tree, iter);
//    }

//    person* pesho = people_hashmap_get(&family_tree,parent_PIN_1[1]);
//    printf("%s\n", pesho->name);
}
