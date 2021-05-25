#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
///////////////////////////








///////////////////////////////////////////////////////
// Pour pouvoir gérer la liste des joueurs qui jouent encore , on utilise une liste chainée
typedef struct element element;
struct element
{
    int val;
    struct element *nxt;
};

typedef element* llist;

//////////////
// permet d'afficher la liste
void afficherListe(llist liste)
{
    element *tmp = liste;
    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)
    {
        /* On affiche */
        printf("%d ", tmp->val);
        /* On avance d'une case */
        tmp = tmp->nxt;
    }
}

/// ajoute en tete
llist ajouterEnTete(llist liste, int valeur)
{
    /* On crée un nouvel élément */
    element* nouvelElement = malloc(sizeof(element));

    /* On assigne la valeur au nouvel élément */
    nouvelElement->val = valeur;

    /* On assigne l'adresse de l'élément suivant au nouvel élément */
    nouvelElement->nxt = liste;

    /* On retourne la nouvelle liste, i.e. le pointeur sur le premier élément */
    return nouvelElement;
}


// ajoute en fin
llist ajouterEnFin(llist liste, int valeur)
{
    /* On crée un nouvel élément */
    element* nouvelElement = malloc(sizeof(element));

    /* On assigne la valeur au nouvel élément */
    nouvelElement->val = valeur;

    /* On ajoute en fin, donc aucun élément ne va suivre */
    nouvelElement->nxt = NULL;

    if(liste == NULL)
    {
        /* Si la liste est videé il suffit de renvoyer l'élément créé */
        return nouvelElement;
    }
    else
    {
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
        element* temp=liste;
        while(temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = nouvelElement;
        return liste;
    }
}


// recherhcer un élément dans une liste // null est retourné si on ne trouve pas l'élement
llist rechercherElement(llist liste, int valeur)
{
    element *tmp=liste;
    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)
    {
        if(tmp->val == valeur)
        {
            /* Si l'élément a la valeur recherchée, on renvoie son adresse */
            return tmp;
        }
        tmp = tmp->nxt;
    }
    return NULL;
}


// supprimer un element de la liste avec sa valeur
llist supprimerElement(llist liste, int valeur)
{
    /* Liste vide, il n'y a plus rien à supprimer */
    if(liste == NULL)
        return NULL;

    /* Si l'élément en cours de traitement doit être supprimé */
    if(liste->val == valeur)
    {
        /* On le supprime en prenant soin de mémoriser
        l'adresse de l'élément suivant */
        element* tmp = liste->nxt;
        free(liste);
        /* L'élément ayant été supprimé, la liste commencera à l'élément suivant
        pointant sur une liste qui ne contient plus aucun élément ayant la valeur recherchée */
        tmp = supprimerElement(tmp, valeur);
        return tmp;
    }
    else
    {
        /* Si l'élement en cours de traitement ne doit pas être supprimé,
        alors la liste finale commencera par cet élément et suivra une liste ne contenant
        plus d'élément ayant la valeur recherchée */
        liste->nxt = supprimerElement(liste->nxt, valeur);
        return liste;
    }
}





// supprimer element en tete
llist supprimerElementEnTete(llist liste)
{
    if(liste != NULL)
    {
        /* Si la liste est non vide, on se prépare à renvoyer l'adresse de
        l'élément en 2ème position */
        element* aRenvoyer = liste->nxt;
        /* On libère le premier élément */
        free(liste);
        /* On retourne le nouveau début de la liste */
        return aRenvoyer;
    }
    else
    {
        return NULL;
    }
}



