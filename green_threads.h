typedef  void  (__cdecl*GtCallback)(void *data); //implement grean thread by implementing this callback
void gt_init(); //initialize the library
int gt_start(GtCallback cb,void *data); //start a new green thread. the data variable is passed the callback
bool gt_next(int gtid); //used it coroutines to fetch the next yielded value
void gt_yield(bool posted); //give up execution of the current thread. optionaly post a value to
