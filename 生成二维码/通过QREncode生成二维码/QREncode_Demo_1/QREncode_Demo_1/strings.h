
    
    
  #ifndef   _STRINGS_H
  #define   _STRINGS_H   
    
  #ifndef   _H_STANDARDS   
  #include   <standards.h>   
  #endif   
    
  #include   <sys/types.h>   
    
  #ifdef   __cplusplus   
  extern   "C"   {   
  #endif   
    
  #if   _XOPEN_SOURCE_EXTENDED==1   
    
  #ifdef   _NO_PROTO   
    
  extern   int   bcmp();   
  extern   void   bcopy();   
  extern   void   bzero();   
  extern   int     ffs();   
  extern   char   *index();   
  extern   char   *rindex();   
    
  #ifndef   _STRCASECMP_DEF   
  #define   _STRCASECMP_DEF   
  extern   int     strcasecmp();   
  extern   int     strncasecmp();   
  #endif   /*   _STRCASECMP_DEF   */   
    
  #else   /*   _NO_PROTO   */   
    
  extern   int   bcmp(const   void   *,   const   void   *,   size_t);   
  extern   void   bcopy(const   void   *,   void   *,   size_t);   
  extern   void   bzero(void   *,   size_t);   
  extern   int     ffs(int);   
  extern   char   *index(const   char   *,   int);   
  extern   char   *rindex(const   char   *,   int);   
    
  #ifndef   _STRCASECMP_DEF   
  #define   _STRCASECMP_DEF   
  extern   int     strcasecmp(const   char   *,   const   char   *);   
  extern   int     strncasecmp(const   char   *,   const   char   *,   size_t);   
  #endif   /*   _STRCASECMP_DEF   */   
    
  #endif   /*   _NO_PROTO   */   
    
  #endif   /*   _XOPEN_SOURCE_EXTENDED   */   
    
  #ifdef   _ALL_SOURCE   
  #include   <string.h>   
  #endif   /*   _ALL_SOURCE   */   
    
  #ifdef   __cplusplus   
  }   
  #endif   
    
  #endif   /*   _STRINGS_H   */
