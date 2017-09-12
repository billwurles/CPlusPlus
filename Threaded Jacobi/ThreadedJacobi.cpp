#include <thread>
#include <time.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <vector>

#define n 200

bool converged;
std::mutex xMutex;

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
void::Timer::time(int timer){
    if(timer < 4 && timer > 0){
        timers[timer-1] = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
}
double Timer::get(int timer1, int timer2){
    return this->getms(timer1, timer2) / 1000;
}
double Timer::getms(int timer1, int timer2){
    double result = -1.0;
    if(timer1 < 4 && timer1 > 0 && timer2 < 4 && timer2 > 0){
        result = (timers[timer2-1] - timers[timer1-1]);
    }
    if(timer1 == 2) avg+=result; cnt++;
    return result;
}
double Timer::getAvg(){
    return avg / cnt;
}
struct calcArgs {
    Timer *timer;
    int start;
    int end;
    int threadNum;
    int Its;
    double m[n][n], b[n], x[n], prv_x[n];
};
void calculate(struct calcArgs *args, int start, int end){
    for(register int i=start; i<end; i++){
        double sum=0.0;
        for(register int j=0;j<n;j++)
            if(i!=j) sum+=args->m[i][j]*args->x[j];
//        xMutex.lock();
        args->x[i]=(args->b[i]-sum)/args->m[i][i];
//        xMutex.unlock();
    }
}
void execute(struct calcArgs *args, int share, int procs){
//    calculate(args,0,n);
    register int p, start, end;
    start = 0;
    end = 0;
    std::vector<std::thread> threads;
    for(p = 0; p < procs; p++){
        end+=share;
        threads.emplace_back(calculate, args, start, end);
        start = end;
    }
    for(p = 0; p < procs; p++) threads[p].join();
    threads.clear();
    args->Its++;
}

void convergedCheck(struct calcArgs *args){
    register int i;
    for(i = 0; i < n; i++) if(args->x[i] != args->prv_x[i]){
        converged = false;
        break;
    }
    if(!converged) for(i = 0; i < n; i++) args->prv_x[i] = args->x[i];
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

void print(struct calcArgs *args, int start, int end){
    register int i;
    args->timer->time(3);
    printf("Its=%d \t",args->Its);
    for(i = start; i < end; i++) printf(" x[%d]=%8.5f",i,args->x[i]);
    printf(" ... [%1.0fms]\n",args->timer->getms(2, 3));
    args->timer->time(2);
}


int main(int argc, char * argv[]){
    struct calcArgs args;
    construct(&args);
    int procs = std::thread::hardware_concurrency();
    int share = n / procs;
    printf("There are %d cores on this system\n",procs);
    printf("Its=0\t\t x[%d]=%1.7f x[%d]=%1.7f x[%d]=%1.7f x[%d]=%1.7f x[%d]=%1.7f x[%d]=%1.7f ... [0ms]\n",
           0, args.x[0], 1, args.x[1], 2, args.x[2], 3, args.x[3], 4, args.x[4], 5, args.x[5]);
    args.timer = new Timer();
    args.timer->time(1);
    args.timer->time(2);
    execute(&args, 0, 6);
    while(!converged){
        if(args.Its%1000==0){
            print(&args, 0, 6);
            execute(&args, share, procs);
            converged = true;
            convergedCheck(&args);
        } else execute(&args, share, procs);
    }
    args.timer->time(2);
    printf("\nThat took me %5.2f seconds in total with an average 1000th Its time of %1.0fms.\n",(args.timer->get(1,2)),args.timer->getAvg());
    return 0;
}

















