/*
 * vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4 foldmethod=marker:
 *
 * macros.h
 *
 * C.C.Allison
 * chris.allison@bgch.co.uk
 *
 * Started: Sunday 23 December 2012, 19:43:14
 * Last Modified: Tuesday  1 September 2015, 10:30:10
 */

extern int llevel;

#define CCA_ERR_CONT(...) syslog(LOG_ERR,__VA_ARGS__)
#define CCA_ERR_EXIT(ccaex,...) if(errno){ CCA_ERR_CONT("error: %d: %s",errno,strerror(errno));} CCA_ERR_CONT(__VA_ARGS__);exit(ccaex);
#define CCAC(...) CCA_ERR_CONT(__VA_ARGS__)
#define CCAE(ccaex,...) CCA_ERR_EXIT(ccaex,__VA_ARGS__)
#define DEBUG(...) if(llevel>=7){ syslog(LOG_DEBUG,__VA_ARGS__);}
#define INFO(...) if(llevel>=6){ syslog(LOG_INFO,__VA_ARGS__);}
#define NOTICE(...) if(llevel>=5){ syslog(LOG_NOTICE,__VA_ARGS__);}
#define WARN(...) if(llevel>=4){ syslog(LOG_WARNING,__VA_ARGS__);}
#define ERROR(...) if(llevel>=3){ syslog(LOG_ERR,__VA_ARGS__);}

#define NDEBUG

#ifndef NDEBUG
#define WHERESTR  "[file %s, line %d]: "
#define WHEREARG  __FILE__, __LINE__
#define DEBUGPRINT2(...)       fprintf(stderr, __VA_ARGS__);syslog(LOG_DEBUG,__VA_ARGS__)
#define DEBUGPRINT3(...)       syslog(LOG_DEBUG,__VA_ARGS__)
#define DEBUGPRINT(_fmt, ...)  DEBUGPRINT3(WHERESTR _fmt, WHEREARG, __VA_ARGS__)
/* #define DBG(_fmt, ...) DEBUGPRINT(_fmt, __VA_ARGS__) */
#define DBGL(_fmt, ...) if(llevel>=7){ DEBUGPRINT2(WHERESTR _fmt"\n", WHEREARG, __VA_ARGS__); }
#define DBG(...) if(llevel>=7){ DEBUGPRINT2(WHERESTR" %s\n",WHEREARG,__VA_ARGS__); }
#else
#define DEBUGPRINT(_fmt, ...) {}
#define DBG(_fmt, ...) {}
#define DBGL(_fmt,...) {}
#endif /* !NDEBUG */
