// ----------------------------------- Headers ----------------------------------
#include "libcoord.h"   /* Library declaration */
#include "hash_table.h" /* Hash table */

#include <sys/types.h>    /* Special types */
#include <sys/socket.h>   /* For connection */
#include <netdb.h>        /* Network address and service translation */
#include <string.h>       /* Memset */
#include <stdlib.h>       /* Standard funcs */
#include <stdio.h>        /* Formatted output */
#include <unistd.h>       /* Syscalls */
#include <signal.h>       /* Signal handling */
#include <event2/event.h> /* libevent */

// ----------------------------------- Defines ----------------------------------
// Initial Hash table
#define INITHASHTBLSIZE 5

// ---------------------------------- Prototypes --------------------------------
// Additional funcs
Module* new_module(mod_id, name_len, char *, int, int);
int     connect_to_coordinator();
int     init_modules_storage();
void    get_fds(char *, msg_len, char *, char *);
void    set_exit_handler();
void    exit_handler(int);
void    shutdown_module();

// --------------------------------- Global vars --------------------------------
Hash_table *ht = NULL;        /* Hash table of Module */
struct event_base* eb = NULL; /* Event base */

// ---------------------------------- Main funcs --------------------------------
/*
 * Initialize new module
 * Notify Coordinator about self-existence
 * Initialize modules' data storage
 * Create event base
 */
int
init_module(char *name, char *addr, char *port) {
    if (set_exit_handler() == -1) {
       fprintf(stderr, "Error while setting exit handler\n"); 
       return -1;
    }
    
    if (connect_to_coordinator(name, addr, port)) {
       fprintf(stderr, "Error while connecting to Coordinator\n"); 
       return -1;
    }

    init_modules_storage();

    // Initialise Libevent
    // Create event base
    *eb = event_base_new();
    if (!eb) {
       fprintf(stderr, "Error while creating event base\n"); 
       return -1;
    }

    return 0;
}

/*
 * Get module info by provided @name
 * Otherwise returns NULL
 */
Module* 
get_module_by_name(char *name) {
    // Search Module in Hash table
    // Return if exist
    // NULL, otherwise
    return (Module *) search_item(ht, name);
}

/*
 * Connect to module provided @mod
 * Send request for connection to Coordinator
 * If connection can be established, update requested module's information in data storage and returns 0 (success) 
 * If no, returns -1 (failure)
 */
int
connect_to_module(char *name) {
    // Connection request
    // 7  = REQCONMSGSIZE + 1 (' ') + 1 ('\n') + 1 ('\0')
    // 29 = ACCONMSGSIZE + 10 (id) + 10 (id) + 2 (' ') + 1 ('\n') + 1 ('\0')
    char *request = (char *) malloc(sizeof(char)*(7 + strlen(name)));      
    char *response[29];

    // Construct request
    sprintf(request, "%s %s\n", REQCONMSG, name);

    // Get Coordinator's module
    Module *coord = get_module_by_name(COORD);
    
    // Send connection request to Coordinator
    if (send_msg(coord, request, strlen(request)) == -1) {
        fprintf(stderr, "Error while sending msg to %s\n", name);
        return -1;
    }
    
    // Read response
    read(coord->read_end, response, 29);

    // If ACCONMSG, then get fds
    if (strncmp(response, ACCONMSG, ACCONMSGSIZE) == 0) {
        char write_end[10]; /* store write-end to Coordinator here */
        char read_end[10];  /* store read-end to Coordinator here */
        
        get_fds(response, ACCONMSGSIZE, write_end, read_end); 

        // Create new module with given fds
        Module *req_module = new_module(name, -1, atoi(write_end), atoi(read_end));
        // Then add it to Modules' storage
        insert_item(ht, req_module->name, req_module);

        // Check if is ready to read
        event *ev = event_new(eb, req_module->read_end, EV_READ | EV_PERSIST);

        // Check if is ready to write
        event *ev = event_new(eb, req_module->write_end, EV_WRITE | EV_PERSIST);

        return 0;
    }
    // If declined connection 
    else if (strncmp(ans, DECCONMSG, DECCONMSGSIZE) == 0) {
        fprintf(stderr, "Connection was declined\n"); 

        return -1;
    }
    // Other messages also is error
    // Return -1
    else {
        return -1;
    }
}

/*
 * Send msg to module by provided @mod
 * Message represented by @buf (actual message here) and @len (length of message)
 */
int
send_msg(Module *mod, char *buf, msg_len len) {
    // If Module is NULL, return error
    if (mod == NULL) {
        fprintf(stderr, "Module doesn't exist\n");
        return -1;
    }
    // Send msg
    if (send(mod->write_end, buf, len, 0) == -1) {
        perror("Error while sending msg to Module");
        return -1;
    }

    return 0;
}

/*
 * Handle incoming messages
 * First use default defined handler 
 * Then use user-provded @handler
 */
int
handle(int (*handler)(Message*)) {
    return 0;
}

/*
 * Get any incoming message
 * Check if any read-ends contains message and can be readed
 * If yes, return pointer to first founded Message
 * Otherwise returns NULL
 */
Message*
get_msg() {
    return NULL;
}
// ------------------------------- Additional funcs -----------------------------
/*
 * Return new Module
 */
