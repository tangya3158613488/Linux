#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<iostream>

using namespace std;
void *rout(void *arg)
{

    string name=(char*)arg;
    while(1)

    { 
        cout<<"I am thread 1"<<endl;
        sleep(2);
    }
}
int main()
{
    

    pthread_t tid;
    pthread_create(&tid,NULL,rout,(void*)"thread 1");
  //  while(1)
  //  {
  //  
  //      cout<<"I am main thread"<<endl;
  //      sleep(3);
  //  }
    return 0;
}

