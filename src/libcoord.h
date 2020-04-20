/*
 * Library for creating modules.
 * Defines: 
 *      1. Modules' information; 
 *      2. Communication between modules;
 */
#ifndef LIBCOORD_H
    #define LIBCOORD_H
        /* defines */
        // Cooordinator's name
        #define COORD     "coordinator" /* Coordinator's name */

        // Handler's defines
        #define CONTHANDL 0             /* continue handling messages */
        #define STOPHANDL 1             /* stop handling messages */ 

        // Messages for communicating with Coordinator
        #define INITMSG         "IAM"   /* "Hello" msg */
        #define ALREADYEXISTMSG "IKNOW" /* Decline new module */
        #define ACEXISTMSG      "LOL"   /* Accept new module */
        #define REQCONMSG       "KUKU"  /* Request connection */
        #define ACCONMSG        "YOPTA" /* Accept connection's request */
        #define DECCONMSG       "NOPE"  /* Decline connection's request */

        // Messages' sizes
        #define INITMSGSIZE         3
        #define ALREADYEXISTMSGSIZE 5
        #define ACEXISTMSGSIZE      3
        #define REQCONMSGSIZE       4
        #define ACCONMSGSIZE        5
        #define DECCONMSGSIZE       4

        /* typedefs */
        typedef unsigned int mod_id;
        typedef unsigned int msg_len;

        /* structs */
        // Message struct
        typedef struct Message {
            mod_id   sender;   /* sender's id */
            char     *msg_buf; /* actual message here */
            msg_len  length;   /* message's length */
        } Message;
        // Struct with module's info
        typedef struct Module {
            mod_id   id;         /* module's id */
            char     *name;      /* module's name */
            int      write_end;  /* module's write-end */
            int      read_end;   /* module's read-end */
        } Module;

        /* functions */
        Module*  get_module_by_name(char *name);                   /* Get modules' information by name */
        int      init_module(char *name, char *addr, char *port);  /* Initialize module */
        int      connect_to_module(char *mod);                     /* Connect to module */
        int      send_msg(Module *mod, char *buf, msg_len len);    /* Send message to module  */
        int      handle(int (*handler)(Message *));                /* Handle incoming messages */
        Message* get_msg();                                        /* Get any incoming message */
#endif
