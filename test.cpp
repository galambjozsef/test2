#include <stdio.h>
#include <fcntl.h>
#include <iostream>

#include "myclass.h"
#include "thread.h"
#include "proxy.h"

#include "workerthread.h"
#include "commander.h"

#include <map>
#include <vector>
#include "socketserver.h"
#include "clientconnection.h"
#include "proxy.h"
#include "SignalException.h"


int main(int argc, char *argv[]) 
{
    printf("hello, world\n");
    printf("hi from me too!!!!!!!\n");



//    int kmem_fd = open("/dev/kmemory_device0", O_RDWR);
//    if(kmem_fd < 0)
//    {

//        printf("error opening\n");
//        return -1;
//    }

////    char test_w[4] = {'a','l','e','x'};
//    char test_w[] ="alex";

//    ssize_t res = pwrite(kmem_fd, test_w, 4, 0);

//    if (res < 0)
//    {
//        printf("error writing !\n");
//        return -1;
//    }

//    printf("%d bytes have been written !\n", res);
//    char test_r[4];


//    res =  pread(kmem_fd, (char*)test_r, 4, 0);

//    if (res < 0)
//    {
//        printf("error reading !\n");
//        return -1;
//    }
//    printf("%d bytes have been read !\n", res);


//    std::string readString(test_r);
//    std::cout<<"Read String = "<<readString<<std::endl;

//    printf("buffer read: %s !\n", test_r);

//    myClass* mC = new myClass("antenio","surfski");
//    mC->printMyName();


//    myClass mC2("pippo","nuts");
//    mC2.printMyName();


    ExceptionHandler g_objExceptionHandler;

//    //AA: Web server !!!!!!!!!!!!!!!!!!!!
//    Proxy theProxy;
//    theProxy.Init();
//    theProxy.Run(0);
//    /////////////////////////////////////

    std::string receivedCmd;



    Commander cmmndr;

    bool enableGetCMD = true;

    do
    {
        if(enableGetCMD) //AA: se si lancia con "test &" e si digita "bg" non si ha + il possesso della cmdline
	{		 //    questo si era reso necessario per effettuare prove di carico (con DoCpuLoad) e poter invocare "top"
			 //    e anche lanciare la prograzzazione del psoc4 con cpu "sotto stress ..."
            std::cout << "Type COMMAND !!!! ";
            getline (std::cin, receivedCmd);
            std::cout << "GOT --> "<<receivedCmd<<std::endl;

            if(!(receivedCmd.find("back") == std::string::npos))
            {
              enableGetCMD = false;
              std::cout << "GOING TO BACKGROUND !!!! "<<std::endl;
            }
            else
              cmmndr.executeCmd(receivedCmd);

            }


    } while(receivedCmd != "q");





    std::cout<<"QUITTING !!!!!!";

}
