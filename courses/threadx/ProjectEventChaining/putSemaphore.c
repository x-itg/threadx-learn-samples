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

/*****************************************************/
/* Send a message to Queue_1 at specified times */
void Queue_1_timer_entry(ULONG invalue)
{
    /* Send a message to Queue_1 using the multiple object suspension approach,
       The Gatekeeper semaphore keeps track of how many messages are available
       via the notification function */
    send_message_1[TX_1_ULONG - 1]++;
    tx_queue_send(&Queue_1, send_message_1, TX_NO_WAIT);
}

/*****************************************************/
/* Send a message to the queue at specified times */
void Queue_2_timer_entry(ULONG invalue)
{
    /* Send a message to Queue_2 using the multiple object suspension approach
       The Gatekeeper semaphore keeps track of how many messages are available
       via the notification function */
    send_message_2[TX_1_ULONG - 1]++;
    tx_queue_send(&Queue_2, send_message_2, TX_NO_WAIT);
}
/*****************************************************/
/* Notification function to increment Gatekeeper semaphore whenever a message has been sent to Queue_1 */
// 通知函数，每当向Queue_1发送消息时，将Gatekeeper信号量递增
void Queue_1_send_notify(TX_QUEUE *queue_ptr_1)
{
    tx_semaphore_put(&Gatekeeper);
}

/*****************************************************/
/* Notification function to increment Gatekeeper semaphore
   whenever a message has been sent to Queue_2 */
void Queue_2_send_notify(TX_QUEUE *queue_ptr_2)
{
    tx_semaphore_put(&Gatekeeper);
}
