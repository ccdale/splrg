/*
 * vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4 foldmethod=marker:
 *
 * splrg.c
 *
 * C.C.Allison
 * chris.allison@bgch.co.uk
 *
 * Started: Friday 28 August 2015, 14:39:24
 * Last Modified: Sunday 30 August 2015, 22:07:41
 *
 */

#include "splrg.h"

int setuphttpserver(void)/* {{{ */
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        CCAE(1,"Error opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno=atoi(configValue("portno"));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        CCAE(1,"Cannot bind to socket");
    }
    return sockfd;
}/* }}} */
void httpserver(void)/* {{{ */
{
    int sockfd,newsockfd;
    socklen_t clilen;
    struct sockaddr_in cli_addr;

    sockfd=setuphttpserver();
    listen(sockfd,5);
    clilen=sizeof(cli_addr);
    while(1)
    {
        errno=0;
        /* this will block until a connection is made or we are yanked out by a signal */
        newsockfd=accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        if(newsockfd<0){
            if(errno==EINTR){
                /* call was interrupted by a signal, restart it if necessary or gracefully exit */
                if(timetodie==1){
                    DBG("Closing listening socket");
                    close(sockfd);
                    break;
                }
            }else{
                /* something else went wrong accessing the socket, time to exit fast */
                DBG("Closing listening socket");
                close(sockfd);
                CCAE(1,"Error accepting connection, exiting");
            }
        }else{
            /* every thing is fine, read the input */
            processinput(newsockfd);
        }
    }
}/* }}} */
void processinput(int isockfd)/* {{{ */
{
    int buflen=4096;
    char *data=NULL;
    char *buffer;
    char header[]="\r\nCache-control: no-cache\r\nConnection: close\r\n\r\n";
    /*
    char rok[]="HTTP/1.1 200\r\nCache-control: no-cache\r\nConnection: close\r\n\r\nOK\r\n";
    char rnok[]="HTTP/1.1 404\r\nCache-control: no-cache\r\nConnection: close\r\n\r\n\r\n";
    */
    char *response=NULL;
    int len;
    int n;

    buffer=xcalloc(buflen,sizeof(char));
    n=read(isockfd,buffer,buflen-1);
    if(n<0){
        CCAC("Error reading from socket, ignoring");
    }
    DBGL("msg rcvd: %s",buffer);
    n=parseinput(buffer,&data);
    if(n==0){
        len=snprintf(response,0,"HTTP/1.1 %d%s%s\r\n",200,header,"");
        response=xmalloc(++len);
        len=snprintf(response,len,"HTTP/1.1 %d%s%s\r\n",200,header,"");
    }else if(n==-1){
        /* data needs to be sent back, data is now in *data  and space has been allocated by malloc */
        len=snprintf(response,0,"HTTP/1.1 %d%s%s\r\n",200,header,data);
        response=xmalloc(++len);
        len=snprintf(response,len,"HTTP/1.1 %d%s%s\r\n",200,header,data);
        free(data);
    }else{
        len=snprintf(response,0,"HTTP/1.1 %d%s%s\r\n",404,header,"");
        response=xmalloc(++len);
        len=snprintf(response,len,"HTTP/1.1 %d%s%s\r\n",404,header,"");
    }
    DBGL("sending: %s",response);
    n=write(isockfd,response,strlen(response));
    if(n<0){
        CCAC("Error writing to socket, ignoring");
    }
    DBG("Closing process socket");
    close(isockfd);
    DBG("Freeing intermediate buffers");
    free(response);
    free(buffer);
}/* }}} */
int parseinput(char *buf,char **data)/* {{{ */
{
    char space[]=" ";
    char *cmd;
    int ret=1;
    int len;

    /* buffer: GET /runpuppet ... */
    cmd=strtok(buf,space);
    cmd=strtok(NULL,space);
    if((strcmp(cmd,"/runpuppet")==0)){
        DBG("Code to run puppet goes here");
        runpuppet();
        ret=0;
    }else if((strcmp(cmd,"/status")==0)){
        len=snprintf(*data,0,"%d",puppetrunning);
        *data=xmalloc(++len);
        len=snprintf(*data,len,"%d",puppetrunning);
        ret=-1;
    }else if((strcmp(cmd,"/lastlog")==0)){
        FILE *f=fopen(configValue("puppetlog"),"rb");
        fseek(f,0,SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);
        *data=xmalloc(fsize+1);
        fread(*data,fsize,1,f);
        fclose(f);
        ret=-1;
    }
    return ret;
}/* }}} */
void runpuppet(void)/* {{{ */
{
    int cpid;
    /*
    char *args=NULL;
    char *arg2=NULL;
    char sudo[]="/usr/bin/sudo";
    */
    char *pbin=NULL;
    char *shell=NULL;
    int len;
    int kstatus;

    cpid=fork();
    if(cpid<0){
        CCAE(1,"out of memory, cannot fork to run puppet");
    }else if(cpid>0){
        /* waitpid(-1, &kstatus, WNOHANG); */
        puppetrunning++;
    }else{
        len=snprintf(pbin,0,"sudo %s >%s 2>&1",configValue("puppetbin"),configValue("puppetlog"));
        pbin=xmalloc(len++);
        len=snprintf(pbin,len,"sudo %s >%s 2>&1",configValue("puppetbin"),configValue("puppetlog"));

        shell=getenv("SHELL");
        DEBUG("running puppet: %s -c %s",shell,pbin);
        errno=0;
        execl(shell,shell,"-c",pbin,(char *)NULL);
        /* we should never get as far as this */
        if(errno){
            ERROR("exec error: %d",errno);
            ERROR(strerror(errno));
        }
    }
    DBG("runpuppet exiting");
}/* }}} */
void catchsignal(int sig)/* {{{1 */
{
    DBG("in sig handler");
    switch(sig){
        case SIGCHLD:
            DBG("SIGCHLD signal caught");
            puppetrunning--;
            break;
        case SIGTERM:
            DBG("SIGTERM signal caught");
            timetodie=1;
            break;
    }
} /* }}} */
char *argprocessing(int argc,char **argv)/* {{{ */
{
    char *conffile=NULL;
    /* Define the allowable command line options, collecting them in argtable[] */
    struct arg_file *conf = arg_file0("c","conf-file",PROGCONF,"configuration file");
    struct arg_lit *help = arg_lit0("h","help","print this help and exit");
    struct arg_int *loglevel = arg_int0("l","log-level","<n>","7=LOG-DEBUG .. 0=LOG_EMERG - default: 5 (LOG_NOTICE)");
    struct arg_lit *vers = arg_lit0("v","version","print version information and exit");
    struct arg_end *end  = arg_end(20);

    void *argtable[] = {conf,help,loglevel,vers,end};

    int nerrors;

    /* check the argtable[] entries were correctly allocated */
    if(arg_nullcheck(argtable) == 0){
        /* set default config filename */
        conf->filename[0]=PROGCONF;
        /* parse the command line */
        nerrors = arg_parse(argc,argv,argtable);

        /* check for help before error processing */
        if(help->count > 0){
            /* print the help/usage statement and exit */
            printf("Usage: %s",PROGNAME);
            arg_print_syntax(stdout,argtable,"\n");
            printf("%s is a daemon to listen for requests to run puppet on this host.\n\n",PROGNAME);
            arg_print_glossary(stdout,argtable,"  %-20s %s\n");
            /* free up memory used for argument processing */
            arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
            printf("\nreport bugs to %s\n",PROGEMAIL);
            return 0;
        }
        /* check for version request before error checking */
        if(vers->count > 0){
            /* free up memory used for argument processing */
            arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
            /* print the requested information and exit */
            printf("%s %s\n",PROGNAME,PROGVERSION);
            printf("%s\n%s",PROGCOPYRIGHT,PROGLICENSE);
            return 0;
        }
        /* check for errors in command line arguments */
        if(nerrors > 0){
            /* display the errors in the arg_end struct */
            arg_print_errors(stdout,end,PROGNAME);
            printf("Try '%s --help' for more information.\n",PROGNAME);
            /* free up memory used for argument processing */
            arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
            CCAE(1,"Failed to process command line correctly");
        }
        if(conf->count > 0){
            conffile=strdup(conf->filename[0]);
        }
        if(loglevel->count > 0){
            llevel=loglevel->ival[0];
        }
    }
    /* free up memory used for argument processing */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
    /* command line processing completed */
    return conffile;
}/* }}} */
void setDefaultConfig(void)/*{{{*/
{
    char *tk;
    char *tv;

    tk=strdup("configfile");
    tv=strdup(CCA_DEFAULT_CONFIGFILE);
    updateConfig(tk,tv);
    tk=strdup("puppetbin");
    tv=strdup(CCA_DEFAULT_PUPPETBIN);
    updateConfig(tk,tv);
    tk=strdup("puppetlog");
    tv=strdup(CCA_DEFAULT_PUPPETLOG);
    updateConfig(tk,tv);
    tk=strdup("username");
    tv=strdup(CCA_DEFAULT_USERNAME);
    updateConfig(tk,tv);
    tk=strdup("groupname");
    tv=strdup(CCA_DEFAULT_GROUPNAME);
    updateConfig(tk,tv);
    tk=strdup("portno");
    tv=strdup(CCA_DEFAULT_PORTNO);
    updateConfig(tk,tv);
}/* }}} */
void daemonize()/* {{{1 */
{
    int i,lfp;
    char str[MAX_MSG_LEN];
    int junk;
    struct passwd *pwd;
    char *username;

    if(getppid()==1) return; /* already a daemon */
    DBG("Forking");
    i=fork();
    if (i<0) 
    {
        CCAE(1,"fork failed. Exiting");
    }
    if (i>0) 
    {
        DBG("parent exiting after fork");
        exit(0);
    } /* parent exits */
    /* child (daemon) continues */
    DBG("fork success. Child process continues.");
    setsid(); /* obtain a new process group */
    DBG("closing file descriptors");
    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    DBG("redirecting standard i/o to /dev/null");
    i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standard I/O */
    DBG("setting umask to 027");
    umask(027); /* set newly created file permissions */

    DBG("Dropping priviledges");
    username=configValue("username");
    if((pwd=getpwnam(username))==NULL){
        CCAE(1,"user not found, will not run as root, exiting");
    }
    errno=0;
    junk=setuid(pwd->pw_uid);
    if(errno){
        CCAE(1,"failed to drop priviledges, will not run as root, exiting");
    }
    errno=0;
    junk=setgid(pwd->pw_gid);
    if(errno){
        CCAE(1,"failed to drop group priviledges, will not run as root, exiting");
    }

    DBGL("cd'ing to %s",pwd->pw_dir);
    chdir(pwd->pw_dir); /* change running directory */
    DBG("Creating lock file");
    lfp=open(CCA_LOCK_FILE,O_RDWR|O_CREAT,0640);
    if (lfp<0) {
        CCAE(1,"Failed to create lock file, exiting.");
    } /* can not open */
    if (lockf(lfp,F_TLOCK,0)<0) {
        CCAE(1,"Cannot lock, another instance running? qitting");
    } /* can not lock */
    DBG("File locked.");
    /* first instance continues */
    sprintf(str,"%d",getpid());
    write(lfp,str,strlen(str)); /* record pid to lockfile */
    DBG("pid written to lock file");
    DBG("setting signal handlers");
    /*
    siga->sa_handler=SIG_IGN;
    if((junk=sigaction(SIGCHLD,siga,NULL))!=0){
        CCAE(1,"cannot ignore signal SIGCHLD");
    }
    */
    siga=xmalloc(sizeof(struct sigaction));
    siga->sa_flags=0;

    /* ignored signals */
    siga->sa_handler=SIG_IGN;
    if((junk=sigaction(SIGTSTP,siga,NULL))!=0){
        CCAE(1,"cannot ignore signal SIGTSTP");
    }
    siga->sa_handler=SIG_IGN;
    if((junk=sigaction(SIGTTOU,siga,NULL))!=0){
        CCAE(1,"cannot ignore signal SIGTTOU");
    }
    siga->sa_handler=SIG_IGN;
    if((junk=sigaction(SIGTTIN,siga,NULL))!=0){
        CCAE(1,"cannot ignore signal SIGTTIN");
    }
    siga->sa_handler=SIG_IGN;
    if((junk=sigaction(SIGHUP,siga,NULL))!=0){
        CCAE(1,"cannot set handler for SIGHUP");
    }
    siga->sa_handler=SIG_IGN;
    if((junk=sigaction(SIGINT,siga,NULL))!=0){
        CCAE(1,"cannot set handler for SIGINT");
    }
    siga->sa_handler=SIG_IGN;
    if((junk=sigaction(SIGUSR1,siga,NULL))!=0){
        CCAE(1,"cannot set handler for SIGUSR1");
    }
    /* setting this explicitly to ignore will automatically reap the zombie child processes when they terminate */
    /* siga->sa_handler=SIG_IGN; */
    siga->sa_handler=catchsignal;
    if((junk=sigaction(SIGCHLD,siga,NULL))!=0){
        CCAE(1,"cannot set handler for SIGCHLD");
    }
    /* interesting (caught) signals */
    siga->sa_handler=catchsignal;
    if((junk=sigaction(SIGTERM,siga,NULL))!=0){
        CCAE(1,"cannot set handler for SIGTERM");
    }
    DBG(PROGNAME" daemonized");
}/* }}} */
int main(int argc,char **argv)/* {{{ */
{
    int ret=0;
    char *conffile=NULL;

    conffile=argprocessing(argc,argv);

    NOTICE(PROGNAME" starting");

    if((ret=initConfig())==0){
        DBG("setting default config");
        setDefaultConfig();
        if(conffile){
            updateConfig("configfile",conffile);
        }
        DBGL("Getting config from %s",configValue("configfile"));
        getConfigFromFile( configValue("configfile") );
        daemonize();
    }else{
        CCAE(1,"Cannot initialise configuration.");
    }
    httpserver();
    INFO(PROGNAME" closing");
    DBG("freeing config");
    deleteConfig();
    DBG("freeing signal handlers");
    free(siga);
    DBG("deleting lock file");
    unlink(CCA_LOCK_FILE);
    NOTICE(PROGNAME" stopped");
    return ret;
}/*}}}*/