Module*
new_module(mod_id id, char *name, int write_end, int read_end) {
    Module *mod = (Module *) malloc(sizeof(Module));

    mod->id = id;
    mod->name = name;
    mod->write_end = write_end;
    mod->read_end = read_end;

    return mod;
}

/*
 * Send self module's name to coordinator (by default address)
 * Wait for connection to coordinator
 * If connection established, return 0 (success)
 * If no, return -1 (failure)
 */
int
connect_to_coordinator(char *name, char *addr, char *port) {
    struct addrinfo hints;   /* connection information */
    struct addrinfo *result; /* possible addresses' info for connection */
    struct addrinfo *rp;     /* current possible address'es info */

    int status; /* status code of getaddrinfo */
    int sfd;    /* socket's fd */

    memset(&hints, 0, sizeof(struct addrinfo)); /* empty structure */

    hints.ai_family   = AF_UNSPEC;   /* IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP segment */ 
    hints.ai_protocol = 0;           /* Any protocol (TCP)*/

    // Get possible adresses for connection
    status = getaddrinfo(name, port, &hints, &result);
    if (status != 0) {
       fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
       return -1;
    }

    // Try each possible address until connection is succesful
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        // Open socket with current possible address'es info
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        // If failed open socket, then use next possible address'es
        if (sfd == -1) {
            continue;
        }

        // Try to connect
        // If success, break cycle
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
           break; 
        }
        // If not, close socket's fd

        close(sfd);
    }

    // If no address succeeded, print error message
    if (rp == NULL) {
       fprintf(stderr, "Could not connect\n");
       return -1;
    }

    freeaddrinfo(result);

    // Send to Coordinator "INITMSG (module's name)\n"
    // (msg) 260 = INITMSGSIZE + 1 (' ') + 1 ('\n') + 255 (module's name)
    // (ans) 27 = ACCEXISTMSGSIZE + 10 (write_end) + 10 (read_end) + 2 (' ') + 1 ('\n') + ('\0')
    char msg[260]; /* message to Coordinator */
    char ans[27];  /* response */
    
    sprintf(msg, "%s (%s)\n", INITMSG, name);
    if (send(sfd, msg, strlen(msg), 0) == -1) {
        perror("Error while sending INITMSG to Coordinator");
        close(sfd);
        return -1;
    }
    
    // Read answer
    read(sfd, ans, 27); 

    // TODO: Perhaps in future this will be in separate function
    // If LOL, then everything is alright
    if (strncmp(ans, ACCEXISTMSG, ACCEXISTMSGSIZE) == 0) {
        char write_end[10]; /* store write-end to Coordinator here */
        char read_end[10];  /* store read-end to Coordinator here */

        // Get fds from answer
        get_fds(ans, ACCEXISTMSGSIZE, write_end, read_end);
        
        // Create new module with given fds
        Module *coord = new_module(COORD, 0, atoi(write_end), atoi(read_end));

        // Then add it to Modules' storage
        insert_item(ht, coord->name, coord);

        close(sfd);
        return 0;
    }
    // It means module with name already exist
    // Report about it and return error
    else if (strncmp(ans, ALREADYEXISTMSG, ALREADYEXISTMSGSIZE) == 0) {
        fprintf(stderr, "Module with that name already exist\n"); 
        close(sfd);

        return -1;
    }
    // Other messages also is error
    // Return -1
    else {
        close(sfd);

        return -1;
    }
}

/*
 * Initialize modules' storage
 */
int
init_modules_storage() {
    // Initialize new Hash table with size defined by INITHASHTBLSIZE
    ht = new_hash_table(INITHASHTBLSIZE); 
    return 0;
}

/*
 * Get fds from answer
 */
void
get_fds(char *msg, msg_len msg_term, char *write_end, char *read_end) {
    msg_len i, j = 0;

    // Checking what value is reading:
    // true  - write_end
    // false - read_end
    bool is_reading_write = true;

    for (i = msg_term; ans[i] != '\n'; i++) {
        // If space is encountered then reading write-end value is done
        // Start reading read-end (ahaha) value
        if (ans[i] == ' ') {
            is_reading_write = false;
            j = 0;
        }

        // Read write-end
        if (is_reading_write) {
            write_end[j++] = ans[i];                   
        } 
        // Read read-end
        else if (!is_reading_write) {
            read_end[j++] = ans[i];
        }
    }
}

/*
 * Set exit handler
 */
int
set_exit_handler() {
    // Here will be all needed info for sighandling
    struct sigaction act; 

    act.sa_handler = exit_handler; /* set handler */
    sigemptyset(&act.sa_mask);     /* empty mask */
    act.sa_flags = 0;              /* no flags are needed */

    // Set actiong for SIGTERM
    // If error, return -1
    if (sigaction(SIGTERM, &act, NULL) == -1) {
        perror("Error while setting actiong for SIGTERM");
        return -1;
    }

    return 0;
}

/*
 * Exit handler
 */
void
exit_handler(int signum) {
    // If we get SIGTERM, then shutdown module
    if (signum == SIGTERM) {
        shutdown_module();
    }
}

/*
 * Shutdown module
 * Notify Coordinator about termination
 * Close active connections
 * Clear modules' data storage
 * Clear event_base struct
 */
void
shutdown_module() {
}
