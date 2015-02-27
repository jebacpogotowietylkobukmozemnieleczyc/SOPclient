#ifndef DATA
#define DATA
typedef void(*Fptr)();

struct Message{
long type;
int action;
int pid;
char nick[16];
char text[1024];
};

struct Chat_message{
    char  name[16];
    time_t time;
    char text[2014];
};

typedef struct Chat_message Chat_message;
struct Chat{
    int count;
    int full ;
    Chat_message messages[100];

};




struct Manu{
    char name[16];
    int count_option;
    char option[9][32];
    Fptr function[9];
};

typedef struct Message Msg;
typedef struct Manu Menu;
typedef struct Chat Chat;

#endif // DATA

