#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>

#include "api.h"

node* racine_globale;

node* create_new_node(){
  node* new = malloc(sizeof(node));
  new->champ = NULL;
  new->character = NULL;
  new->fils = NULL;
  new->frere = NULL;
  new->taille = 0;
  return new;
}

void modif_node(node* nod,char* champ,char* character,int taille,node* frere,node* fils){
  nod->champ = champ;
  nod->character = character;
  nod->taille = taille;
  nod->frere = frere;
  nod->fils = fils;
}

void parseur (char* req,int taille){
  int i = 0;
  node* fils = create_new_node();
  int size = 0;
  int flag = 1;
  node* racine = create_new_node();
  int pos = 0;

  modif_node (racine,"root",req,taille,NULL,fils);
  racine_globale = racine;

  while(req[i] != '\r' && req[i+1] != '\n' && i+1 <= taille){/*espace = 32*/
    size ++;
    i++;
  }
  flag = start_line(req,0,size,fils);

  while (i + 2 < taille && flag == 1){
    pos += size + 2;
    size = 0;
    i = pos;
    if(i+1 < taille){
      while(req[i] != '\r' && req[i] != '\n' && i+1 < taille){/*espace = 32*/
        size ++;
        i++;
      }
    }
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = header_field(req,pos,size,fils);
  }
}

int start_line (char* req, int pos, int taille, node* noeud){
  int flag = 1;

  printf("in start line\n");
  noeud->champ = "start_line";
  noeud->character = req + pos;
  noeud->taille = taille;
  noeud->fils = create_new_node();

  flag = request_line(req,pos,taille,noeud->fils);
  return flag;
}

int request_line(char* req, int pos, int taille, node* noeud){
  int i = pos;
  node* fils;
  int size = 0;
  int flag = 1;

  printf("in request line\n");

  noeud->champ = "request_line";
  noeud->character = req + pos;
  noeud->taille = taille;
  noeud->fils = create_new_node();
  fils = noeud->fils;

  while(req[i] != ' '){
    size ++;
    i++;
  }
  flag = method(req,pos,size,fils);
  if (flag == 1){
    pos += size;
    size = 0;
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _sp(req,pos,1,fils);
  }
  if (flag == 1){
    pos ++;
    i = pos;
    while(req[i] != ' '){
      size ++;
      i++;
    }
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = request_target(req,pos,size,fils);
  }
  if (flag == 1){
    pos += size;
    size = 0;
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _sp(req,pos,1,fils);
  }
  if (flag == 1){
    pos ++;
    while(req[i] != '\r' && req[i+1] != '\n'){
      size ++;
      i++;
    }
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = HTTP_version(req,pos,size,fils);
  }
  if (flag == 1){
    pos += size;
    size = 0;
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = __CRLF(req,pos-1,2,fils);
  }
  return flag;
}

int method(char* req,int pos,int taille,node* noeud){
  int flag = 1;

  printf("in method\n");
  noeud->champ = "method";
  noeud->character = req + pos;
  noeud->taille = taille;
  noeud->fils = create_new_node();

  flag = token(req,pos,taille,noeud->fils);
  return flag;
}

int _sp(char* req,int pos,int taille,node* noeud){
  int flag = 1;

  printf("in _sp\n");
  noeud->champ = "_sp";
  noeud->character = req + pos;
  noeud->taille = taille;

  if(*(noeud->character) != 32){
    flag = 0;
  }
  return flag;
}

int request_target(char* req,int pos,int taille,node* noeud){
  int flag = 1;

  printf("in request target\n");
  noeud->champ = "request_target";
  noeud->character = req + pos;
  noeud->taille = taille;

  if(*noeud->character != '/'){
    flag = 0;
  }
  return flag;
}

