#include <iostream> //basic input output
#include <string>   //because using a string for status instead of a bool
#include <vector>   //actually very simaler to deque, but what i'm used to for dynamic arrays and already wrote some parts using it before i added
#include <queue>    //using deque (double ended queue) from included library because it's traversable
using namespace std;

/*
implemented mostly just using outlined data structure in assinment, which could have been made more effeicent but aside from the server list i didn't care enough to try
my server list deviates because i didn't want gross extra loops, so 1 loop through and let them queue themselves
also updated main loop to run better with updated server list (plus actually loading in task on first loop)
*/

class task
{
private:
    int task_number;      //-task_number: int
    int arrival_time;     //-arrival_time: int
    int waiting_time;     //-waiting_time: int
    int transaction_time; //-transaction_time: int
public:
    task(int = 0, int = 0, int = 0, int = 0); //+task(int = 0, int = 0, int = 0, int = 0)
    ~task();
    void setCustomerInfo(int = 0, int = 0, int = 0, int = 0); //+setCustomerInfo(int = 0, int = 0, int = 0, int = 0): void
    int getWaitingTime();                                     //+getWaitingTime() : int
    void setWaitingTime(int);                                 //+setWaitingTime(int): void
    void incrementWaitingTime();                              //+incrementWaitingTime(): void
    int getArrivalTime();                                     //+getArrivalTime() const: int
    int getTransactionTime();                                 //+getTransactionTime() const: int
    int getTaskNumber();                                      //+getTaskNumber() const: int
};

class server
{
private:
    task *currentTask;   //-currentTask: task
    string status;       //-status: string
    int transactionTime; //-transactionTime: int
public:
    bool isQueued = 0;  // if server is queued to be loaded with new task
    server(/* args */); //+server()
    ~server();
    bool isFree();                       //+isFree() const: bool
    void setBusy();                      //+setBusy(): void
    void setFree();                      //+setFree(): void
    void setTransactionTime(int);        //+setTransactionTime(int) : void
    void setTransactionTime();           //+setTransactionTime(): void
    int getRemainingTransactionTime();   //+getRemainingTransactionTime() const: int
    void decreaseTransactionTime();      //+decreaseTransactionTime(): void
    void setCurrentTask(task *, int);         //+setCurrentTask(task): void
    int getCurrentTaskNumber();          //+getCurrentTaskNumber() const: int
    int getCurrentTaskArrivalTime();     //+getCurrentTaskArrivalTime() const: int
    int getCurrentTaskWaitingTime();     //+getCurrentTaskWaitingTime() const: int
    int getCurrentTaskTransactionTime(); //+getCurrentTaskTransactionTime() const: int
};

class server_list
{
private:
    int numOfServers;         //-numOfServers: int
    vector<server *> servers; //-*servers: server
    deque<unsigned int> freeQueue;

public:
    server_list(); //+serverList(int = 1)
    ~server_list();
    void setServers(int);
    int getFreeServerID();                //+getFreeServerID() const: int
    int getNumberOfBusyServers();         //+getNumberOfBusyServers() const: int
    void setServerBusy(int, task *, int); //+setServerBusy(int, task, int): void
    void setServerBusy(int, task *);      //+setServerBusy(int, task): void
    void updateServers();                 //+updateServers(): void
    void loadQueue(int);
    void queueFree(unsigned int);
};

unsigned int promptInt(string prompt)
{
    cout << prompt << ": ";
    unsigned int in;
    cin >> in;
    while ((getchar()) != '\n')
        ; // cin.ignore(numeric_limits<streamsize>::max(),'\n'); //wipe input buffer
    return in;
}
//---------------------------------------------------------------------------------------------------------------------
server_list SERVERS;
vector<deque<task *> *> TASKS;
int TASKNUM = 0;

