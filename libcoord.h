/*
 * Library for creating modules.
 * Defines: 
 *      1. Modules' information; 
 *      2. Communication between modules;
 */
#ifndef LIBCOORD_H
    #define LIBCOORD_H
        /* defines */
        #define continue_handling 0 /* continue handling messages */
        #define exit_handling     1 /* stop handling messages */ 

        /* typedefs */
        typedef unsigned int mod_id;
        typedef unsigned int msg_lent;

        /* structs */
        // Message struct
        typedef struct Message {
            mod_id    sender;  /* sender's id */
            char*     msg_buf; /* actual message here */
            msg_lent  length;  /* message's length */
        } Message;
        // Struct with module's info
        typedef struct Module {
            mod_id id;        /* module's id*/
            char*  name;      /* module's name */
            int    write_end; /* module's write-end */
            int    read_end;  /* module's read-end */
        } Module;

        /* functions */
        Module*  get_module_by_name(char* name);                   /* Get modules' information by name */
        Module*  get_module_by_id(mod_id id);                      /* Get modules' information by id */
        int      init_module();                                    /* Initialize module */
        int      connect_to_module(Module * mod);                  /* Connect to module */
        int      send_msg(Module * mod, char * buf, msg_lent len); /* Send message to module  */
        int      handle(int (*handler)(Message *));                /* Handle all incoming messages */
        Message* get_msg();                                        /* Get any incoming message */
#endif
