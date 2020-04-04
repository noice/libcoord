/*
 * Library for creating modules.
 * Defines: 
 *      1. Modules' information; 
 *      2. Communication between modules;
 */
#ifndef LIBCOORD_H
    #define LIBCOORD_H
        /* typedefs */
        typedef unsigned int mod_id;
        typedef unsigned int msg_t;
        /* structs */
        // Message struct
        typedef struct Message {
            mod_id sender;  /* sender's id */
            char * msg_buf; /* actual message here */
            msg_t  length;  /* message's length */
        } Message;
        // Struct with module's info
        typedef struct Module {
            mod_id id;        /* module's id*/
            char * name;      /* module's name */
            int    write_end; /* module's write-end */
            int    read_end;  /* module's read-end */
        } Module;
        /* functions */
        Module * get_module_by_name(char * name);              /* Get modules' information */
        int      init_module();                                /* Initialize module */
        int      connect_to_module(mod_id id);                 /* Connect to module with id */
        int      send_msg(mod_id mod, char * buf, msg_t len);  /* Send message to module  */
        int      handle_msgs(int (*handler)(Message *));       /* Handle all incoming messages */
        Message* get_msg();                                    /* Get any incoming message */
#endif
