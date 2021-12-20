# Solucionari

### Activitat 01: 
Descriu les diferències entre aquests dos codi i raona el motiu de les diferències observades. (Fixa't en el fork() i en l'obertura del descriptor open()).

En el primer cas:

```c

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

void main() {
int fd=0;
fork();
fd = open("out.txt", (O_CREAT|O_RDWR));
write(fd, "so\n", 3);
close(fd);
exit(0);
}

```
El valor de la variable fd inicial és el mateix en el procés pare i fill. Un cop cada procés executa la crida a sistema open aquest valor serà diferent en cada procés. Per tant, tindrem dos còpies que escriuran sobre el mateix fitxer i en la mateixa posició. El resultat serà el de l'últim procés executat que sobrescriura el valor del primer. 

```cat out.txt``` ens retornar la cadena "so\n".

En el segon cas:

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

void main() {
int fd;
fd = open("out.txt", (O_CREAT|O_RDWR));
fork();
write(fd, "so\n", 3);
close(fd);
exit(0);
}
```

El fitxer és obert pel procés pare. El procés pare i fill hereden el fitxer obert en el mateix descriptor de fitxer **fd**. El primer procés escriure sobre aquest descriptor (fitxer) i actualitzarà el valor de l'apuntador al final de la cadena escrita en el fitxer. El segon procés com comparteix descriptor escriurà a continuació i per tant observarem els comportament del mode **append**.

```cat out.txt``` ens retornar la cadena "so\nso\n".

### Activitat 02: 
Modifiqueu el codi anterior per tenir una comunicació bidireccional seguint els passos anteriors.

```c
#include <stdio.h>
#include <unistd.h>
#include <erno.h>
#include <sys/types.h>

 void main(){
     
     // Declarem el nostre array per emmagatzemar els descriptor associats a una pipe    
     int     fd[2];
     int     fd1[2]; //Sol
     pid_t   pidFill;
     /* El primer element de la matriu (fd[0]) està configurat i obert per a la lectura, 
        mentre que el segon element està configurat i obert per escriure (fd[1]. 
        Totes les dades que viatgen per la canonada es mouen pel nucli. */
     
     pipe(fd);
     pipe(fd1); //Sol

     pidFill = fork();
        
     if(pidFill == -1){
        perror("fork");
        exit(1);
     } else if (pidFill == 0){
         /*El procés fill tanca la escriptura per la pipe*/
         close(fd[1]);
         close(fd1[0]); //Sol
     } else {
         /* El procés pare tanca la lectura per la pipe */
         close(fd[0]);
         close(fd1[1]); //Sol
     }
 }
```

# Activitat 03: 
Raoneu i reflexioneu sobre les següents preguntes. 

* Es comparteix l’espai d'adreces entre dos processos Pare-Fill que comparteixen un pipe?
* On van les dades després de l’escriptura a una pipe, però abans de la lectura del pipe?

En sistemes operatius UNIX dos processos Pare i Fill mai comparteixen l'espai d'adreces. El pipes resideixen al nucli i per aquest motiu ens permeten comunicar dos processos diferents. Les dades que es transfereixen per la pipe s'allotgen a un buffer de la memòria caché que es troba en l'espai d'adreces del nucli.

![](https://cs61.seas.harvard.edu/site/img/pipe-fork.gif)

# Activitat 04:
Explica que fa el codi següent i raona l'efecte de comentar i descomentar la línia marcada amb //(X).

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    int r = pipe(fd);
    assert(r == 0);

    pid_t p1 = fork();
    assert(p1 >= 0);

    if (p1 == 0) {

        close(fd[0]);
        char msg[5];
        sprintf(msg,"msg1");
        ssize_t nw = write(fd[1], msg, strlen(msg));
        assert(nw == (ssize_t) strlen(msg));
        sprintf(msg,"msg2");
        nw = write(fd[1], msg, strlen(msg));
        assert(nw == (ssize_t) strlen(msg));
        //close(fd[1]); //(X)
        while(1);
        exit(0);
    }

    close(fd[1]);
    char buf[100];

    while(read(fd[0],&buf,sizeof(buf)) ){
        printf("I got a message! It was %s\n", buf);
    }
    printf("No more messages :(\n");
    close(fd[0]);

}
```

Aquest codi ens mostra la comunicació unidireccional entre un pare i un fill utilitzant una pipe. En aquest cas, el fill és l'encarregat d'escriure per la pipe dos misatge **msg1** i **msg2**. Per fer-ho, cada procés tanca el descriptor de la pipe que no necessita. En el cas del fill ```close(fd[0]);```i del pare ```close(fd[1]);```. 

El pare fa un bucle infinit que bloquejarà el procés mentre el descriptor de fitxer d'escriptura de la pipe estigui obert. D'aquesta manera si el procés fill no tanca el descriptor descomentant la línia ```//(X)```el procés pare no acabarà. De forma implicita si el procés fill fa ```l'exit(0)```també es tancarien els descriptors i el procés sortirià del bucle.

# Activitat 05:
Completeu aquest codi amb c per aconseguir replicar la comanda (ls | wc -l).

```c
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int fd[2];
    char s[100];
    char *p1[] = {"ls", NULL};
    char *p2[] = {"wc", "-l", NULL};
	
	if (pipe(fd)<0)
	{
		perror("Error de creació del pipe fd[]");
		exit(-1);
	}

    int pid;

    switch (pid = fork()){
        case -1:	
					perror("Error fork()");
					exit(-2);
					break;
					
		case 0:	
				if (close(fd[0]) < 0) perror("close fd[0]");
				if (dup2(fd[1], 1) < 0) perror("dup2 fd[1] -> 1");
				if (close(fd[1]) < 0) perror("close fd[1]");

                execvp(p1[0], p1);
				exit(-3);
	
		default:
                if (close(fd[1]) < 0) perror("close fd[1]");
				if (dup2(fd[0], 0) < 0) perror("dup2 fd[0] -> 0");
				if (close(fd[0]) < 0) perror("close fd[0]");
					
				execvp(p2[0], p2);
				exit(-3);
	    }
    
}	
```

# Activitat 06:
Modifiqueu el codi anterior per tenir dos processos fills, un farà el `ls` i l'altre el `wc -l`. El pare esperarà que els dos fills acabin per acabar.

```c
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int fd[2];
    char s[100];
    char *p1[] = {"ls", NULL};
    char *p2[] = {"wc", "-l", NULL};
	
	if (pipe(fd)<0)
	{
		perror("Error de creació del pipe fd[]");
		exit(-1);
	}

    int pid;

    switch (pid = fork()){
        case -1:	
					perror("Error fork()");
					exit(-2);
					break;
					
		case 0:	
				if (close(fd[1]) < 0) perror("close fd[1]");
				if (dup2(fd[0], 0) < 0) perror("dup2 fd[0] -> 0");
				if (close(fd[0]) < 0) perror("close fd[0]");
					
				execvp(p2[0], p2);
				exit(-3);
	
		default:
                if (close(fd[0]) < 0) perror("close fd[0]");
				if (dup2(fd[1], 1) < 0) perror("dup2 fd[1] -> 1");
				if (close(fd[1]) < 0) perror("close fd[1]");

                execvp(p1[0], p1);
				exit(-3);
	    }
    
}	
```

# Activitat 07: 

Milloreu el codi presentat, eliminar repeticions, simplificant la lògica, millorant el Makefile... Com a mínim heu de fer una aportació.

# Activitat 08:
Simulant combats Pokémon amb pipes.

Solucionada a la carpeta act08
