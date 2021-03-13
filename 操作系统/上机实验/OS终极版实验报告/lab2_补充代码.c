static void
default_init_memmap(struct Page *base, size_t n) {
    assert(n > 0);              
    struct Page *p = base;
    for (; p != base + n; p ++) {        //初始化n块物理页
        assert(PageReserved(p));         //检查此页是否为保留页
        p->flags = 0;                     //标志位清0
        SetPageProperty(p);                  //设置标志位为1
        p->property = 0;               
        set_page_ref(p, 0);                   //清除引用此页的虚拟页的个数
        list_add_before(&free_list, &(p->page_link));      //加入空闲链表
    }
    nr_free += n;                    //计算空闲页总数
    //first block
    base->property = n;                 //修改base的连续空页值为n
}

static struct Page *
default_alloc_pages(size_t n) {
    assert(n > 0);                  //判断n是否大于0
    if (n > nr_free) {            //需要分配页的个数小于空闲页的总数,直接返回
        return NULL;
    }
    struct Page *page = NULL;
    list_entry_t *le = &free_list, *le2;
    while ((le = list_next(le)) != &free_list) {       //遍历整个空闲链表
        struct Page *p = le2page(le, page_link);
        if (p->property >= n) {                //找到合适的空闲页
            page = p;
            break;
        }
    }
    if (page != NULL) {
        int i;
        for(i = 0; i < n; i++){
            le2 = list_next(le);
            struct Page *pp = le2page(le, page_link);      
            SetPageReserved(pp);                       //设置每一页的标志位
            ClearPageProperty(pp);
            list_del(le);                            //将此页从free_list中清除
            le = le2;
        }
        if (page->property > n) {            //如果页块大小大于所需大小，分割
           (le2page(le,page_link))->property = page->property - n;
        }
        nr_free -= n;
        ClearPageProperty(page);
        SetPageReserved(page);
    }
    return page;
}

static void
default_free_pages(struct Page *base, size_t n) {
    assert(n > 0);
    assert(PageReserved(base));            //检查需要释放的页块是否已经被分配

    list_entry_t *le = &free_list;
    struct Page * p;
    while((le=list_next(le)) != &free_list) {       //寻找合适的位置
      p = le2page(le, page_link);
      if(p>base){
        break;
      }
    }
    //list_add_before(le, base->page_link);
    for(p=base;p<base+n;p++){
      list_add_before(le, &(p->page_link));       //将每一空闲块对应的链表插入空闲链表中
    }
    base->flags = 0;
    set_page_ref(base, 0);
    ClearPageProperty(base);
    SetPageProperty(base);
    base->property = n;                       //设置连续大小为n
    
    p = le2page(le,page_link) ;            //如果是高位，则向高地址合并
    if( base+n == p ){
      base->property += p->property;
      p->property = 0;
    }
    le = list_prev(&(base->page_link));         //如果是低位且在范围内，则向低地址合并
    p = le2page(le, page_link);
    if(le!=&free_list && p==base-1){
      while(le!=&free_list){
        if(p->property){
          p->property += base->property;
          base->property = 0;
          break;
        }
        le = list_prev(le);
        p = le2page(le,page_link);
      }
    }

    nr_free += n;
    return ;
}

//自己实现的best-fit
//最佳适配
static struct Page *
default_alloc_pages(size_t n) {
    assert(n > 0);
    if (n > nr_free) {
        return NULL;
    }
    struct Page *page = NULL;
    list_entry_t *le = &free_list, *le2;
    while ((le = list_next(le)) != &free_list) {          //遍历整个空闲链表
        struct Page *p = le2page(le, page_link);
		if(page == NULL)
			cprintf("当前遍历到的页面: %d, 目前最优页面为: %d", p->property, 0);
		else
			cprintf("当前遍历到的页面: %d, 目前最优页面为: %d", p->property, page->property);
		
        if (page == NULL || (p->property >= n && p->property < page->property)) {    //找到合适的空闲页
            page = p;
        }
    }
    if (page != NULL) {
        int i;
        for(i = 0; i < n; i++){
            le2 = list_next(le);
            struct Page *pp = le2page(le, page_link);
            SetPageReserved(pp);                   //设置每一页的标志位
            ClearPageProperty(pp);
            list_del(le);
            le = le2;
        }
        if (page->property > n) {
           (le2page(le,page_link))->property = page->property - n;
        }
        nr_free -= n;
        ClearPageProperty(page);
        SetPageReserved(page);
    }
    return page;
}