int main()
{
    cout << "MockSim - BaileyK" << endl;
    unsigned int TICKS = promptInt("Please input alloted server ticks");   // server time (# tottal ticks(itterations of main loop))
    unsigned int NUMSERVERS = promptInt("Please input number of servers"); //# of servers
    SERVERS.setServers(NUMSERVERS);
    unsigned int NUMQUEUES = promptInt("Please input number of queues"); //# of queues
    for (size_t i = 0; i < NUMQUEUES; i++)
    {
        TASKS.push_back(new deque<task *>);
    }

    bool RAND_QUEUE = false;
    if (NUMQUEUES > 1)
    {
        cout << "would you like to randomly decide which queue task gets added to? (y/n): ";
        char ans;
        cin >> ans;
        while ((getchar()) != '\n')
            ;
        if (ans == 'y')
        {
            RAND_QUEUE = true;
        }
    }

    unsigned int MAXTASKS = promptInt("Please input MAX number of tasks");      //# of tasks
    unsigned int TIMEMIN = promptInt("please input min task transaction time"); // task transaction time range
    unsigned int TIMEMAX = promptInt("     and now max task transaction time");
    unsigned int TICKDELAY = promptInt("Please input task delay (in ticks)"); // task delevery delay (in ticks)
    unsigned int seed = promptInt("input seed (or 0 for default, 1 for static)");
    if (seed = 1)
    {
        srand(42069626);
    }
    else if (seed > 1)
    {
        srand(seed);
    }

    int queue = 0;
    int tick = 0;
    // for (size_t tick = 0; tick < TICKS; tick++)//loop until server time out
    do // main loop
    {
        cout << endl
             << "tick:" << tick << ":" << endl;
        if (tick % TICKDELAY == 0 && TASKNUM < MAXTASKS) // see if new task needs added
        {
            if (RAND_QUEUE)
            {
                queue = rand() % TASKS.size(); // randome num from 0 -- size-1
            }
            int time = rand() % (TIMEMAX - TIMEMIN + 1) + TIMEMIN;
            task *add = new task(TASKNUM, tick, 0, time);
            cout << "Adding task :" << TASKNUM << ": to queue :" << queue << ": with " << time << " required ticks to finish. mem addr :" << add << ":" << endl;
            deque<task *> *QUEUE = TASKS[queue];
            QUEUE->push_back(add);
            TASKNUM++;
        }
        SERVERS.updateServers(); // update servers - decraments time (unloads task if complete)/ if empty and not queued up queues for new task
        SERVERS.loadQueue(tick);//loads tasks to servers until out of servers or tasks
        // increment tasks -note would love to calc on arrival, but hw specified incrementing every tick
        for (deque<task *> *taskList : TASKS) // for each task queue
        {
            for (task *task : *taskList) // for each task
            {
                task->incrementWaitingTime();
            }
        }
        tick++;

        if (tick == TICKS)
        {
            cout << endl
                 << "Remaining tasks debug" << endl;
            for (deque<task *> *currentQueue : TASKS)
            {
                // deque<task*> currentQueue = TASKS.front();
                cout << "New queue mem :" << &currentQueue << ": with :" << currentQueue->size() << ": tasks" << endl;
                while (currentQueue->size() != 0)
                {
                    task *task = currentQueue->front();
                    cout << "   task :" << task->getTaskNumber() << ": required ticks :" << task->getTransactionTime() << ": mem :" << task << ":" << endl;
                    currentQueue->pop_front();
                }
            }
        }

    } while (tick < TICKS);

    return (0);
}

task::task(int task_number, int arrival_time, int waiting_time, int transaction_time)
{
    this->task_number       = task_number;
    this->arrival_time      = arrival_time;
    this->waiting_time      = waiting_time;
    this->transaction_time  = transaction_time;
}

task::~task()
{
    cout << "Task :" << task_number << ": finished. total waiting time = " << this->getWaitingTime() << ". dealocating mem" << endl;
}

void task::setCustomerInfo(int taskNumber, int arrivalTime, int waitingTime, int transactionTime) //+setCustomerInfo(int = 0, int = 0, int = 0, int = 0): void
{
    this->task_number = taskNumber;
    this->arrival_time = arrivalTime;
    this->waiting_time = waitingTime;
    this->transaction_time = transactionTime;
}
int task::getWaitingTime() //+getWaitingTime() : int
{
    return this->waiting_time;
}
void task::setWaitingTime(int newTime) //+setWaitingTime(int): void
{
    this->waiting_time = newTime;
}
void task::incrementWaitingTime() //+incrementWaitingTime(): void
{
    this->waiting_time++;
}
int task::getArrivalTime() //+getArrivalTime() const: int
{
    return this->arrival_time;
}
int task::getTransactionTime() //+getTransactionTime() const: int
{
    return this->transaction_time;
}
int task::getTaskNumber() //+getTaskNumber() const: int
{
    return this->task_number;
}

