typedef  void  (__cdecl*GtCallback)(void *data); 
void gt_init();
int gt_start(GtCallback cb,void *data);
bool gt_next(int gtid);
void gt_yield(bool posted); 
