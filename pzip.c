#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//~cs537-1/tests/bin/test-pzip.csh

void* zip(void* input);

int num_files;
int count = 0;
int*** store_count;
char** store_char[1024];//[num_files][total_file_sz][1024];
pthread_mutex_t store_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pos_lock = PTHREAD_MUTEX_INITIALIZER;

int global_pos = 0;
int file_num = 0;
int* global_call_num;
pthread_t* thread_num;

int file_size;

// void constructor (argc, ) {
//
//
//
// }

int main(int argc, char* argv[] ) {

  if (argc <= 1) {
    printf ("my-zip: file1 [file2 ...]\n");
    exit(1);
  }
  num_files = argc-1;
  // for (int i=1; i<argc; i++) {
   struct stat buf;
  // stat(argv[i], &buf); //TODO
  // file_size[i-1] = (int)buf.st_size;
  // total_file_sz+=file_size[i-1];
  // }

  for (int i=1; i<argc; i++) {

    int fd = open(argv[i], O_RDONLY);

    FILE *f_pointer = fopen(argv[i], "rb");

    if (fd == -1) {
      printf("my-zip: cannot open file\n");
      exit(1);
    }
    file_num = i-1;

    if(fstat(fd, &buf) < 0) {
      printf("fstat error\n");
      close(fd);
      exit(1);
    }
    file_size = (int)buf.st_size;
    char* pmap = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    for (int i=0, j=0; i<get_nprocs() && j<file_size; i++, j+=1024) {
      if(file_size - j < 0) break;
      if(file_size - j < 1024) pmap = pmap + file_size-j;
      else pmap = pmap+j;

      if ((pmap == NULL) || (*pmap == EOF)) break;
      pthread_create(&thread_num[i], NULL, zip, pmap); //TODO j = pointer to mmap

    }
    for (int i=0; i<get_nprocs(); i++) {
      pthread_join(thread_num[i], NULL);
    }
    //munmap(void *addr, size_t length);
    fclose(f_pointer);
  }

  for (int k=0; k<num_files; k++) {
    for (int l=0; l<global_call_num[k]; l++) {
      for (int m=0; m<1024; m++) {
        fwrite(&(store_count[k][l][m]), sizeof(int), 1, stdout);
        fprintf(stdout, "%c",store_char[k][l][m]);
    }
  }
}
  // //Outputs last data which could not be written due to while loop terminaton.
  // fwrite(&count, sizeof(int), 1, stdout);
  // fprintf(stdout, "%c", checker);
  // return(0);
}

 void *zip(void* pmap) {
   char* input = (char*) pmap;
   int position;
   int i = 0;
   int loc_call_num; ///*loc_file_num,*/;

   while(global_pos <= file_size) {

     pthread_mutex_lock(&pos_lock);
     global_pos += i;
     position = global_pos;
     loc_call_num = ++(global_call_num[file_num]);
     pthread_mutex_unlock(&pos_lock);

     int count = 0;
     int store_num = 0;
     char checker = *input;

     for(i=0; i<1024; i++) {
       if (*(input+i+position) == EOF) break;
       if (NULL == input+i+position) break;
       if(*(input+i+position) == checker) count++;
       else{
         if(count > 0) {
           pthread_mutex_lock(&store_lock);
           store_count[file_num][loc_call_num][store_num] = count; //TODO file_num
           store_char[file_num][loc_call_num][store_num] = checker; //TODO
           store_num++;
           pthread_mutex_unlock(&store_lock);
         }
         checker = *(input+i);
         count = 1;
       }
     }
     store_count[file_num][loc_call_num][store_num] = '\0'; //TODO file_num
     store_char[file_num][loc_call_num][store_num] = '\0'; //TODO
   }
     return 0; // input;
   }
