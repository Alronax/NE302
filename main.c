#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

int main(int argc,char *argv[]){
  int flag = 0;
  int* drapeau = &flag;
  node* racine;
  node* branche;
  char requete[] = "GET /index.html HTTP/1.0\r\nTransfer Encoding: chunked,gzip,compress,deflate\r\nHost Header: www.esisar.grenoble-inp.fr\r\n";
  parseur (requete,strlen(requete));

  racine = getRootTree();
  branche = searchTree(racine,"_alpha",drapeau,strlen(requete));
  
  print_node(branche);
  return 0;
}
