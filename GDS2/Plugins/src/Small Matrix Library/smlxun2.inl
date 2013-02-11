#define end	size%4
#define four size/4

#if four > 0
ff(4,0,op);
#endif
#if four > 1
ff(4,1,op);
#endif
#if four > 2
ff(4,2,op);
#endif
#if four > 3
ff(4,3,op);
#endif
#if four > 4
ff(4,4,op);
#endif
#if four > 5
ff(4,5,op);
#endif
#if four > 6
ff(4,6,op);
#endif
#if four > 7
ff(4,7,op);
#endif
#if four > 8
ff(4,8,op);
#endif

#if end == 2
ff(2, (four)*2, op);
#endif
#if end == 3
ff(2, (four)*2, op);
#endif

/*
#if end == 2
ff(2, (four)*2, op);
#endif

#if end == 3
ff(2, (four)*2, op);
fl(op, size-1);
#endif
#if end == 1
fl(op, size-1);
#endif
*/
#undef four
#undef end
