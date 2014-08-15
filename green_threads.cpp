#include "green_threads.h"
#define STACK_SIZE (1024*64)
#define MAX_GT (16)
#pragma runtime_checks( "[runtime_checks]", off )
enum GtStatus{
    gts_zomby,
    gts_ready,
    gts_posted, 
    gts_new,
};
class GreenThread{
public:
    char stack[STACK_SIZE];
    int stack_end;//stack grows back from here
    void *data;
    GtCallback cb;
    GtStatus status;
    bool posted;
    unsigned esp;
    unsigned ebp;
    unsigned tid;
};
unsigned gs_num_threads;
unsigned gs_cur_thread;
GreenThread *gs_threads[MAX_GT];
GreenThread *gs_next_thread;

GreenThread *add_thread(){
    auto ans=new GreenThread;
    ans->data=0;
    ans->cb=0;
    ans->status=gts_new;
    ans->posted=false;
    ans->esp=0;
    ans->tid=gs_num_threads;
    gs_threads[gs_num_threads]=ans;
    gs_num_threads++;
    return ans;
}
int gt_start(GtCallback cb,void *data){
    auto gt=add_thread();
    gt->cb=cb;
    gt->data=data;
    gt->esp=(unsigned)&(gt->stack_end);
    gt->ebp=0;
    return gt->tid;
}
void gt_init(){
    auto gt=add_thread();
    gt->status=gts_ready;
    gs_cur_thread=0;
}
void run_new_thread(){
    gs_next_thread->status=gts_ready;
    gs_next_thread->cb(gs_next_thread->data);
    gs_next_thread->status=gts_zomby;
    gt_yield(false);//thats pure gold right there
}
bool gt_next(int tid){
    auto gt = gs_threads[tid];
    while(true){
        switch(gt->status){
        case gts_new:
        case gts_ready:
            gt_yield(false);
            continue;
        case gts_posted:
            gt->status=gts_ready;
            return true;
        case gts_zomby:
            return false;
        }
    }
    return true;
}
unsigned get_next_thread_id(){
    for (unsigned i=gs_cur_thread+1;i != gs_cur_thread;i++){
        if (i>=gs_num_threads)
            i=0;
        auto the_status=gs_threads[i]->status;
        if (the_status==gts_ready || the_status==gts_new)
            return i;
    }
    return gs_cur_thread;
}
__declspec(noinline) void  gt_yield(bool posted){
    auto gt = gs_threads[gs_cur_thread];
    if (posted)
        gt->status=gts_posted;
    unsigned next_tid=get_next_thread_id();
    if (next_tid==gs_cur_thread)
        return;
    gs_next_thread=gs_threads[next_tid];
    gs_cur_thread=next_tid;
    unsigned cur_esp;
    unsigned cur_ebp;
    _asm{
        mov cur_esp,esp
        mov cur_ebp,ebp
    }
    gt->esp=cur_esp;
    gt->ebp=cur_ebp;
    int next_esp=gs_next_thread->esp;
    int next_ebp=gs_next_thread->ebp;
    __asm{
        mov esp,next_esp
        mov ebp,next_ebp
    }
    if (gs_next_thread->status==gts_new)
        run_new_thread();
}