int HTTP_version(char* req,int pos,int taille,node* noeud){
  int i = pos;
  int debut = pos;
  node* fils;
  int size = 0;
  int flag = 1;

  printf("in http version\n");
  noeud->champ = "HTTP_version";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;

  while(req[i] != '/'){
    size ++;
    i++;
  }
  flag = HTTP_name(req,pos,size,fils);


  if (flag == 1){
    pos += size;
    size = 0;
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = case_insensitive_string(req,pos,1,fils);
  }

  if (flag == 1){
    pos ++;
    i = pos;
    while(req[i] != '.'){
      size ++;
      i++;
    }
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _digit(req,pos,size,fils);
  }

  if (flag == 1){
    pos += size;
    size = 0;
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = case_insensitive_string(req,pos,1,fils);
  }

  if (flag == 1){
    pos ++;
    i = pos;
    while((i - debut) != taille){
      size ++;
      i++;
    }
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _digit(req,pos,size,fils);
  }

  return flag;
}

int __CRLF(char* req,int pos,int taille,node* noeud){
  int flag = 1;

  printf("in crlf\n");

  noeud->champ = "__CRLF";
  noeud->character = req + pos;
  noeud->taille = taille;

  if(*(noeud->character) != '\r' || *(noeud->character + 1) != '\n'){
    flag = 0;
  }

  return flag;
}

int HTTP_name(char* req,int pos,int taille,node* noeud){
  int flag = 1;

  printf("in http_name\n");
  noeud->champ = "HTTP_name";
  noeud->character = req + pos;
  noeud->taille = taille;
  if(*noeud->character != 'H' || *(noeud->character + 1) != 'T' || *(noeud->character + 2) != 'T' || *(noeud->character + 3) != 'P'){
    flag = 0;
  }

  return flag;
}

int case_insensitive_string(char* req,int pos,int taille,node* noeud){
  int flag = 1;

  printf("in case_insensitive_string\n");
  noeud->champ = "case_insensitive_string";
  noeud->character = req + pos;
  noeud->taille = taille;

  if(*noeud->character != '.' && *noeud->character != '/' && *noeud->character != ':' && *noeud->character != '-' && *noeud->character != '_' && *noeud->character != '~' && *noeud->character != ','){
    flag = 0;
  }
  return flag;
}

int _digit(char* req,int pos,int taille,node* noeud){
  int flag = 1;

  printf("in _digit\n");
  noeud->champ = "_digit";
  noeud->character = req + pos;
  noeud->taille = taille;

  if(*noeud->character != '1' && *noeud->character != '2' && *noeud->character != '3' && *noeud->character != '4' && *noeud->character != '5' && *noeud->character != '6' && *noeud->character != '7' && *noeud->character != '8' && *noeud->character != '9' && *noeud->character != '0'){
    flag = 0;
  }
  return flag;
}

int token(char* req,int pos,int taille,node* noeud){
  int i = 0;
  node* fils;
  int flag = 1;

  printf("in token\n");
  noeud->champ = "token";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;
  flag = _alpha(req,pos,1,fils);
  pos ++;
  i++;

  while(*(req + pos) <= 122 && *(req + pos) >= 65 && flag == 1){
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _alpha(req,pos,1,fils);
    pos ++;
    i++;
  }

  if(i < taille){
    flag = 0;
  }
  return flag;
}

int _alpha(char* req,int pos,int taille,node* noeud){
  int flag = 1;

  noeud->champ = "_alpha";
  noeud->character = req + pos;
  noeud->taille = taille;
  if(*noeud->character > 122 || *noeud->character < 65){
    flag = 0;
  }
  return flag;
}

int header_field(char* req,int pos,int taille,node* noeud){
  node* fils;
  int flag = 1;

  printf("in header_field\n");
  noeud->champ = "header-field";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;
  if(req[pos] == 'T'){
    flag = Transfer_Encoding(req,pos,taille,fils);
  }

  if (req[pos] == 'H'){
    flag = Host_Header(req,pos,taille,fils);
  }
  return flag;
}

