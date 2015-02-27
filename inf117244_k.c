#include <stdio.h>
#include <stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include"inf117244_kdata.h"

Menu menu;
int run =1;
Msg rcvmsg;
Msg sndmsg;
int pid;
char status[10][32];
char action[10][64];

int showmenu(){

    int i;
    int r;
    for (i = 0; i < menu.count_option; ++i) {
        printf("%d: %s\n",i+1,menu.option[i]);
    }
    printf("Wybierz opcje: \n");
    scanf("%d",&r);
    return r;
}


void showusers(){
    sndmsg.action = 1;
}

void spm(){
    printf("Nick: ");
    scanf("%s",sndmsg.nick);
    printf("Text: ");
    scanf("%s",sndmsg.text);
    sndmsg.action = 2;
}

void sgm(){
    printf("Nazwa grupy: ");
    scanf("%s",sndmsg.nick);
    printf("Text: ");
    scanf("%s",sndmsg.text);
    sndmsg.action = 3;
}

void showgroups(){
    sndmsg.action = 4;
}


void changenick(){
    char newnick[16];
    printf("Nowy nick: ");
    scanf("%s",newnick);
    strcpy(sndmsg.nick,newnick);
    sndmsg.action = 5;
}


void joingroup(){
    char groupname[16];
    printf("Nazwa grupa: ");
    scanf("%s",groupname);
    strcpy(sndmsg.nick,groupname);
    sndmsg.action = 6;
}

void disjoingroup(){
    char groupname[16];
    printf("Nazwa grupa: ");
    scanf("%s",groupname);
    strcpy(sndmsg.nick,groupname);
    sndmsg.action = 7;
}

void logout(){
    sndmsg.action = 9;
    run =0;
}



//chat
Chat * chat;


void showchat(){
    printf("Liczba otrzymanych wiadomości: %d \n",chat->count);
    int i;
    if(chat->full==1){
        for(i=chat->count;i<100;i++){
          printf("%s \n",chat->messages[i].text);
        }
    }

    for(i=0;i<chat->count;i++){
      printf("%s \n",chat->messages[i].text);
    }
}

//date
int Addchat(char * name,char * text){
    if(++chat->count>100){
        chat->count = 0;
        chat->full =1;
    }
    chat->messages[chat->count-1].time = time(NULL);
    strcpy(chat->messages[chat->count-1].name,name);
   strcpy(chat->messages[chat->count-1].text,text);
   return 1;
}



void ReceiveMessage(){
    if(rcvmsg.pid==0){
        printf("%s \n",action[rcvmsg.action]);
        if(rcvmsg.action==1  || rcvmsg.action==4){
            printf("%s\n",rcvmsg.text);
        }
    }
    else printf("%s \n",status[rcvmsg.pid]);

    if(rcvmsg.pid==9){
        Addchat("Brak",rcvmsg.text);
        printf("%s\n",rcvmsg.text);
    }
}


//semafory
int shmid;

static struct sembuf sbuf;

void opusc(int semid, int semnum){
sbuf.sem_num = semnum;
sbuf.sem_op = -1;
sbuf.sem_flg = 0;
if (semop(semid, &sbuf, 1) == -1){
perror("Opuszczenie semafora");
exit(1);
}
}

void podnies(int semid, int semnum){
sbuf.sem_num = semnum;
sbuf.sem_op = 1;
sbuf.sem_flg = 0;
if (semop(semid, &sbuf, 1) == -1){
perror("Podnoszenie semafora");
exit(1);
}
}




