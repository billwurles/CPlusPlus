//mpiCC -o exec dist.cpp -Wall -std=c++0x && mpirun -nodes 5 exec
#include <iostream>
#include <mpi.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <thread>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

#define n 200

bool converged;
class Timer {
public:
    Timer();
    void time(int);
    double get(int, int);
    double getms(int, int);
    double getAvg();
private:
    double timers[3];
    double avg;
    int cnt;
};
Timer::Timer(){
    avg = 0;
    cnt = 0;
}
double Timer::getAvg(){
    return avg / cnt;
}
double Timer::get(int timer1, int timer2){
    return this->getms(timer1, timer2) / 1000;
}
void::Timer::time(int timer){
    if(timer < 4 && timer > 0){
        timers[timer-1] = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::monotonic_clock::now().time_since_epoch()).count();
    }
}
double Timer::getms(int timer1, int timer2){
    double result = -1.0;
    if(timer1 < 4 && timer1 > 0 && timer2 < 4 && timer2 > 0){
        result = (timers[timer2-1] - timers[timer1-1]);
    }
    if(timer1 == 2) avg+=result; cnt++;
    return result;
}
// void Timer::time(int timer){
// }
// double Timer::getms(int timer1, int timer2){
//     return 9999.0;
// }
struct calcArgs {
    int start, end, Its;
    double m[n][n], b[n], x[n], prv_x[n], sum;
    int rank, nodes, nodeShare;
    int length, tag, message[1];
    char name[MPI_MAX_PROCESSOR_NAME+1];
    Timer *timer;
    MPI_Status status;
};
void calculate(struct calcArgs *args, int start, int end){
    for(register int i=start; i<end; i++){
    	double sum=0.0;
        for(register int j=0;j<n;j++)
            if(i!=j)sum+=args->m[i][j]*args->x[j];
        args->x[i]=(args->b[i]-sum)/args->m[i][i]; 
    }
}

void execute(struct calcArgs *args, int share, int procs){
//     calculate(args, args->start, args->end);
   register int p, start, end;
   start = args->start;
   end = args->start;
   std::vector<std::thread> threads;
   for(p = 0; p < procs; p++){
       end+=share;
      threads.emplace_back(calculate, args, start, end);
      start = end;
  }
  for(p = 0; p < procs; p++) threads[p].join();
   args->Its++;
}
bool convergedCheck(struct calcArgs *args){
    for(register int i = args->start; i < args->end; i++)
        if(args->x[i] != args->prv_x[i]) return false;
    return true;
}

void construct(struct calcArgs *args){
    register int r,c,d;
    args->Its = 0;
    for(r=0;r<n;r++){
        for(c=0;c<n;c++){
            d=abs(r-c);
            if(d==0)args->m[c][r]=-2.0;
            if(d==1)args->m[c][r]= 1.0;
            if(d>1 )args->m[c][r]= 0.0;
        }
        args->b[r]=0.0;
        args->x[r]=1.0;
    }
    args->b[0]=-1.0*(double)(n+1);
}

void printNode(struct calcArgs *args, int amount, int conv){
    register int i;
    printf("N:[%d] Its=%d\t",args->rank, args->Its);
    for(i = args->start; i < args->start+amount; i++) printf(" x[%d]=%8.5f",i,args->x[i]);
    if(conv) printf(" -- CONVERGED");
    printf("\n");
}
void print(struct calcArgs *args, int start, int end){
    register int i;
    args->timer->time(3);
    printf("Its=%d \t",args->Its);
    for(i = start; i < end; i++) printf(" x[%d]=%8.5f",i,args->x[i]);
    printf(" ... [%1.0fms]\n",args->timer->getms(2, 3));
    args->timer->time(2);
}

int main(int argc, char * argv[]) {
    Timer *timer = new Timer();
    timer->time(1);
    struct calcArgs args;
    int checkFrequency = 1000;
    int procs;
    srand((unsigned int) time(NULL));
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SERIALIZED, &procs); 
    MPI_Comm_rank(MPI_COMM_WORLD,&args.rank);
    MPI_Comm_size(MPI_COMM_WORLD,&args.nodes);
    MPI_Get_processor_name(args.name, &args.length);
    args.timer = timer;
    args.nodeShare = n / (args.nodes - 1);
    args.tag = 50;
    args.Its = 0;
    
    if(procs == 0) procs = 1;
    int share = args.nodeShare / procs;
    int starts[args.nodes], ends[args.nodes];
    if(args.rank==0){
        printf("There are %d systems in this cluster and %d cores on this system\n",args.nodes, procs);
    }
    printf("NODE:[%d]-[%s][%d cores] INITIALISING\n",args.rank,args.name,procs);
    if(args.rank==0){ // master
        construct(&args);
        int start = 0, end = args.nodeShare;
        register int dest;
        for(dest = 1; dest < args.nodes; dest++){
            if(dest == (args.nodes-1)) end = n;
            int shareBuf[] = {start, end};
            starts[dest] = start;
            ends[dest] = end;
            MPI_Send(&shareBuf, 2, MPI_INT, dest, args.tag, MPI_COMM_WORLD);
            start = end;
            end+=args.nodeShare;
        }
        timer->time(2);
        printf("constructing took %3.2fms\n",timer->getms(1,2));
    }
    else { // slave
        int shareBuf[2];
        construct(&args);
        MPI_Recv(shareBuf, n, MPI_INT, 0, args.tag, MPI_COMM_WORLD, &args.status);
        args.start =  shareBuf[0];
        args.end = shareBuf[1];
        printf("NODE:[%d] [%d--%d]\t",args.rank,args.start, args.end);
        printNode(&args, 3, 0);
    }
    args.message[0] = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    timer->time(2);
    while(!converged){
        MPI_Bcast(args.message, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(args.x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if(args.rank==0){ // master
            int convCount = 0;
            if(args.message[0] == 1){
                converged = true;
                break;
            }
            for(register int source = 1; source < args.nodes; source++){
                double xBuffer[n];
                MPI_Recv(args.message,1,MPI_INT,source,args.tag,MPI_COMM_WORLD,&args.status);
                MPI_Recv(xBuffer,n,MPI_DOUBLE,source,args.tag,MPI_COMM_WORLD,&args.status);
                convCount += args.message[0];
                for(register int i = starts[source]; i < ends[source]; i++)
                    args.x[i] = xBuffer[i];
            }
            if(args.Its%checkFrequency==0){
                if(convCount == (args.nodes - 1)) args.message[0] = 1;
                else args.message[0] = 0;
                print(&args, 0, 3);
            }
            args.Its++;
        } else {
            if(!args.message[0]){
                execute(&args, share, procs);
                if(args.Its%checkFrequency==0){
                    args.message[0] = convergedCheck(&args);
                    if(args.message[0] == 0) for(register int i = args.start; i < args.end; i++) args.prv_x[i] = args.x[i];
//                     printNode(&args, 3, args.message[0]);
                }
                MPI_Send(args.message, 1, MPI_INT, 0, args.tag, MPI_COMM_WORLD);
                MPI_Send(args.x, n, MPI_DOUBLE, 0, args.tag, MPI_COMM_WORLD);
            } else converged = true;
        }
    }
    if(args.rank == 0){
        timer->time(3);
	printf("\nThat took me %5.2f seconds in total with an average 1000th Its time of %1.0fms.\n",(args.timer->get(1,2)),args.timer->getAvg());
    }
    
    
    MPI_Finalize();
    return 0;
}