int Transfer_Encoding(char* req,int pos,int taille,node* noeud){
  node* fils;
  int flag = 1;
  int tmp = pos;

  printf("in Transfer-Encoding\n");
  noeud->champ = "Transfer-coding";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;

  if(!(*(req + pos) == 'T' && *(req + pos + 1) == 'r' && *(req + pos + 2) == 'a' && *(req + pos + 3) == 'n' && *(req + pos + 4) == 's' && *(req + pos + 5) == 'f' && *(req + pos + 6) == 'e' && *(req + pos + 7) == 'r' && *(req + pos + 8) == ' '
  && *(req + pos + 9) == 'E' && *(req + pos + 10) == 'n' && *(req + pos + 11) == 'c' && *(req + pos + 12) == 'o' && *(req + pos + 13) == 'd' && *(req + pos + 14) == 'i' && *(req + pos + 15) == 'n' && *(req + pos + 16) == 'g' && *(req + pos + 17) == ':' && *(req + pos + 18) == ' ')){
    flag = 0;
  }

  flag = _alpha(req,pos,1,fils);
  pos ++;
  while(*(req + pos) <= 122 && *(req + pos) >= 65 && flag == 1){
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _alpha(req,pos,1,fils);
    pos ++;
  }

  fils->frere = create_new_node();
  fils = fils->frere;
  flag = _sp(req,pos,1,fils);
  pos ++;

  while(*(req + pos) <= 122 && *(req + pos) >= 65 && flag == 1){
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _alpha(req,pos,1,fils);
    pos ++;
  }

  fils->frere = create_new_node();
  fils = fils->frere;
  flag = case_insensitive_string(req,pos,1,fils);
  pos++;
  fils->frere = create_new_node();
  fils = fils->frere;
  flag = _sp(req,pos,1,fils);
  pos ++;

  do{
    if (*(req+pos) == 'd'){
      flag = deflate(req,pos,7,fils);
      pos += 7;
    }

    else if(*(req+pos) == 'g'){
      flag = gzip(req,pos,4,fils);
      pos += 4;
    }

    else if(*(req+pos) == 'c' && *(req + pos + 1) == 'h'){
      flag = chunked(req,pos,7,fils);
      pos += 7;
    }

    else if(*(req+pos) == 'c' && *(req + pos + 1) == 'o'){
      flag = compress(req,pos,8,fils);
      pos += 8;
    }

    else if(*(req+pos) == ','){
      flag = case_insensitive_string(req,pos,1,fils);
      pos ++;
    }

    else{
      flag = 0;
    }

    fils->frere = create_new_node();
    fils = fils->frere;
  }while((pos - tmp) < taille -2 && flag == 1);

  flag = __CRLF(req,pos,2,fils);
  return flag;
}

int deflate(char* req,int pos,int taille,node* noeud){
  int flag = 1;
  node* fils;

  printf("in deflate\n");
  noeud->champ = "deflate";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;

  if(!(*(req + pos) == 'd' && *(req + pos + 1) == 'e' && *(req + pos + 2) == 'f' && *(req + pos + 3) == 'l' && *(req + pos + 4) == 'a' && *(req + pos + 5) == 't' && *(req + pos + 6) == 'e')){
    flag = 0;
  }

  flag = _alpha(req,pos,1,fils);
  pos++;

  while(*(req + pos) <= 122 && *(req + pos) >= 65 && flag == 1){
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _alpha(req,pos,1,fils);
    pos++;
  }
  return flag;
}

int gzip(char* req,int pos,int taille,node* noeud){
  int flag = 1;
  node* fils;

  noeud->champ = "gzip";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;

  if(!(*(req + pos) == 'g' && *(req + pos + 1) == 'z' && *(req + pos + 2) == 'i' && *(req + pos + 3) == 'p')){
    flag = 0;
  }

  flag = _alpha(req,pos,1,fils);
  pos++;

  while(*(req + pos) <= 122 && *(req + pos) >= 65 && flag == 1){
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _alpha(req,pos,1,fils);
    pos++;
  }

  return flag;
}

