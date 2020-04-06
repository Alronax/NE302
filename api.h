/*structure node correspondant à un noeud de l'arbre
son champ est le tag qui lui correspond ; son character est un pointeur vers la chaine de caractere de depart "req" ou commence le header de ce noeud
la taille est le nombre de caracteres du header ; le frere est un pointeur vers une node du meme niveau et le fils vers une node de niveau en dessous*/
typedef struct node{
  char* champ;
  char* character;
  int taille;
  struct node* frere;
  struct node* fils;
}node;

/*create node cree une node et l'initialise*/
node* create_new_node();

/*modif_node modifie une node existante*/
void modif_node(node* nod,char* champ,char* character,int taille,node* frere,node* fils);

/*fonction utilisee pour creer les nodes de la start line dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int start_line (char* req, int pos, int taille, node* noeud);

/*fonction utilisee pour creer les nodes de la request line dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int request_line(char* req, int pos, int taille, node* noeud);

/*fonction utilisee pour creer les nodes de la method dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int method(char* req,int pos,int size,node* noeud);

/*fonction utilisee pour creer les nodes d'un _sp dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int _sp(char* req,int pos,int size,node* noeud);

/*fonction utilisee pour creer les nodes de la request target dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int request_target(char* req,int pos,int size,node* noeud);

/*fonction utilisee pour creer les nodes de l'HTTP_version dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int HTTP_version(char* req,int pos,int size,node* noeud);

/*fonction utilisee pour creer les nodes du CRLF dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int __CRLF(char* req,int pos,int size,node* noeud);

/*fonction utilisee pour creer les nodes du HTTP_name dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int HTTP_name(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes d'un case_insensitive_string dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int case_insensitive_string(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes d'un digit dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int _digit(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes d'un token dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int token(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes d'un alpha dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int _alpha(char* req,int pos,int taille,node* noeud);

/* L'appel à votre parser un char et une longueur à parser.*/
void parseur (char* req,int taille);

/*fonction utilisee pour creer les nodes du header field dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int header_field(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes du transfer encoding dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int Transfer_Encoding(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes du deflate dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int deflate(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes du gzip dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int gzip(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes du chunked line dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int chunked(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes du compress dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int compress(char* req,int pos,int taille,node* noeud);

/*fonction utilisee pour creer les nodes du host header dans l'arbre
elle renvoie 1 si la start line et tout ce qui en découle est bonne, 0 sinon*/
int Host_Header(char* req,int pos,int taille,node* noeud);

/* Fonction qui retourne un pointeur (type opaque) vers la racine de l'arbre construit.*/
node *getRootTree();

/* Fonction qui recherche dans l'arbre tous les noeuds dont l'etiquette est egale à la chaine de caractères en argument.
 Par convention si start == NULL alors on commence à la racine
 sinon on effectue une recherche dans le sous-arbre à partir du noeud start*/
node *searchTree(node* branche,char *name,int* flag, int taille);

/* fonction qui renvoie un pointeur vers char indiquant l'etiquette du noeud. (le nom de la rulename, intermediaire ou terminal)
 et indique (si len!=NULL) dans *len la longueur de cette chaine.*/
char *getElementTag(node* noeud,int *len);

/* fonction qui renvoie un pointeur vers char indiquant la valeur du noeud. (la partie correspondnant à la rulename dans la requete HTTP )
 et indique (si len!=NULL) dans *len la longueur de cette chaine.*/
char *getElementValue(node *noeud,int *len);

/*affiche le champ character d'une node*/
void print_node(node* node);
