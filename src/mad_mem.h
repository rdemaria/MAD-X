#ifndef MAD_MEM_H
#define MAD_MEM_H

#ifdef _USEGC

// #define GC_DEBUG

#include <gc.h>

#define mymalloc(fn, sz)            mytrace(sz,myptrchk(fn, GC_MALLOC_IGNORE_OFF_PAGE(sz)))
#define mymalloc_atomic(fn, sz)     mytrace(sz,myptrchk(fn, GC_MALLOC_ATOMIC_IGNORE_OFF_PAGE(sz)))
#define myrealloc(fn, p, sz)        mytrace(sz,myptrchk(fn, GC_REALLOC((p),(sz))))
#define myfree(fn, p)               ((void)(GC_FREE(p), (void)fn, (p)=0))

#define mycalloc(fn, n, sz)         memset(mymalloc(fn, (n)*(sz)), 0, (n)*(sz))
#define mycalloc_atomic(fn, n, sz)  memset(mymalloc_atomic(fn, (n)*(sz)), 0, (n)*(sz))
#define myrecalloc(fn, p, osz, sz)  ((void*)((char*)memset((char*)myptrchk(fn,GC_REALLOC((p),(sz)))+(osz),0,(sz)-(osz))-(osz)))

#define mycollect()                 GC_gcollect()

#else

#define mymalloc(fn, sz)            mytrace(sz,myptrchk(fn, malloc(sz)))
#define mymalloc_atomic(fn, sz)     mytrace(sz,myptrchk(fn, malloc(sz)))
#define myrealloc(fn, p, sz)        mytrace(sz,myptrchk(fn, realloc((p),(sz))))
#define myfree(fn, p)               ((void)(free(p), (void)fn, (p)=0))

#define mycalloc(fn, n, sz)         mytrace(sz,myptrchk(fn, calloc((n),(sz))))
#define mycalloc_atomic(fn, n, sz)  mytrace(sz,myptrchk(fn, calloc((n),(sz))))
#define myrecalloc(fn, p, osz, sz)  ((void*)((char*)memset((char*)myptrchk(fn,realloc((p),(sz)))+(osz),0,(sz)-(osz))-(osz)))

#define mycollect()                 

#endif

#ifdef DEBUG_MEM
#define mytrace_limit 100000
#define mytrace(sz,expr)           ((sz)>mytrace_limit && fprintf(stderr, "mytrace:%s:%d: %ld\n", __FILE__, __LINE__, (long)(sz)),(expr))          
#else
#define mytrace(sz,expr)           (expr)
#endif

// SIGSEGV handler
void  mad_mem_handler(int sig);

// Pointer check for null
void* myptrchk(const char *caller, void *ptr);

#endif // MAD_MEM_H
