#include "tx_api.h"
#include <stdio.h>
#define STACK_SIZE 1024
#define BYTE_POOL_SIZE 9120
#define NUMBER_OF_MESSAGES 100
#define MESSAGE_SIZE TX_1_ULONG
#define QUEUE_SIZE MESSAGE_SIZE * sizeof(ULONG) * NUMBER_OF_MESSAGES

/* intervals at which timers send messages to two queues */
#define Queue_1_SEND_INTERVAL 12
#define Queue_2_SEND_INTERVAL 9

/* Urgent_thread and Routine_thread entry function prototypes */
extern void Urgent_thread_entry(ULONG thread_input);
extern void Routine_thread_entry(ULONG thread_input);


/* Define the ThreadX object control blocks */
extern TX_THREAD Urgent_thread;  /* higher priority thread */
extern TX_THREAD Routine_thread; /* lower priority thread */

extern TX_BYTE_POOL my_byte_pool; /* byte pool for stacks and queues */
extern TX_SEMAPHORE Gatekeeper;   /* indicate how many messages available */

extern TX_QUEUE Queue_1; /* queue for multiple object suspension */
extern TX_QUEUE Queue_2; /* queue for multiple object suspension */

extern TX_TIMER stats_timer;   /* generate statistics at periodic intervals */
extern TX_TIMER Queue_1_timer; /* send message to Queue_1 at intervals */
extern TX_TIMER Queue_2_timer; /* send message to Queue_2 at intervals */

/* Variables needed to get info about the message queue contents */
extern ULONG Enqueued_1;
extern ULONG Enqueued_2;
/* Define the variables used in the project application  */
extern ULONG Urgent_thread_counter;
extern ULONG total_Urgent_time;
extern ULONG Routine_thread_counter;
extern ULONG total_Routine_time;
extern ULONG send_message_1[TX_1_ULONG];
extern ULONG send_message_2[TX_1_ULONG];
extern ULONG receive_message_1[TX_1_ULONG];
extern ULONG receive_message_2[TX_1_ULONG];

/* timer entry function prototypes */
extern void Queue_1_timer_entry(ULONG thread_input);
extern void Queue_2_timer_entry(ULONG thread_input);
extern void print_stats(ULONG);

/* event notification function prototypes */
extern void Queue_1_send_notify(TX_QUEUE *Queue_1_ptr);
extern void Queue_2_send_notify(TX_QUEUE *Queue_2_ptr);