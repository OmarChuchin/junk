#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define EXACTSUM 45
#define NUM_THREADS 9
#define len 9
int boolean=0;
int sudoku[9][9];

//Makes the sum of the numbers on the rows and changes boolean if not 45 exactly.
int checkrow(int tid){
  int temp=0;
  for(int i=0;i<len;i++){
    temp+=sudoku[tid][i];
  }
  boolean+=(EXACTSUM-temp);
  return 0;
}

//Makes the sum of the numbers of the columns and changes boolean if not 45 exactly
int checkcolumns(int tid){
  int temp=0;
  for(int i=0;i<len;i++){
    temp+=sudoku[i][tid];
  }
  boolean+=EXACTSUM-temp;
  return 0;
}

/*
According to the id of the thread it chooses where to start and where to end the box
Makes the sum of the numbers on the box it chose and changes the boolean if not 45 exactly
*/
int checkboxes(int tid){
  int a,b,c,d;
  switch (tid) {
    case 0:
      a=0;
      b=2;
      c=0;
      d=2;
      break;
    case 1:
      a=0;
      b=2;
      c=3;
      d=5;
      break;
    case 2:
      a=0;
      b=2;
      c=6;
      d=8;
      break;
    case 3:
      a=3;
      b=5;
      c=0;
      d=2;
      break;
    case 4:
      a=3;
      b=5;
      c=3;
      d=5;
      break;
    case 5:
      a=3;
      b=5;
      c=6;
      d=8;
      break;
    case 6:
      a=6;
      b=8;
      c=0;
      d=2;
      break;
    case 7:
      a=6;
      b=8;
      c=3;
      d=5;
      break;
    case 8:
      a=6;
      b=8;
      c=6;
      d=8;
      break;
    default:
      break;
  }
  int temp=0;
  for(int x=a;x<=b;x++){
    for(int y=c;y<=d;y++){
      temp+=sudoku[x][y];
    }
  }
  boolean+=EXACTSUM-temp;
  return 0;
}

//Controller of all the methods the thread is supposed to do
void *threadProtocol(void *threadid){
  long tid;
  tid = (long)threadid;
  int select=0;
  while(boolean<1 && select<3){
    switch (select) {
      case 0:
        checkrow(tid);
        break;
      case 1:
        checkcolumns(tid);
        break;
      case 2:
        checkboxes(tid);
        break;
      default:
        break;
    }
    select++;
  }
}

int Loader(){
  FILE *ptr_file;
  char buf[1000];

  ptr_file =fopen("sudoku","r");
  if (!ptr_file)
      return 1;

  int i;
  int column;
  int row=0;
  char number[2];

  while (fgets(buf,1000, ptr_file)!=NULL){
      i=0;
      column=0;
      while(buf[i]!=NULL){
        if(buf[i]!=' '){
          number[0]=buf[i];
          sudoku[row][column]=atoi(number);
          column++;
        }
        i++;
      }
      row++;
  }
  fclose(ptr_file);
  return 0;
}

/*
int runner(){
  pthread_id_np_t tid;
  tid=pthread_getthreadid_np();
  printf("Hello from thread num>%d\n",tid);
  return 0;
}
*/

int main(){

  Loader();

  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      printf("%d ",sudoku[i][j]);
    }
    printf("\n");
  }


  pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t<NUM_THREADS; t++){
      rc = pthread_create(&threads[t], NULL, threadProtocol, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
    }
    for(int i=0;i<NUM_THREADS;i++){
        pthread_join(threads[i],NULL);
    }

    if(boolean==0){
      printf("The sudoku solution is correct");
    }
    else{
      printf("The sudoku solution has a mistake");
    }
    printf("\n");
    

  return 0;
}
