/********************************************************************
 *File: pgm1.cpp
 *Author: Marco Delgado
 *Procedures:
  *main: procedure that creates two processes using fork and pipes to communicate in between those two processes
  *magics: it will take the nine-digit input and will verify whether it is a magic square 
  *sysinfo: uses different functions to print the information about the machine
  *******************************************************************/
#include<cstring>
#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/utsname.h>

int magics(int);
void sysinfo();

/********************************************************************
 *int main()
 *Author: Marco Delgado
 *Date: 17 September 2021
 *Description: Creates two processes using fork. Recieves input from the user, then pipes will be used to communicate between the two processes providing 
 a final statement of whether the input is a magic square or not.
********************************************************************/
int main()
{
  int nu;
  char number[9]; //Number input from the user
  pid_t pr;       //Variable for fork()
  int fd1[2];     //First pipe
  int fd2[2];     //Second pipe

  sysinfo();      //Call sysinfo to print the information about the machine

  if (pipe(fd1) == -1 || pipe(fd2) == -1)
    {
      fprintf(stderr, "Error on pipe"); //Make sure that no error on pipes
      return 1;
    }

  fprintf(stdout, "Enter a nine-digit number: \n\n");

  scanf("%s", number); //Read input from the user to the "number" variable

  pr = fork();         //Forking the program

  if(pr < 0)
    {
      fprintf(stderr, "Fork failed "); //Make sure fork did not fail
    }

  //Parent process (client)
  else if(pr >  0)
    {
      int output;     //The result from the server 
      close(fd1[0]);  //Close first pipe read end
      write(fd1[1], number, strlen(number)+1); //Write input to the first pipe
      close(fd1[1]);  //Close first pipe write-end
      wait(NULL);     //Wait for the child procress (server) for response
      close(fd2[1]);  //Close second pip write-end
      read(fd2[0], &output, sizeof(output)); //Read the result from the server
      if(output == 1)
	{
	  fprintf(stdout, "It is not a magic square!\n"); //True from server 
	}
      else
	{
	  fprintf(stdout, "It is a magic square!\n"); //False from server
	}
      return 0;
   
      
      
    }
  //Child process (server)
  else
    {
      char num[9];          //nine-digit number from the client
      int result = 0;       //result from the verification process
      close(fd1[1]);        //close first pipe write end
      read(fd1[0], num, 9); //read the number from the client to num
      close(fd1[0]);        //close first pipe read end
      close(fd2[0]);        //close second pipe read end
      nu = atoi(num);       //Convert the char var to integer 
      result = magics(nu);  //Call mnagics procedure and storing in result
      write(fd2[1], &result, sizeof(result)); //Send the result to the client
      close(fd2[1]);        //Close second pipe write end
      
      exit(0);
      
    }
  
  return 0;
}
/************************************************************
 *int magics(int x)
 *Author: Marco Delgado
 *Date: 17 September 2021
 *Description: it will recieve a nine-digit number. Then turn it into a 3x3 matrix and solve to see if it is a magic square or not.
 *Parameters:
       *x             Represents the nine-digit number from the user
       *******************************************************/
int magics(int x)
{
     int a[3][3];    //3x3 matrix for the square
     long temp[8];   //A temp array for numbers to hold separate values
     int s = 0;      //Counter
     int count = 1;  //Counter 

     for(int i = 8; i >= 0;  i--)
       {
	 temp[i] = x / count % 10;  //This for loop will traverse through x
	 count = count * 10;        //by getting every digit separate to temp
       } 
     for(int j = 0; j < 3; j++)
       {
	 for(int k = 0; k < 3; k++)
	   {
	     a[j][k] = temp[s];     //For loop to assign every element of the
	     s++;                   //temp array to the 3x3 matrix 
	   }
       }

     count = 0;                    //Set to 0 for future use 

     int sumrow = 0;               //hold the sum of a row
     int sumcol = 0;               //hold the sum of column
     int sum = 0;                  //keeps count of the sum
     int row[3];                   //hold the three sums for all the rows
     int col[3];                   //hold the three sums for all the columns

     //Diagonal sum
     for(int m = 0; m < 3; m++)
       {
	 count = count + a[m][m];  //This loop will get the sum of the first
	                           //of the first diagonal
       }
     for(int m = 0, n = 2; m < 3; m++, n--)
       {
	 sum = sum + a[m][n];      //This loop will get the sum of the second
                                   //diagonal of the matrix
       }                           
     if(sum != count)              //if the sums are not the same, then it is
       {                           //false (in this case, 1)
	 return 1;
       }
     for(int m = 0; m < 3; m++)
       {
	 sumrow = 0;
	 sumcol = 0;
	 for(int n = 0; n < 3; n++)     //This loop will calculate the sum of
	   {                            //the rows and columns from the matrix
	     sumrow = sumrow + a[m][n];
	     sumcol = sumcol + a[n][m];
	   }
	 row[m] = sumrow;               //Every time the sum of a row or a 
	 col[m] = sumcol;               //column is gotten, it is sent to the
	                                //respective array
	 
       }
     for(int i = 0; i < 3; i++)
       {
	 if(row[0] != sum || col[0] != sum) //This loop will make sure that 
	   {                                //all the sums are the same, even
	     return 1;                      //comparing with the diagonal sums
	   }                                //return 1 for false, return 2 for
       }                                    //true
     return 2;
 }
/********************************************************************
 *void sysinfo()
 *Author: Marco Delgado
 *Date: 17 September 2021
 *Description: This procedure just serves as calling other prodecures to print the machine specifications
 ********************************************************************/
void sysinfo()
{
  char name[100];                     //Hold the buffer from the procedures
  int size = 100;                     //size of the buffer
  struct utsname uData;               //struct var for the uname procedure
  getdomainname(name, size);          //get the name of the domain
  printf("%s\n", name);           
  gethostname(name, size);            //get the name of the host
  printf("%s\n", name); 
  uname(&uData);                      //call the uname prodecure to print
  printf("%s\n", uData.sysname);      //specific information about the machine
  printf("%s\n", uData.release);     
  printf("%s\n", uData.version);      //Several pieces of information are 
  printf("%s\n\n\n", uData.machine);  //being printed from uname
  system("lsb_release -a");           //system command is called to print the
                                      //version of ubuntu
} 
