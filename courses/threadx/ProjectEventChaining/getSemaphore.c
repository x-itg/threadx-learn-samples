
#include <user.h>

/****************************************************/
/*              Function Definitions                */
/****************************************************/

/* Entry function definition of Urgent_thread it has a higher priority than Routine_thread */
// 高优先级任务
void Urgent_thread_entry(ULONG thread_input)
{
    UINT status;
    ULONG start_time, cycle_time, current_time;

    /* This is the higher priority Urgent thread */
    while (1)
    {
        /* Get the starting time for this cycle */
        start_time = tx_time_get();

        /* Wait for a message to appear on either one of
           the two queues and sleep 7 timer ticks */
        tx_semaphore_get(&Gatekeeper, TX_WAIT_FOREVER); // 一直等待信号量

        /* Determine whether a message on Queue_1 or Queue_2 is available */
        status = tx_queue_receive(&Queue_1, receive_message_1, TX_NO_WAIT); // 取到信号量了 取队列消息

        if (status == TX_SUCCESS)
            ; /* A message on Queue_1 has been found - process */
        else
            /* Receive a message from Queue_2 */
            tx_queue_receive(&Queue_2, receive_message_2, TX_NO_WAIT);

        /* Process the message */
        tx_thread_sleep(7);

        /* Increment the thread counter and get timing info  */
        Urgent_thread_counter++;
        current_time = tx_time_get();
        cycle_time = current_time - start_time;
        total_Urgent_time += cycle_time;
    }
}

/************************************************************/

/* Entry function definition of Routine_thread
   it has a lower priority than Urgent_thread */

void Routine_thread_entry(ULONG thread_input)
{
    UINT status;
    ULONG start_time, cycle_time, current_time;
    while (1)
    {
        /* Get the starting time for this cycle */
        start_time = tx_time_get();

        /* Wait for a message to appear on either one of
           the two queues and sleep 20 timer ticks */
        tx_semaphore_get(&Gatekeeper, TX_WAIT_FOREVER);

        /* Determine whether a message on Queue_1 or Queue_2 is available */
        status = tx_queue_receive(&Queue_1, receive_message_1, TX_NO_WAIT);

        if (status == TX_SUCCESS)
            ; /* A message on Queue_1 has been found - process */
        else
            /* Receive a message from Queue_2 */
            tx_queue_receive(&Queue_2, receive_message_2, TX_NO_WAIT);

        /* Process the message */
        tx_thread_sleep(20);

        /* Increment the thread counter and get timing info  */
        Routine_thread_counter++;
        current_time = tx_time_get();
        cycle_time = current_time - start_time;
        total_Routine_time += cycle_time;
    }
}
