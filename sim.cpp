#include <iostream>
#include <string>
#include <vector>
using namespace std;

class task
{
private:
    int task_number;    //-task_number: int
    int arrival_time;   //-arrival_time: int
    int waiting_time;   //-waiting_time: int
    int transaction_time;//-transaction_time: int
public:
    task(int = 0, int = 0, int = 0, int = 0); //+task(int = 0, int = 0, int = 0, int = 0)
    ~task();
    void setCustomerInfo(int = 0, int = 0, int = 0, int = 0);//+setCustomerInfo(int = 0, int = 0, int = 0, int = 0): void
    int getWaitingTime();                                    //+getWaitingTime() : int
    void setWaitingTime(int);                                //+setWaitingTime(int): void
    void incrementWaitingTime();                             //+incrementWaitingTime(): void
    int getArrivalTime();                                    //+getArrivalTime() const: int
    int getTransactionTime();                                //+getTransactionTime() const: int
    int getTaskNumber();                                     //+getTaskNumber() const: int
};

class server
{
private:
    task currentTask;   //-currentTask: task
    string status;      //-status: string
    int transactionTime;//-transactionTime: int
public:
    server(/* args */); //+server()
    ~server();
    bool isFree();//+isFree() const: bool
    void setBusy();//+setBusy(): void
    void setFree();//+setFree(): void
    void setTransactionTime(int);//+setTransactionTime(int) : void
    void setTransactionTime();//+setTransactionTime(): void
    int getRemainingTransactionTime();//+getRemainingTransactionTime() const: int
    void decreaseTransactionTime();//+decreaseTransactionTime(): void
    void setCurrentTask(task);//+setCurrentTask(task): void
    int getCurrentTaskNumber();//+getCurrentTaskNumber() const: int
    int getCurrentTaskArrivalTime();//+getCurrentTaskArrivalTime() const: int
    int getCurrentTaskWaitingTime();//+getCurrentTaskWaitingTime() const: int
    int getCurrentTaskTransactionTime();//+getCurrentTaskTransactionTime() const: int
};

class server_list
{
private:
    int numOfServers;//-numOfServers: int
    vector<server> servers;//-*servers: server
public:
    server_list();//+serverList(int = 1)
    ~server_list();
    int getFreeServerID();//+getFreeServerID() const: int
    int getNumberOfBusyServers();//+getNumberOfBusyServers() const: int
    void setServerBusy(int, task, int);//+setServerBusy(int, task, int): void
    void setServerBusy(int, task);//+setServerBusy(int, task): void
    void updateServers();//+updateServers(): void
    
};



int main() {
    //server time (# tottal ticks(itterations of main loop))
    //# of servers
    //# of queues
    //# of tasks
    //task transaction time range
    //task delevery delay (in ticks)
    return(0);
}

task::task(int task_number, int arrival_time, int waiting_time, int transaction_time)
{
}

task::~task()
{
}


server::server(/* args */)
{
}

server::~server()
{
}


server_list::server_list()
{
}

server_list::~server_list()
{
}