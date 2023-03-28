#include<math.h>
#include<vector>
#include<bits/stdc++.h>
#include<random>
using namespace std;

int LRU(int, int[]);
int FIFO(int, int[]);
int Clock(int, int[]);
bool ClockSearch(int[], int, int, bool[]);
int ClockUpdate(int[], int, int, int, bool[]);

int main(int argc, char *argv[])
{
 
  int data[1000];                             //array to hold page numbers
  default_random_engine ra;                   //generator for random number
  normal_distribution<double> dis(10.0,2.0);  //creation of distribution
                                              //with respective sd and mean
  int LRUResults[200];                        //hold the LRU results
  int FIFOResults[200];                       //hold the fifo results
  int ClockResults[200];                      //hold the clock results

  //this loop will simulate the 1000 experiments 
  for(int i = 0; i < 1000; i++)
    {
      //this loop will simulate the traces by creating the page numbers in the      // array 
      for(int j = 0; j < 1000; j++)
	{
	  data[j] = (10 * ((int) (j / 100))) + dis(ra);  //a random number wil                                                         //be generated with n                                                         //ormal distribution 
	}
      //this for loop will store the results from the algorithms
	    for(int wss = 4; wss <= 20; wss++)
	      {
		LRUResults[wss] += LRU(wss, data);    //store results ofLRU
		FIFOResults[wss] += FIFO(wss, data);  //store results of FIFO
		ClockResults[wss] += Clock(wss, data);//store results of Clock
	      }
	   
    }

  printf("LRU  FIFO  CLOCK\n\n");                     //print header for resul                                                      //ts
    //for loop that will print the results of the algorithms
    //the results will be averages of the 1000 experiments of the 1000 pages
    //per working set size 
    for(int wss = 4; wss <= 20; wss++)
	{
	 
	  printf("%d    %d    %d\n ", LRUResults[wss] / 1000, FIFOResults[wss] / 1000, ClockResults[wss] / 1000);                   //results from each work                                                      //ing set size will be 
	                                              //printed in rows
	}           

int LRU(int wss, int d[])
{

  vector<int> ve;                                        //used vectors to sto                                                         //re the page numbers                                                         //in memory
  int total = 0;                                         //total of page fault

  //this for loop will go through all the page numbers to find faults
  for(int i = 0; i <= 999 ; i++)
    {
      auto iterator =  find(ve.begin(), ve.end(), d[i]); //we use an iterator                                                          //to find elements in                                                         //memory

      if(iterator == ve.end())                   //if the element is                                                           //equals to the end o                                                         //f the vector 
	{
	  if(ve.size() == wss)                   //and if the size of                                                          //our vector is the s                                                         //ame as our wss
	    {
	      ve.erase(ve.begin());              //removes the first e                                                         //lement of the vecto
	    }

	  ve.push_back(d[i]);                    //add our page number                                                         //to the vector
	  total++;                               //add a page fault

	}
      else
	{
	  ve.erase(iterator);                     //if the iterator is                                                          //not the end of the                                                          //vector. Then we del                                                         //ete it 
	  ve.push_back(d[i]);                     //delete the page number
	}
    
    }

  return total;                                   //return total of page fault
}

int FIFO(int si, int d[])
{
  //to accomplish FIFO we will use a queue and an unordered set
  queue<int> qu;                          //queue is initialized 
  unordered_set<int> us;                  //set is initialized 
  int total = 0;                          //total of page faults
  
  for(int i = 0; i <= 999; i++)           //for loop to go through all the                                              //page numbers
    {

      if(us.size() < si)                  //check set capacity 
	{ 
 
       	  if(us.find(d[i]) == us.end())   //and if page number is in the se                                             //t is at the end of the set
	    {
	      us.insert(d[i]);            //insert the page number to the s                                             //et
	      qu.push(d[i]);              //push the page number in the queue
	      total++;                    //increment the total of page faults
	    }
	}
      else
      {

	if(us.find(d[i]) == us.end())     //make sure that d[i] is not in the                                           //set
	  {
	    int temp = qu.front();        //temp variable to hold the front of                                          //the queue
	    qu.pop();                     //pop the front page number
	    us.erase(temp);               //erase the temp from the set 
	    us.insert(d[i]);              //insert the page number in the set
	    qu.push(d[i]);                //push the page number in the queue
	    total++;                      //increment page fault
	  }
      }

    }
  return total;                           //return total of page faults
}

int Clock(int s, int d[])
{
  bool val;                                    //bool val to see if page numbe                                               //r was found 
  int p = 0;                                   //pointer for the algorithm
  int total = 0;                               //total of page faults
  bool temp[s];                                //array for second chances
  int a[s];                                    //second array to hold page num                                               //bers

  
  //for loop to go through all the page numbers
  for(int i = 0; i <= 999; i++) 
    {
      val = ClockSearch(a, d[i], s, temp);     //Call Clocksearch to look for                                                //the page number in the clock                                                //position

      if(val == false)                         //if not found
	{
	  p = ClockUpdate(a, p, d[i], s, temp);//update the pointer
	  total++;                             //increment page faults
	}
      
    }
  return total;                                //return total of page faults
}

bool ClockSearch(int ar[], int page, int s, bool tep[])
  {
    for(int i = 0; i < s; i++)   //for loop to find a page and set offset to t                                 //true 
      {
	if(ar[i] == page)        //if page is found
	  {
	    tep[i] == true;      //set second chance to true
	    return true;         //return true if it was found
	  }
      }

    return false;                //return false if it was not found 
  }
    
int ClockUpdate(int ar[], int po, int page, int s, bool tep[])
{
  int val = 0;                       //variable to help the replacement of pag                                     //e number and pointer

  while(true)                        //execute this loop until it returns the                                      // replacement 
    {
      if(tep[po] == false)           //if page is found, it is set to be repla                                     //ced
	{
	  ar[po] = page;             //replace the current page to the array
	  val = (po + 1) % s;        //update pointer 
	  return val;                //return pointer
	}

      tep[po] = false;               //set the element to false to be replaced                                     //next time it gets accessed
      po = (po + 1) % s;             //update pointer 
    }
}