int main(void)
{


//menu


strcpy(menu.name,"Zalogowany");
strcpy(menu.option[0],"Pokaż uzytkowników");
menu.function[0] = showusers;
strcpy(menu.option[1],"Wyślij prywatną wiadomość");
menu.function[1] = spm;
strcpy(menu.option[2],"Wyślij grupową wiadomość");
menu.function[2] = sgm;
strcpy(menu.option[3],"Pokaż grupy");
menu.function[3] = showgroups;
strcpy(menu.option[4],"Zmień nick");
menu.function[4] = changenick;
strcpy(menu.option[5],"Dołącz do grupy");
menu.function[5] = joingroup;
strcpy(menu.option[6],"Odejdź z grupy");
menu.function[6] = disjoingroup;
strcpy(menu.option[7],"Pokaż czat");
menu.function[7] = showchat;
strcpy(menu.option[8],"Wyloguj");
menu.function[8] = logout;
menu.count_option = 9;

//status

strcpy(status[0],"Polecenie wykonane poprawnie");
strcpy(status[1],"Jest już taki nick.");
strcpy(status[2],"Pełna tablica.");
strcpy(status[3],"Jest już taki pid.");
strcpy(status[4],"Nie ma takiego użykownika");
strcpy(status[5],"Nie ma grupy o takiej nazwie.");
strcpy(status[6],"Już należysz do tej grupy.");
strcpy(status[7],"Nie należysz do tej grupy.");
strcpy(status[8],"Jesteś niezalogowany.");
strcpy(status[9],"Otrzymałeś wiadomość.");


//action dla statusu=0

        strcpy(action[0],"Zalogowałes sie.");
        strcpy(action[1],"Lista użytkownikow:");
        strcpy(action[2],"Wiadomość została wysłana.");
        strcpy(action[3],"Wiadomość została wysłana.");
        strcpy(action[4],"Lista grup:");
        strcpy(action[5],"Zmieniłeś nick.");
        strcpy(action[6],"Dałączyłeś do grupy.");
        strcpy(action[7],"Opuściłeś grupe.");
        strcpy(action[8],"Wyświetlanie czasu");
        strcpy(action[9],"Wylogowałeś się.");


//login

sndmsg.pid = getpid();

int sid = msgget(12345, 0600 | IPC_CREAT);




while(1){

    printf("Nick: ");
    scanf("%s",sndmsg.nick);
    sndmsg.type = 1;
    sndmsg.action = 0;
    msgsnd(sid,&sndmsg,sizeof(Msg),0);
    msgrcv(sid,&rcvmsg,sizeof(Msg),sndmsg.pid,0);
    ReceiveMessage();

    if(rcvmsg.pid==0)break;
}



if(fork()==0){

    int shmid = shmget(35371,sizeof(Chat),IPC_CREAT|0600);
    chat = (Chat*) shmat(shmid,NULL,0);

    int semid = semget(46281,1,IPC_CREAT | 0644 | IPC_EXCL);
    if(semid==-1)semid=semget(12345,0,0);
    else semctl(semid,0,SETVAL,1);

    while(run){
        msgrcv(sid,&rcvmsg,sizeof(Msg),sndmsg.pid,0);
        ReceiveMessage();
    }

    semctl(semid,0,IPC_RMID,NULL);

    shmdt(chat);
    shmctl(shmid,IPC_RMID,NULL);
}
else{

    int shmid = shmget(35371,sizeof(Chat),IPC_CREAT|0600);
    if(shmid ==-1){printf("Brak alokacji pamięci współdzielonej. \n");abort();}
    chat = (Chat*) shmat(shmid,NULL,0);
    chat->count =0;
    chat->full =0;

    int semid = semget(46281,1,IPC_CREAT | 0644 | IPC_EXCL);
    if(semid==-1)semid=semget(12345,0,0);
    else semctl(semid,0,SETVAL,1);


    while(run){
        int w = showmenu();
        w--;
        if(w<0 ||w >=menu.count_option)continue;
        menu.function[w]();
        if(w!=7)msgsnd(sid,&sndmsg,sizeof(Msg),0);
    }

    semctl(semid,0,IPC_RMID,NULL);

    shmdt(chat);
    shmctl(shmid,IPC_RMID,NULL);
}

    printf("Koniec \n");
    return 0;
}

