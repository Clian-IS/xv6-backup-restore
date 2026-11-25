#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAX_BUF 512

int calculate_checksum(char *buffer, int size) {
  int sum = 0;
  for(int i = 0; i < size; i++) {
    sum += buffer[i];
  }
  return sum;
}

int
main(int argc, char *argv[])
{
  int fd_src, fd_dest, n;
  char buf[MAX_BUF];
  int checksum = 0;
  struct stat st; // Estructura para guardar info del archivo

  if(argc < 3){
    fprintf(2, "Uso: backup <archivo_origen> <archivo_destino>\n");
    exit(1);
  }

  // Abrir archivo origen (solo lectura)
  if((fd_src = open(argv[1], O_RDONLY)) < 0){
    fprintf(2, "Error: no se puede abrir el origen %s\n", argv[1]);
    exit(1);
  }

  // VERIFICAR SI EL DESTINO YA EXISTE usando stat
  if(stat(argv[2], &st) == 0){
    fprintf(2, "Error: el archivo destino %s ya existe. No se sobrescribira.\n", argv[2]);
    close(fd_src);
    exit(1);
  }

  // Crear archivo destino (solo si no existía)
  if((fd_dest = open(argv[2], O_CREATE | O_WRONLY)) < 0){
    fprintf(2, "Error: no se puede crear el destino %s\n", argv[2]);
    close(fd_src);
    exit(1);
  }

  printf("Iniciando respaldo de %s a %s...\n", argv[1], argv[2]);

  while((n = read(fd_src, buf, MAX_BUF)) > 0){
    checksum += calculate_checksum(buf, n);
    if(write(fd_dest, buf, n) != n){
      fprintf(2, "Error: fallo al escribir en el destino\n");
      close(fd_src);
      close(fd_dest);
      exit(1);
    }
  }

  if(n < 0){
    fprintf(2, "Error: fallo al leer del origen\n");
  } else {
    printf("Respaldo completado con exito.\n");
    printf("Checksum de integridad: %d\n", checksum);
  }

  close(fd_src);
  close(fd_dest);
  exit(0);
}
