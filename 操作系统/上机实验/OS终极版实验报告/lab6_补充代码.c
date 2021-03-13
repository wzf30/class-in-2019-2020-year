//MFQC： 
// Multilevel Feedback Queue Scheduling 实现多级反馈队列 

#define num_Queue 5

struct proc_struct {
    enum proc_state state;                      // Process state
    int pid;                                    // Process ID
    int runs;                                   // the running times of Proces
    uintptr_t kstack;                           // Process kernel stack
    volatile bool need_resched;                 // bool value: need to be rescheduled to release CPU?
    struct proc_struct *parent;                 // the parent process
    struct mm_struct *mm;                       // Process's memory management field
    struct context context;                     // Switch here to run process
    struct trapframe *tf;                       // Trap frame for current interrupt
    uintptr_t cr3;                              // CR3 register: the base addr of Page Directroy Table(PDT)
    uint32_t flags;                             // Process flag
    char name[PROC_NAME_LEN + 1];               // Process name
    list_entry_t list_link;                     // Process link list 
    list_entry_t hash_link;                     // Process hash list
    int exit_code;                              // exit code (be sent to parent proc)
    uint32_t wait_state;                        // waiting state
    struct proc_struct *cptr, *yptr, *optr;     // relations between processes
    struct run_queue *rq;                       // running queue contains Process
    list_entry_t run_link;                      // the entry linked in run queue
	
    int time_slice;                             // time slice for occupying the CPU
    int queue;                                  //记录该进程在哪个队列中
};


struct sched_class default_sched_class = {
    .name = "MFQC_scheduler",                    //将默认调度器设置为多级反馈队列
    .init = RR_init,
    .enqueue = RR_enqueue,
    .dequeue = RR_dequeue,
    .pick_next = RR_pick_next,
    .proc_tick = RR_proc_tick,
};

struct run_queue {                         
    list_entry_t run_list[num_Queue];                //run_queue 需要设置多个链表作为多级队列
    unsigned int proc_num;
	unsigned int procQueue_num[num_Queue];           //记录每一个队列的进程个数
    int max_time_slice;                          
    // For LAB6 ONLY
    skew_heap_entry_t *lab6_run_pool;                   
};

static void
MFQC_init(struct run_queue *rq) {
	int i;
	for(i = 0; i < num_Queue; i++)              //需要对每一个队列进行初始化
    {
		list_init(&(rq->run_list[i]));
		rq->procQueue_num[i] = 0;                //设置个数为0
	}
    rq->proc_num = 0;                  
}

static void
MFQC_enqueue(struct run_queue *rq, struct proc_struct *proc, int q) {          //新增参数
    assert(list_empty(&(proc->run_link)));
    list_add_before(&(rq->run_list[q]), &(proc->run_link));              //加入第q个队列
    if (proc->time_slice == 0 || proc->time_slice > rq->max_time_slice) {
        proc->time_slice = rq->max_time_slice;
    }
    proc->rq = rq;
	proc->queue = q;
    rq->proc_num ++;
	rq->procQueue_num[q]++;
}

static void
MFQC_dequeue(struct run_queue *rq, struct proc_struct *proc) {
    assert(!list_empty(&(proc->run_link)) && proc->rq == rq);
    list_del_init(&(proc->run_link));  
    rq->proc_num --;                         //总数-1
	rq->procQueue_num[proc->queue]--;            //相应队列进程个数-1
}

static struct proc_struct *
MFQC_pick_next(struct run_queue *rq) {      
    list_entry_t *le;
	int i = 0;
	for(i = 0; i < num_Queue; i++)             //从序号最小的序列开始遍历
		if(rq->procQueue_num[i])
		{
			le = list_next(&(rq->run_list));
			break;
		}
		
    if (le != &(rq->run_list)) {
        return le2proc(le, run_link);
    }
    return NULL;
}

static void
MFQC_proc_tick(struct run_queue *rq, struct proc_struct *proc) {     
    if (proc->time_slice > 0) {
        proc->time_slice --;
    }
    if (proc->time_slice == 0) {                      //如果时间片用完
        proc->need_resched = 1;
		if(proc->queue ==  num_Queue - 1)            //如果该进程已经在最后一个队列
				MFQC_enqueue(rq, proc, proc->queue);
		else
		{
			MFQC_enqueue(rq, proc, proc->queue + 1);
			proc->queue++;                             //往后移一个队列
		}
		proc->proc_num++;
		rq->procQueue_num[proc->queue]++;
    }
}
