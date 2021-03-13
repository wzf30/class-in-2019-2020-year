//do_execve函数第六部分
//要先清空进程的中断帧,再重新设置进程的中断帧,使得在执行中断返回指令iret后
//能够让 CPU转到用户态特权级,并回到用户态内存空间,使用用户态的代码段
//数据段和堆栈,且能够跳转到用户进程的第一条指令执行,并确保在用户态能够响应中断
	tf->tf_cs = USER_CS;
    tf->tf_ds = tf->tf_es = tf->tf_ss = USER_DS;
    tf->tf_esp = USTACKTOP;
    tf->tf_eip = elf->e_entry;
    tf->tf_eflags = FL_IF;
    ret = 0;
	
	


	
//proc.h	
static struct proc_struct *
alloc_proc(void) {
 struct proc_struct *proc = kmalloc(sizeof(struct proc_struct));
    if (proc != NULL) {
        proc->state = PROC_UNINIT;
        proc->pid = -1;
        proc->runs = 0;
        proc->kstack = 0;
        proc->need_resched = 0;
        proc->parent = NULL;
        proc->mm = NULL;
        memset(&(proc->context), 0, sizeof(struct context));
        proc->tf = NULL;
        proc->cr3 = boot_cr3;
        proc->flags = 0;
        memset(proc->name, 0, PROC_NAME_LEN);
        proc->wait_state = 0; //pcb新增的条目，初始化进程等待状态
        proc->cptr = proc->optr = proc->yptr = NULL; //设置指针,表示进程间的关系
    }
    return proc;
}



int
do_fork(uint32_t clone_flags, uintptr_t stack, struct trapframe *tf) {
    int ret = -E_NO_FREE_PROC;
    struct proc_struct *proc;
    if (nr_process >= MAX_PROCESS) {
        goto fork_out;
    }
    ret = -E_NO_MEM;
    if ((proc = alloc_proc()) == NULL) {
        goto fork_out;
    }
    proc->parent = current;
    assert(current->wait_state == 0); //确保进程在等待
    if (setup_kstack(proc) != 0) {
        goto bad_fork_cleanup_proc;
    }
    if (copy_mm(clone_flags, proc) != 0) {
        goto bad_fork_cleanup_kstack;
    }
    copy_thread(proc, stack, tf);
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        proc->pid = get_pid();
        hash_proc(proc);
        set_links(proc); //设置进程链接
		//list_add(&proc_list, &(proc->list_link));       //加入进程链表
        //nr_process ++;                    //进程数+1
    }
    local_intr_restore(intr_flag);
    wakeup_proc(proc);
    ret = proc->pid;
fork_out:
    return ret;
bad_fork_cleanup_kstack:
    put_kstack(proc);
bad_fork_cleanup_proc:
    kfree(proc);
    goto fork_out;
}


//trap.c
void
idt_init(void) {
    extern uintptr_t __vectors[];
    int i;
    for (i = 0; i < sizeof(idt) / sizeof(struct gatedesc); i ++) {
        SETGATE(idt[i], 0, GD_KTEXT, __vectors[i], DPL_KERNEL);
    }
    SETGATE(idt[T_SYSCALL], 1, GD_KTEXT, __vectors[T_SYSCALL], DPL_USER);   //设置系统调用相应的中断描述符
    lidt(&idt_pd);
}

static void
trap_dispatch(struct trapframe *tf) {
...
if (ticks % TICK_NUM == 0) {
            assert(current != NULL);
            current->need_resched = 1;         //当时间片用完，设置为可调度
        }
		
		
		
//pmm.c
//copy range
void * src = page2kva(page);     //获取父进程的内核虚拟页地址 
void * dst = page2kva(npage);     //获取子进程的内核虚拟页地址 
memcpy(dst, src, PGSIZE);            //进行复制

ret = page_insert(to, npage, start, perm);    //建立好物理映射关系
assert(ret == 0);