// supprimer element en fin
llist supprimerElementEnFin(llist liste)
{
    /* Si la liste est vide, on retourne NULL */
    if(liste == NULL)
        return NULL;

    /* Si la liste contient un seul élément */
    if(liste->nxt == NULL)
    {
        /* On le libère et on retourne NULL (la liste est maintenant vide) */
        free(liste);
        return NULL;
    }

    /* Si la liste contient au moins deux éléments */
    element* tmp = liste;
    element* ptmp = liste;
    /* Tant qu'on n'est pas au dernier élément */
    while(tmp->nxt != NULL)
    {
        /* ptmp stock l'adresse de tmp */
        ptmp = tmp;
        /* On déplace tmp (mais ptmp garde l'ancienne valeur de tmp */
        tmp = tmp->nxt;
    }
    /* A la sortie de la boucle, tmp pointe sur le dernier élément, et ptmp sur
    l'avant-dernier. On indique que l'avant-dernier devient la fin de la liste
    et on supprime le dernier élément */
    ptmp->nxt = NULL;
    free(tmp);
    return liste;
}







///////// cette fonction permet de retourner le numéro du joueur qui doit jouer
// l'argument m est supposé etre le joueur suivant // par exemple si le joueur 0 a joué , m sera égal 1 ,
// l'argument ma_list represente la liste des joueurs qui sont encore dans le jeu
// par exemple si m=1 est deja eliminé , donc il est plus dans la liste , cette fonction va permettre de revoyer le joueur qui peut jouer et qui est encore dans la liste
int who(int m,llist ma_liste){


  // si m est égal à 4 ca veut dire qu'on a fait les 4 joueurs , le tour est au joueur 0
  if (m==4){
    m=0;
    // on vérifie que le joueur 0 peut jouer , sil est dans la liste
    llist ma_liste2 =rechercherElement(ma_liste, m);
    // si le joueur n'est pas dans la liste ma_liste2 == null

    while(ma_liste2==NULL){
      // on prends le suivant jusqu'à on tombe sur un joueur qui peut jouer
      m++;
      ma_liste2 =rechercherElement(ma_liste, m);
    }
    return m;
  }


  // dans le cas ou 0<=m<4
  llist ma_liste2 =rechercherElement(ma_liste, m);
  while(ma_liste2==NULL){
    printf(" y a pas\n");
    m++;
    // si on retombe sur le cas m=4;
    if (m==4){
      m=0;
      llist ma_liste2 =rechercherElement(ma_liste, m);
      afficherListe(ma_liste);
      printf("\n" );
      while(ma_liste2==NULL){
        m++;
        ma_liste2 =rechercherElement(ma_liste, m);
      }
      return m;
    }
    ma_liste2 =rechercherElement(ma_liste, m);
  }
  int p=ma_liste2->val;
  return p;
}














int joueurSel;
  int objetSel;
int guiltSel=-1;
int object=-1;
int name=-1;

int valeur;


int joueurCourant;

struct _client
{
        char ipAddress[40];
        int port;
        char name[40];
} tcpClients[4];
int nbClients;
int fsmServer;
int deck[13]={0,1,2,3,4,5,6,7,8,9,10,11,12};
int tableCartes[4][8];
char *nomcartes[]=
{"Sebastian Moran", "irene Adler", "inspector Lestrade",
  "inspector Gregson", "inspector Baynes", "inspector Bradstreet",
  "inspector Hopkins", "Sherlock Holmes", "John Watson", "Mycroft Holmes",
  "Mrs. Hudson", "Mary Morstan", "James Moriarty"};

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void melangerDeck()
{
        int i;
        int index1,index2,tmp;

        for (i=0;i<1000;i++)
        {
                index1=rand()%13;
                index2=rand()%13;

                tmp=deck[index1];
                deck[index1]=deck[index2];
                deck[index2]=tmp;
        }
}

