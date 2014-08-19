#include <stdio.h>
#include "green_threads.h"
struct Enumerate{
    unsigned start;
    unsigned end;
    unsigned cur;
};
void enumerate_integers(void *data){
    auto e = (Enumerate *)data;
    unsigned end=e->end;
    for (e->cur=e->start;e->cur<end;e->cur++)
        gt_yield(true);
}
void print_integrers(int start,int end){
    Enumerate e;
    e.start=start;
    e.end=end;
    int tid=gt_start(enumerate_integers,&e);
    while(gt_next(tid))
        if (e.cur%10000000==0)
            printf("%d\n",e.cur);
}
//this test program create one green thread. It enumerate though a range integers and post each using the gt_yield
void main(){
    gt_init();
    print_integrers(10,100000000);
}
