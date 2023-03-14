#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
   pid_t pid1, pid2, pid3;
   int status;

   // fork the first child
   pid1 = fork();
   pid2 = fork();

   if (pid1 < 0) {
      perror("Error: failed to fork the first child");
      return -1;
   }

   // child 1
   if (pid1 == 0) {
// child 2
	if (pid2 ==0){
      // fork the grandchild
     // pid3 = fork();
     // if (pid3 < 0) {
     //    perror("Error: failed to fork the grandchild");
     //    return -1;
     // }

      // grandchild
     // if (pid3 == 0) {
         // change working directory to /home/username/lab4
         if (chdir("./laab4") < 0) {
            perror("Error: failed to change working directory");
            return -1;
         }

         // create a file sample.txt with permissions 0777
         int fd = open("sample.txt", O_CREAT | O_WRONLY, 0777);
         if (fd < 0) {
            perror("Error: failed to create file");
            return -1;
         }
         close(fd);
         printf("Grandchild: created file sample.txt\n");

         return 0;
      }

      // child 1 waits for grandchild to complete execution
     // waitpid(pid3, &status, 0);
      printf("Child 1: grandchild has completed execution\n");

      // use execv() to differentiate and print the contents of /home/username/lab4
      char *args[] = {"ls", "-1", "./laab4", NULL};
      execv("/bin/ls", args);

      perror("Error: execv() failed");
      return -1;
   }

   // fork the second child
//   pid2 = fork();
   if (pid2 < 0) {
      perror("Error: failed to fork the second child");
      return -1;
   }

   // child 2
   if (pid2 == 0) {
      // differentiate child 2 using execl()
      execl("/bin/ls", "ls", "-1", "/Users/jaskaransingh", (char *) NULL);
      perror("Error: execl() failed");
      return -1;
   }

   // main process waits for child 1 and child 2 to complete execution
   waitpid(pid1, &status, 0);
   waitpid(pid2, &status, 0);
   printf("Main process: both children have completed execution\n");

   return 0;
}