int chunked(char* req,int pos,int taille,node* noeud){
  int flag = 1;
  node* fils;

  printf("in chunked\n");
  noeud->champ = "chunked";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;

  if(!(*(req + pos) == 'c' && *(req + pos + 1) == 'h' && *(req + pos + 2) == 'u' && *(req + pos + 3) == 'n' && *(req + pos + 4) == 'k' && *(req + pos + 5) == 'e' && *(req + pos + 6) == 'd')){
    flag = 0;
  }

  flag = _alpha(req,pos,1,fils);
  pos++;

  while(*(req + pos) <= 122 && *(req + pos) >= 65 && flag == 1){
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _alpha(req,pos,1,fils);
    pos++;
  }

  return flag;
}

int compress(char* req,int pos,int taille,node* noeud){
  int flag = 1;
  node* fils;

  printf("in compress\n");
  noeud->champ = "compress";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;

  if(!(*(req + pos) == 'c' && *(req + pos + 1) == 'o' && *(req + pos + 2) == 'm' && *(req + pos + 3) == 'p' && *(req + pos + 4) == 'r' && *(req + pos + 5) == 'e' && *(req + pos + 6) == 's' && *(req + pos + 7) == 's')){
    flag = 0;
  }

  flag = _alpha(req,pos,1,fils);
  pos++;
  while(*(req + pos) <= 122 && *(req + pos) >= 65 && flag == 1){
    fils->frere = create_new_node();
    fils = fils->frere;
    flag = _alpha(req,pos,1,fils);
    pos++;
  }
  return flag;
}

int Host_Header(char* req,int pos,int taille,node* noeud){
  node* fils;
  int flag = 1;

  printf("in Host-Header\n");
  noeud->champ = "host-header";
  noeud->character = req + pos;
  noeud->taille = taille;

  noeud->fils = create_new_node();
  fils = noeud->fils;
  if(!(*(req + pos) == 'H' && *(req + pos + 1) == 'o' && *(req + pos + 2) == 's' && *(req + pos + 3) == 't' && *(req + pos + 4) == ' ' && *(req + pos + 5) == 'H' && *(req + pos + 6) == 'e' && *(req + pos + 7) == 'a' && *(req + pos + 8) == 'd'
  && *(req + pos + 9) == 'e' && *(req + pos + 10) == 'r' && *(req + pos + 11) == ':' && *(req + pos + 12) == ' ')){
    flag = 0;
  }
  pos += 13;

  while(*(req + pos) != '\r' && *(req + pos + 1) != '\n'&& flag == 1){
    if(*(req + pos) == '.' || *(req + pos) == '/' || *(req + pos) == ':' || *(req + pos) == '-' || *(req + pos) == '_' || *(req + pos) == '~'){
      fils->frere = create_new_node();
      fils = fils->frere;
      flag = case_insensitive_string(req,pos,1,fils);
    }

    else if(*(req + pos) <= 122 && *(req + pos) >= 65){
      fils->frere = create_new_node();
      fils = fils->frere;
      flag = _alpha(req,pos,1,fils);
    }

    else if(*(req + pos) == '\r' && *(req + pos + 1) == '\n'){
      fils->frere = create_new_node();
      fils = fils->frere;
      flag = __CRLF(req,pos,2,fils);
    }

    else{
      flag = 0;
    }
    pos ++;
  }
  return flag;
}

node *getRootTree(){
  return racine_globale;
}

char *getElementTag(node* noeud,int *len){
  *len = strlen(noeud->champ);
  return noeud->champ;
}

char *getElementValue(node* noeud,int *len){
  *len = noeud->taille;
  return noeud->character;
}

node *searchTree(node* branche,char *name,int* flag,int taille){
  node* tmp;

    if (strcmp(branche->champ,name) == 0){
      *flag = 1;
      tmp = branche;
    }
  if(*flag == 0 && branche->fils != NULL){
    tmp = searchTree(branche->fils,name,flag,branche->fils->taille);
  }

  if(*flag == 0 && branche->frere != NULL){
    tmp = searchTree(branche->frere,name,flag,branche->frere->taille);
  }

  if (*flag == 1){
    return tmp;
  }
  else{
    return NULL;
  }
}

void print_node(node* node){
  int i;
  printf("chaine de caractere de cette node :");
  for(i=0;i<node->taille;i++){
    printf("%c",node->character[i]);
  }
  printf("\n");
}