void createTable()
{
	// Le joueur 0 possede les cartes d'indice 0,1,2
	// Le joueur 1 possede les cartes d'indice 3,4,5
	// Le joueur 2 possede les cartes d'indice 6,7,8
	// Le joueur 3 possede les cartes d'indice 9,10,11
	// Le coupable est la carte d'indice 12
	int i,j,c;

	for (i=0;i<4;i++)
		for (j=0;j<8;j++)
			tableCartes[i][j]=0;

	for (i=0;i<4;i++)
	{
		for (j=0;j<3;j++)
		{
			c=deck[i*3+j];
			switch (c)
			{
				case 0: // Sebastian Moran
					tableCartes[i][7]++;
					tableCartes[i][2]++;
					break;
				case 1: // Irene Adler
					tableCartes[i][7]++;
					tableCartes[i][1]++;
					tableCartes[i][5]++;
					break;
				case 2: // Inspector Lestrade
					tableCartes[i][3]++;
					tableCartes[i][6]++;
					tableCartes[i][4]++;
					break;
				case 3: // Inspector Gregson
					tableCartes[i][3]++;
					tableCartes[i][2]++;
					tableCartes[i][4]++;
					break;
				case 4: // Inspector Baynes
					tableCartes[i][3]++;
					tableCartes[i][1]++;
					break;
				case 5: // Inspector Bradstreet
					tableCartes[i][3]++;
					tableCartes[i][2]++;
					break;
				case 6: // Inspector Hopkins
					tableCartes[i][3]++;
					tableCartes[i][0]++;
					tableCartes[i][6]++;
					break;
				case 7: // Sherlock Holmes
					tableCartes[i][0]++;
					tableCartes[i][1]++;
					tableCartes[i][2]++;
					break;
				case 8: // John Watson
					tableCartes[i][0]++;
					tableCartes[i][6]++;
					tableCartes[i][2]++;
					break;
				case 9: // Mycroft Holmes
					tableCartes[i][0]++;
					tableCartes[i][1]++;
					tableCartes[i][4]++;
					break;
				case 10: // Mrs. Hudson
					tableCartes[i][0]++;
					tableCartes[i][5]++;
					break;
				case 11: // Mary Morstan
					tableCartes[i][4]++;
					tableCartes[i][5]++;
					break;
				case 12: // James Moriarty
					tableCartes[i][7]++;
					tableCartes[i][1]++;
					break;
			}
		}
	}
}

void printDeck()
{
        int i,j;

        for (i=0;i<13;i++)
                printf("%d %s\n",deck[i],nomcartes[deck[i]]);

	for (i=0;i<4;i++)
	{
		for (j=0;j<8;j++)
			printf("%2.2d ",tableCartes[i][j]);
		puts("");
	}
}

void printClients()
{
        int i;
        for (i=0;i<nbClients;i++)
                printf("%d: %s %5.5d %s\n",i,tcpClients[i].ipAddress,
                        tcpClients[i].port,
                        tcpClients[i].name);
}

int findClientByName(char *name)
{
        int i;

        for (i=0;i<nbClients;i++)
                if (strcmp(tcpClients[i].name,name)==0)
                        return i;
        return -1;
}

void sendMessageToClient(char *clientip,int clientport,char *mess)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(clientip);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(clientport);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
                printf("ERROR connecting\n");
                exit(1);
        }

        sprintf(buffer,"%s\n",mess);
        n = write(sockfd,buffer,strlen(buffer));

    close(sockfd);
}

void broadcastMessage(char *mess)
{
        int i;

        for (i=0;i<nbClients;i++)
                sendMessageToClient(tcpClients[i].ipAddress,
                        tcpClients[i].port,
                        mess);
}









