//vmm,c
int
do_pgfault(struct mm_struct *mm, uint32_t error_code, uintptr_t addr) {
     ....
    /*LAB3 EXERCISE 1: YOUR CODE
    * Maybe you want help comment, BELOW comments can help you finish the code
    *
    * Some Useful MACROs and DEFINEs, you can use them in below implementation.
    * MACROs or Functions:
    *   get_pte : get an pte and return the kernel virtual address of this pte for la
    *             if the PT contians this pte didn't exist, alloc a page for PT (notice the 3th parameter '1')
    *   pgdir_alloc_page : call alloc_page & page_insert functions to allocate a page size memory & setup
    *             an addr map pa<--->la with linear address la and the PDT pgdir
    * DEFINES:
    *   VM_WRITE  : If vma->vm_flags & VM_WRITE == 1/0, then the vma is writable/non writable
    *   PTE_W           0x002                   // page table/directory entry flags bit : Writeable
    *   PTE_U           0x004                   // page table/directory entry flags bit : User can access
    * VARIABLES:
    *   mm->pgdir : the PDT of these vma
    *
    */
	
	ptep = get_pte(mm->pgdir, addr, 1);              // 获取当前发生缺页的虚拟页对应的PTE
	
	if (ptep == NULL) {
        cprintf("get_pte in do_pgfault failed\n");
        goto failed;
    }
	
	 if (*ptep == 0) { // if the phy addr isn't exist, then alloc a page & map the phy addr with logical addr
        if (pgdir_alloc_page(mm->pgdir, addr, perm) == NULL) {
            cprintf("pgdir_alloc_page in do_pgfault failed\n");
            goto failed;
        }
    }
	
	else { // if this pte is a swap entry, then load data from disk to a page with phy addr
           // and call page_insert to map the phy addr with logical addr
		   // 如果需要的物理页是没有分配而不是被换出到外存中，区别上面的情况
        if(swap_init_ok) {                                  //// 判断是否当前交换机制正确被初始化
            struct Page *page=NULL;
            if ((ret = swap_in(mm, addr, &page)) != 0) {               // 将物理页换入到内存中
                cprintf("swap_in in do_pgfault failed\n");
                goto failed;
            }    
            page_insert(mm->pgdir, page, addr, perm);               // 将物理页与虚拟页建立映射关系
            swap_map_swappable(mm, addr, page, 1);                // 设置当前的物理页为可交换的
        }
        else {
            cprintf("no swap_init_ok but ptep is %x, failed\n",*ptep);
            goto failed;
        }
   }
   ret = 0;
failed:
    return ret;
}

//swap_fifo.c
static int
_fifo_map_swappable(struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in)
{
    list_entry_t *head=(list_entry_t*) mm->sm_priv;              // 找到链表入口
    list_entry_t *entry=&(page->pra_page_link);                 // 找到当前物理页用于组织成链表的list_entry_t
 
    assert(entry != NULL && head != NULL);                         // 进行一系列检查
    //record the page access situlation
    /*LAB3 EXERCISE 2: YOUR CODE*/ 
    //(1)link the most recent arrival page at the back of the pra_list_head qeueue.
    list_add(head, entry);                                  // 将当前指定的物理页插入到链表的末尾
    return 0;
}

/*
 *  (4)_fifo_swap_out_victim: According FIFO PRA, we should unlink the  earliest arrival page in front of pra_list_head qeueue,
 *                            then set the addr of addr of this page to ptr_page.
 */

static int
_fifo_swap_out_victim(struct mm_struct *mm, struct Page ** ptr_page, int in_tick)
{
     list_entry_t *head=(list_entry_t*) mm->sm_priv;        // 找到链表的入口
         assert(head != NULL);                            // 进行一系列检查
     assert(in_tick==0);
     /* Select the victim */
     /*LAB3 EXERCISE 2: YOUR CODE*/ 
     //(1)  unlink the  earliest arrival page in front of pra_list_head qeueue
     //(2)  set the addr of addr of this page to ptr_page
     /* Select the tail */
     list_entry_t *le = head->prev;                         // 取出链表头，即最早进入的物理页面
     assert(head!=le);                                          
     struct Page *p = le2page(le, pra_page_link);           // 找到对应的物理页面的Page结构               
     list_del(le);                                             // 从链表上删除取出的即将被换出的物理页面                       
     assert(p !=NULL);
     *ptr_page = p;
     return 0;
}