//pmm.c

//get_pte - get pte and return the kernel virtual address of this pte for la
//        - if the PT contians this pte didn't exist, alloc a page for PT
// parameter:
//  pgdir:  the kernel virtual base address of PDT
//  la:     the linear address need to map
//  create: a logical value to decide if alloc a page for PT
// return vaule: the kernel virtual address of this pte
pte_t *
get_pte(pde_t *pgdir, uintptr_t la, bool create) {
    /* LAB2 EXERCISE 2: YOUR CODE
     *
     * If you need to visit a physical address, please use KADDR()
     * please read pmm.h for useful macros
     *
     * Maybe you want help comment, BELOW comments can help you finish the code
     *
     * Some Useful MACROs and DEFINEs, you can use them in below implementation.
     * MACROs or Functions:
     *   PDX(la) = the index of page directory entry of VIRTUAL ADDRESS la.
     *   KADDR(pa) : takes a physical address and returns the corresponding kernel virtual address.
     *   set_page_ref(page,1) : means the page be referenced by one time
     *   page2pa(page): get the physical address of memory which this (struct Page *) page  manages
     *   struct Page * alloc_page() : allocation a page
     *   memset(void *s, char c, size_t n) : sets the first n bytes of the memory area pointed by s
     *                                       to the specified value c.
     * DEFINEs:
     *   PTE_P           0x001                   // page table/directory entry flags bit : Present
     *   PTE_W           0x002                   // page table/directory entry flags bit : Writeable
     *   PTE_U           0x004                   // page table/directory entry flags bit : User can access
     */
#if 0
    pde_t *pdep = NULL;   // (1) find page directory entry
    if (0) {              // (2) check if entry is not present
                          // (3) check if creating is needed, then alloc page for page table
                          // CAUTION: this page is used for page table, not for common data page
                          // (4) set page reference
        uintptr_t pa = 0; // (5) get linear address of page
                          // (6) clear page content using memset
                          // (7) set page directory entry's permission
    }
    return NULL;          // (8) return page table entry
#endif
    pde_t *pdep = &pgdir[PDX(la)];
    if (!(*pdep & PTE_P)) {
        struct Page *page;
        if (!create || (page = alloc_page()) == NULL) {
            return NULL;
        }
        set_page_ref(page, 1);
        uintptr_t pa = page2pa(page);
        memset(KADDR(pa), 0, PGSIZE);
        *pdep = pa | PTE_U | PTE_W | PTE_P;
    }
    return &((pte_t *)KADDR(PDE_ADDR(*pdep)))[PTX(la)];
}


//page_remove_pte - free an Page sturct which is related linear address la
//                - and clean(invalidate) pte which is related linear address la
//note: PT is changed, so the TLB need to be invalidate 
static inline void
page_remove_pte(pde_t *pgdir, uintptr_t la, pte_t *ptep) {
    /* LAB2 EXERCISE 3: YOUR CODE
     *
     * Please check if ptep is valid, and tlb must be manually updated if mapping is updated
     *
     * Maybe you want help comment, BELOW comments can help you finish the code
     *
     * Some Useful MACROs and DEFINEs, you can use them in below implementation.
     * MACROs or Functions:
     *   struct Page *page pte2page(*ptep): get the according page from the value of a ptep
     *   free_page : free a page
     *   page_ref_dec(page) : decrease page->ref. NOTICE: ff page->ref == 0 , then this page should be free.
     *   tlb_invalidate(pde_t *pgdir, uintptr_t la) : Invalidate a TLB entry, but only if the page tables being
     *                        edited are the ones currently in use by the processor.
     * DEFINEs:
     *   PTE_P           0x001                   // page table/directory entry flags bit : Present
     */
#if 0
    if (0) {                      //(1) check if page directory is present
        struct Page *page = NULL; //(2) find corresponding page to pte
                                  //(3) decrease page reference
                                  //(4) and free this page when page reference reachs 0
                                  //(5) clear second page table entry
                                  //(6) flush tlb
    }
#endif
    if (*ptep & PTE_P) {
        struct Page *page = pte2page(*ptep);
        if (page_ref_dec(page) == 0) {
            free_page(page);
        }
        *ptep = 0;
        tlb_invalidate(pgdir, la);
    }
}