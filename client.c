#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "server.h"

int main(int argc, char **argv) /*"named pipe" client.c*/
{
	struct message msg;
	int n, fdpub, fdpriv;
	char line[LINESIZE];
	
	/* синтезировать имя личного программного канала и создать 
	этот канал с правами чтения и записи для всех процессов*/
	/*3. */
	sprintf(msg.privfifo, "Fifo%d", getpid());
	/*2. */
	if (mkfifo(msg.privfifo, S_IFIFO|0666) == -1){ 
		perror (msg.privfifo);
		exit(1);
	}
	
	/* общедоступный именованный канал (создается перед запуском сервера %mkfifo public) открывается на запись, и в него записываются имена личного канала и требуемого файла */
	if ((fdpub=open(PUBLIC, O_WRONLY)) == -1){ 
		perror (PUBLIC);
		exit (2);
	}
	strcpy (msg.filename, argv[1]);
	write (fdpub, (char*)&msg, sizeof(msg));
	
	/* личный именованный канал открывается для чтения */
	if ((fdpriv=open (msg.privfifo, O_RDONLY)) == -1){ 
		perror (msg.privfifo);
		exit (3);
	}
	
    while((n = read(fdpriv, line, 100)) > 0){
    char* echo_f = "echo ";
    char* sed_f1 = "| sed 's/^[ \t]*//'";
    char* sed_f2 = "| rev";
    char cToStr[2];
    cToStr[1] = '\0';
	/*распечатать данные, полученные из личного канала */
	/*5. */ 
	while((n = read(fdpriv, line, 10000)) > 0){
    char* buff = calloc(1000, 1);
    for (int i =0; i < strlen(line); i++){
    	if(line[i] == '\n'){
    		char* buff2 = calloc(1000, 1);
    		strcat(buff2, echo_f);
    		strcat(buff2, buff);
    		strcat(buff2, sed_f1);
    		strcat(buff2, sed_f2);
    		system(buff2);
    		free(buff);
    		free(buff2);
    		char* buff = calloc(1000, 1);
    	} else {
    		cToStr[0] = line[i];
    		strcat(buff, cToStr);
    	}
    }
 
	}
	}

	/*личный именованный канал закрывается и удаляется из текущего каталога */
	close (fdpriv);
	unlink (msg.privfifo);
	exit(0);
}