server::server(/* args */)
{
}

server::~server()
{
}

bool server::isFree() //+isFree() const: bool
{
    return (this->status == "free");
}
void server::setBusy() //+setBusy(): void
{
    this->status = "busy";
    this->isQueued = false;
}
void server::setFree() //+setFree(): void
{
    if (this->currentTask != NULL)
    {
        this->currentTask->~task();
    }
    this->currentTask = NULL;
    this->status = "free";
}
void server::setTransactionTime(int time) //+setTransactionTime(int) : void
{
    this->transactionTime = time;
}
void server::setTransactionTime() //+setTransactionTime(): void
{
    this->transactionTime = this->currentTask->getTransactionTime();
}
int server::getRemainingTransactionTime() //+getRemainingTransactionTime() const: int
{
    return this->transactionTime;
}
void server::decreaseTransactionTime() //+decreaseTransactionTime(): void
{
    if (this->transactionTime > 0)
    {
        this->transactionTime--;
    }
}
void server::setCurrentTask(task *newTask, int tick) //+setCurrentTask(task): void
{
    this->currentTask = newTask;
    newTask->setWaitingTime(tick);
    this->setBusy();
    this->setTransactionTime();
}
int server::getCurrentTaskNumber() //+getCurrentTaskNumber() const: int
{
    return this->currentTask->getTaskNumber();
}
int server::getCurrentTaskArrivalTime() //+getCurrentTaskArrivalTime() const: int
{
    return this->currentTask->getArrivalTime();
}
int server::getCurrentTaskWaitingTime() //+getCurrentTaskWaitingTime() const: int
{
    return this->currentTask->getWaitingTime();
}
int server::getCurrentTaskTransactionTime() //+getCurrentTaskTransactionTime() const: int
{
    return this->currentTask->getTransactionTime();
}

server_list::server_list()
{
}

server_list::~server_list()
{
}

void server_list::setServers(int num)
{
    this->numOfServers = num;
    for (size_t i = 0; i < num; i++)
    {
        server *add = new server();
        this->servers.push_back(add);
    }
}
void server_list::updateServers() //+updateServers(): void
{
    for (int index = 0; index < this->servers.size(); index++)
    {
        server *server = this->servers[index];
        if (!(server->isQueued))
        {
            server->decreaseTransactionTime();
            if (server->getRemainingTransactionTime() == 0)
            {
                this->queueFree(index);
            }
        }
    }
}
void server_list::queueFree(unsigned int index)
{
    this->servers[index]->setFree();
    this->freeQueue.push_back(index);
    this->servers[index]->isQueued = true;
    cout << "server :" << index << ": freed" << endl;
}
void server_list::loadQueue(int tick)
{
    // deque<task*> currentTaskQueue;
    int taskQueueIndex = 0;
    while (this->freeQueue.size() > 0 && taskQueueIndex < TASKS.size()) // while there are free severs and we arn't out of task queues
    {
        // currentTaskQueue = TASKS[taskQueueIndex];
        //cout << TASKS.size() << endl;
        //cout << taskQueueIndex << endl;
        //cout << TASKS[taskQueueIndex]->empty() << endl;
        while (this->freeQueue.size() > 0 && TASKS[taskQueueIndex]->size() > 0) // while there are free servers and tasks in current queue
        {

            this->servers[this->freeQueue.front()]->setCurrentTask(TASKS[taskQueueIndex]->front(), tick);
            cout << "Server :" << this->freeQueue.front() << ": loaded task :" << TASKS[taskQueueIndex]->front()->getTaskNumber() << ":" << endl;
            this->freeQueue.pop_front();
            TASKS[taskQueueIndex]->pop_front();
        }
        // TASKS[taskQueueIndex] = currentTaskQueue;
        taskQueueIndex++;
    }
}
int server_list::getFreeServerID() //+getFreeServerID() const: int
{
    return this->freeQueue.front();
}
int server_list::getNumberOfBusyServers() //+getNumberOfBusyServers() const: int
{
    return this->servers.size() - this->freeQueue.size();
}
void server_list::setServerBusy(int, task *, int) //+setServerBusy(int, task, int): void
{
}
void server_list::setServerBusy(int, task *) //+setServerBusy(int, task): void
{
}