#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
  if(argc != 3){
    printf("Uso: restore copia destino\n");
    exit(1);
  }
  if(strcmp(argv[1], argv[2]) == 0){
    printf("Error: origen y destino son el mismo archivo\n");
    exit(1);
  }
  int fd = open(argv[1], O_RDONLY);
  if(fd < 0){
    printf("Error: no existe la copia %s\n", argv[1]);
    exit(1);
  }
  int out = open(argv[2], O_CREATE | O_WRONLY | O_TRUNC);
  if(out < 0){
    printf("Error: no se pudo crear %s\n", argv[2]);
    close(fd);
    exit(1);
  }
  char buf[512];
  int n;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    if(write(out, buf, n) != n){
      printf("Error: fallo al escribir en %s\n", argv[2]);
      close(fd);
      close(out);
      exit(1);
    }
  }
  close(fd);
  close(out);
  printf("Restauracion lista: %s -> %s\n", argv[1], argv[2]);
  exit(0);
}