int main(int argc, char *argv[])
{








///
// on initalise la liste
  llist ma_liste = NULL;
  int y;

  for(y=3;y>=0;y--)
  {
      ma_liste = ajouterEnTete(ma_liste, y);

  }










     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	int i;

        char com;
        char clientIpAddress[256], clientName[256];
        int clientPort;
        int id;
        char reply[256];


     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
srand( time( NULL ) );
	printDeck();
	melangerDeck();
	createTable();
	printDeck();
	joueurCourant=0;

	for (i=0;i<4;i++)
	{
        	strcpy(tcpClients[i].ipAddress,"localhost");
        	tcpClients[i].port=-1;
        	strcpy(tcpClients[i].name,"-");
	}
















// le ok permet d'arreter le serveur une fois qu'un joueur a gagné
// il repasse a zero
int ok=1;
     while (ok)
     {
     	newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     	if (newsockfd < 0)
          	error("ERROR on accept");

     	bzero(buffer,256);
     	n = read(newsockfd,buffer,255);
     	if (n < 0)
		error("ERROR reading from socket");

        printf("Received packet from %s:%d\nData: [%s]\n\n",
                inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buffer);





////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////
        if (fsmServer==0)
        {
          // tant que tous les joeurs ne se sont pas connectés fmserver=0
        	switch (buffer[0])
        	{
                  //quand un joueur se connecte il envoie un msg commencant par c , suivi de ses informations
                	case 'C':
                        	sscanf(buffer,"%c %s %d %s", &com, clientIpAddress, &clientPort, clientName);
                        	printf("COM=%c ipAddress=%s port=%d name=%s\n",com, clientIpAddress, clientPort, clientName);


                                strcpy(tcpClients[nbClients].ipAddress,clientIpAddress);
                                tcpClients[nbClients].port=clientPort;
                                strcpy(tcpClients[nbClients].name,clientName);
                                nbClients++;

                                printClients();

				                      // on trouve l'id du joueur à partir de son nom

                                id=findClientByName(clientName);
                                printf("id=%d\n",id);

				                        // on lui envoie un message personnel pour lui communiquer son id

                                sprintf(reply,"I %d",id);
                                sendMessageToClient(tcpClients[id].ipAddress,
                                       tcpClients[id].port,
                                       reply);

				                    // Envoyer un message broadcast pour communiquer a tout le monde la liste des joueurs actuellement connectés


                                sprintf(reply,"L %s %s %s %s", tcpClients[0].name, tcpClients[1].name, tcpClients[2].name, tcpClients[3].name);
                                broadcastMessage(reply);

				// Si le nombre de joueurs atteint 4, alors on peut lancer le jeu

                                if (nbClients==4)
				{
					// On envoie ses cartes au joueur 0, ainsi que la ligne qui lui correspond dans tableCartes

          sprintf(reply,"D %d %d %d",deck[0],deck[1],deck[2]);
          sendMessageToClient(tcpClients[0].ipAddress,tcpClients[0].port,reply);
          for(i=0;i<8;i++)
					{
						sprintf(reply,"V %d %d %d",0,i,tableCartes[0][i]);
						sendMessageToClient(tcpClients[0].ipAddress,tcpClients[0].port,reply);
					}



					// On envoie ses cartes au joueur 1, ainsi que la ligne qui lui correspond dans tableCartes
          sprintf(reply,"D %d %d %d",deck[3],deck[4],deck[5]);
          sendMessageToClient(tcpClients[1].ipAddress,tcpClients[1].port,reply);
          for(i=0;i<8;i++)
					{
						sprintf(reply,"V %d %d %d",1,i,tableCartes[1][i]);
						sendMessageToClient(tcpClients[1].ipAddress,tcpClients[1].port,reply);
					}




					// On envoie ses cartes au joueur 2, ainsi que la ligne qui lui correspond dans tableCartes
          sprintf(reply,"D %d %d %d",deck[6],deck[7],deck[8]);
          sendMessageToClient(tcpClients[2].ipAddress,tcpClients[2].port,reply);
          for(i=0;i<8;i++)
					{
						sprintf(reply,"V %d %d %d",2,i,tableCartes[2][i]);
						sendMessageToClient(tcpClients[2].ipAddress,tcpClients[2].port,reply);
					}







					// On envoie ses cartes au joueur 3, ainsi que la ligne qui lui correspond dans tableCartes
          sprintf(reply,"D %d %d %d",deck[9],deck[10],deck[11]);
          sendMessageToClient(tcpClients[3].ipAddress,tcpClients[3].port,reply);
          for(i=0;i<8;i++)
					{
						sprintf(reply,"V %d %d %d",3,i,tableCartes[3][i]);
						sendMessageToClient(tcpClients[3].ipAddress,tcpClients[3].port,reply);
					}




					// On envoie enfin un message a tout le monde pour definir qui est le joueur qui commence // joueurcourant=0
          sprintf(reply,"M %d",joueurCourant);
					broadcastMessage(reply);
                                        // on passe à 1 le jeu peut commencer
                                        fsmServer=1;
				}
				break;
                }
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////



	else if (fsmServer==1)
	{

    // buffer[0] a chaque fois contient le type du msg que le client (joueur) a envoyé
 		switch (buffer[0])
		{
        // cas G , on est dans le cas ou le joueur soupçonne quelqu'un
        case 'G':
          sscanf(buffer,"G %d %d",&id,&guiltSel);

            // on regarde si la carte quil a choisit c'est la bonne que ca correspond à la 12 eme
            // si ce n'est pas la bonne , on supprime le joueur de la liste des joueurs qui peuvent jouer

            if(guiltSel!=deck[12]){
              printf("LA LOSE \n" );
              sprintf(reply,"P %d",id);
              // on envoie l'id du joueur qui a perdu à tous les clients avec p au debut du msg
              broadcastMessage(reply);

              if (id==0){
                llist liste2=supprimerElementEnTete(ma_liste);
                ma_liste=liste2;
              }
              else{
              supprimerElement(ma_liste, id);
              }
              afficherListe(ma_liste);
          }

          else{

            printf("quelqu'un a gagné !!!!\n");
            printf("j'ai fini mon travail\n");
            sprintf(reply,"W %d",id);
            // on envoie l'id du gagnant avec w au debut du msg
            broadcastMessage(reply);

            // cest pour desactiver le bouton go // on envoie 5 parceque personne ne possede cet id
            joueurCourant=5;
            sprintf(reply,"M %d",joueurCourant);
            broadcastMessage(reply);
              // ok passe a zéro pour arreter la boucle infini
            ok=0;
            fsmServer=0;
            break;

          }


        case 'O':
        // on est dans le cas ou un joueur sélectionne un object
        sscanf(buffer,"O %d %d",&id,&object);
        for(i=0;i<4;i++)
        {
          printf("valeur joueur %d :%d\n",i,tableCartes[i][object]);

          // il faut que ca soit different du joueur qui a fait la demande 
          if(i!=joueurCourant)
						{
              // soit le joueur ne possede pas il renvoie zero
          if(tableCartes[i][object]==0){
            valeur=0;
            sprintf(reply,"V %d %d %d",i,object,valeur);
            broadcastMessage(reply);
          }
          // dans le cas ou un joueur possede un certain nombre, on envoie 100 , qui va etre afficher comme une *
          else{
            valeur=100;
            sprintf(reply,"V %d %d %d",i,object,valeur);
            broadcastMessage(reply);

          }
        }
      }


        // pour determiner le  joueur qui doit jouer
        joueurCourant++;
        afficherListe(ma_liste);
        joueurCourant=who(joueurCourant,ma_liste);
        printf("j'ai envoyé:%d\n", joueurCourant);
        sprintf(reply,"M %d",joueurCourant);
        broadcastMessage(reply);
				break;





			case 'S':
          // dans le cas ou le joueur sélectionne un autre joueur et un objet
          // on voit savoir donc la valeur quil a

          sscanf(buffer,"S %d %d %d",&id,&name,&object);

          printf("valeur joueur %d :%d\n",id,tableCartes[name][object]);



          valeur=tableCartes[name][object];
          sprintf(reply,"V %d %d %d",name,object,valeur);
          broadcastMessage(reply);




          // pour determiner le  joueur qui doit jouer
      joueurCourant++;
      joueurCourant=who(joueurCourant,ma_liste);
      sprintf(reply,"M %d",joueurCourant);
      broadcastMessage(reply);
			break;




      default:
                        	break;
		}
  }
     	close(newsockfd);
     }
     close(sockfd);
     return 0;
}
