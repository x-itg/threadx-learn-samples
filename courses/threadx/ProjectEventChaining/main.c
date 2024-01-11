/* ProjectEventChaining.c

   Create two threads, one byte pool, two message queues, three timers, and
   one counting semaphore. This is an example of multiple object suspension
   using event-chaining, i.e., Urgent thread and Routine thread wait for a message
   to appear on either of two queues.

   Your task is to modify this source code to implement multiple object suspension
   on three queues. Among other items, you will need another queue, timer,
   notification function, and message send function, as well as various variables,
   defines, and creates. */

/****************************************************/
/*    Declarations, Definitions, and Prototypes     */
/****************************************************/
#include "user.h"

TX_THREAD Urgent_thread;  /* higher priority thread */
TX_THREAD Routine_thread; /* lower priority thread */

TX_BYTE_POOL my_byte_pool; /* byte pool for stacks and queues */
TX_SEMAPHORE Gatekeeper;   /* indicate how many messages available */

TX_QUEUE Queue_1; /* queue for multiple object suspension */
TX_QUEUE Queue_2; /* queue for multiple object suspension */

TX_TIMER stats_timer;   /* generate statistics at periodic intervals */
TX_TIMER Queue_1_timer; /* send message to Queue_1 at intervals */
TX_TIMER Queue_2_timer; /* send message to Queue_2 at intervals */

/* Variables needed to get info about the message queue contents */
ULONG Enqueued_1 = 0, Enqueued_2 = 0;

/* Define the variables used in the project application  */
ULONG Urgent_thread_counter = 0, total_Urgent_time = 0;
ULONG Routine_thread_counter = 0, total_Routine_time = 0;
ULONG send_message_1[TX_1_ULONG] = {0X0},
      send_message_2[TX_1_ULONG] = {0X0};
ULONG receive_message_1[TX_1_ULONG],
    receive_message_2[TX_1_ULONG];

/* timer entry function prototypes */
void Queue_1_timer_entry(ULONG thread_input);
void Queue_2_timer_entry(ULONG thread_input);
void print_stats(ULONG);

/* event notification function prototypes */
void Queue_1_send_notify(TX_QUEUE *Queue_1_ptr);
void Queue_2_send_notify(TX_QUEUE *Queue_2_ptr);

/****************************************************/
/*               Main Entry Point                   */
/****************************************************/

/* Define main entry point.  */
int main()
{
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}

/****************************************************/
/*             Application Definitions              */
/****************************************************/
/* Define what the initial system looks like.
   Create threads, queues, the semaphore, timers,
   and register functions for event-chaining */

void tx_application_define(void *first_unused_memory)
{
    CHAR *Urgent_stack_ptr, *Routine_stack_ptr;
    CHAR *Queue_1_ptr, *Queue_2_ptr;

    /* Create a byte memory pool from which to allocate the thread stacks.  */
    tx_byte_pool_create(&my_byte_pool, "my_byte_pool",
                        first_unused_memory, BYTE_POOL_SIZE);

    /* Allocate the stack for Urgent_thread.  */
    tx_byte_allocate(&my_byte_pool, (VOID **)&Urgent_stack_ptr,
                     STACK_SIZE, TX_NO_WAIT);

    /* Create Urgent_thread.  */
    tx_thread_create(&Urgent_thread, "Urgent_thread", Urgent_thread_entry, 0,
                     Urgent_stack_ptr, STACK_SIZE, 5, 5, TX_NO_TIME_SLICE, TX_AUTO_START);

    /* Allocate the stack for Routine_thread.  */
    tx_byte_allocate(&my_byte_pool, (VOID **)&Routine_stack_ptr,
                     STACK_SIZE, TX_NO_WAIT);

    /* Create Routine_thread */
    tx_thread_create(&Routine_thread, "Routine_thread", Routine_thread_entry, 1,
                     Routine_stack_ptr, STACK_SIZE, 15, 15, TX_NO_TIME_SLICE, TX_AUTO_START);

    /* Create the message queues used by both threads.  */
    tx_byte_allocate(&my_byte_pool, (VOID **)&Queue_1_ptr,
                     QUEUE_SIZE, TX_NO_WAIT);

    tx_queue_create(&Queue_1, "Queue_1", MESSAGE_SIZE,
                    Queue_1_ptr, QUEUE_SIZE);

    tx_byte_allocate(&my_byte_pool, (VOID **)&Queue_2_ptr,
                     QUEUE_SIZE, TX_NO_WAIT);

    tx_queue_create(&Queue_2, "Queue_2", MESSAGE_SIZE,
                    Queue_2_ptr, QUEUE_SIZE);

    /* Create the Gatekeeper semaphore that counts the number of objects available  */
    tx_semaphore_create(&Gatekeeper, "Gatekeeper", 0); // 信号量初始化

    /* Create and activate the stats timer */
    tx_timer_create(&stats_timer, "stats_timer", print_stats,
                    0x1234, 500, 500, TX_AUTO_ACTIVATE);

    /* Create and activate the timer to send messages to Queue_1 */
    tx_timer_create(&Queue_1_timer, "Queue_1_timer", Queue_1_timer_entry,
                    0x1234, Queue_1_SEND_INTERVAL, Queue_1_SEND_INTERVAL,
                    TX_AUTO_ACTIVATE);

    /* Create and activate the timer to send messages to Queue_2 */
    tx_timer_create(&Queue_2_timer, "Queue_2_timer", Queue_2_timer_entry,
                    0x1234, Queue_2_SEND_INTERVAL, Queue_2_SEND_INTERVAL,
                    TX_AUTO_ACTIVATE);

    /* Register the function to increment the Gatekeeper semaphore when a
       message is sent to Queue_1 */
    tx_queue_send_notify(&Queue_1, Queue_1_send_notify);

    /* Register the function to increment the Gatekeeper semaphore when a
       message is sent to Queue_2 */
    tx_queue_send_notify(&Queue_2, Queue_2_send_notify);
}

/*****************************************************/
/* print statistics at specified times */
void print_stats(ULONG invalue)
{
    ULONG current_time, avg_Routine_time, avg_Urgent_time;

    current_time = tx_time_get();
    avg_Routine_time = total_Routine_time / Routine_thread_counter;
    avg_Urgent_time = total_Urgent_time / Urgent_thread_counter;

    tx_queue_info_get(&Queue_1, TX_NULL, &Enqueued_1,
                      TX_NULL, TX_NULL, TX_NULL, TX_NULL);

    tx_queue_info_get(&Queue_2, TX_NULL, &Enqueued_2,
                      TX_NULL, TX_NULL, TX_NULL, TX_NULL);

    printf("\nProjectEventChaining:\n");
    printf("   2 threads, 1 byte pool, 2 queues, 1 semaphore, & 3 timers\n\n");
    printf("  Current Time:                     %lu\n", current_time);
    printf("           Urgent_thread counter:   %lu\n", Urgent_thread_counter);
    printf("          Urgent_thread avg time:   %lu\n", avg_Urgent_time);
    printf("          Routine_thread counter:   %lu\n", Routine_thread_counter);
    printf("         Routine_thread avg time:   %lu\n\n", avg_Routine_time);

    printf(" total # messages sent to Queue_1:  %lu\n", send_message_1[TX_1_ULONG - 1]);
    printf(" total # messages sent to Queue_2:  %lu\n", send_message_2[TX_1_ULONG - 1]);
    printf("    current # messages in Queue_1:  %lu\n", Enqueued_1);
    printf("    current # messages in Queue_2:  %lu\n\n", Enqueued_2);
}
