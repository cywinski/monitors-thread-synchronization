#include "monitor.h"
#include "queue.h"

int TOTAL_MESSAGES;
/* Arrays of sleep times of every User. */
int SLEEP_USER1[10] = {0,0,1,0,0,0,0,0,0,0};
int SLEEP_USER2[10] = {0,0,1,0,0,0,0,0,0,0};
int SLEEP_VIP1[10] = {1,0,0,0,0,0,0,0,0,0};
int SLEEP_VIP2[10] = {1,0,0,0,0,0,0,0,0,0};
int SLEEP_READER[30] = {2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};

int SEND_MESSAGES = 0;

Queue* queue = new Queue(10);

void* vip_user1(void* param)
{
    std::cout << "VIP User 1 created\n";
    for (int i = 0; i < 10; i++)
    {
        sleep(SLEEP_VIP1[i]);
        Message* mess = new Message("VIP1 message", 1, SEND_MESSAGES);
        SEND_MESSAGES++;
        queue->putMessage(mess);
    }
    return nullptr;
}

void* vip_user2(void* param)
{
    std::cout << "VIP User 2 created\n";
    for (int i = 0; i < 10; i++)
    {
        sleep(SLEEP_VIP2[i]);
        Message* mess = new Message("VIP2 message", 1, SEND_MESSAGES);
        SEND_MESSAGES++;
        queue->putMessage(mess);
    }
    return nullptr;
}

void* normal_user1(void* param)
{
    std::cout << "Normal User 1 created\n";
    for (int i = 0; i < 10; i++)
    {
        sleep(SLEEP_USER1[i]);
        Message* mess = new Message("normal1 message", 0, SEND_MESSAGES);
        SEND_MESSAGES++;
        queue->putMessage(mess);
    }
    return nullptr;
}

void* normal_user2(void* param)
{
    std::cout << "Normal User 2 created\n";
    for (int i = 0; i < 10; i++)
    {
        sleep(SLEEP_USER2[i]);
        Message* mess = new Message("normal2 message", 0, SEND_MESSAGES);
        SEND_MESSAGES++;
        queue->putMessage(mess);
    }
    return nullptr;
}


void* reader(void* param)
{
    std::cout << "Reader created\n";
    while (queue->getReadMess() < TOTAL_MESSAGES)
    {
        sleep(SLEEP_READER[queue->getReadMess()]);
        queue->getMessage();
    }
    return nullptr;
}

void test_both_users(void)
{
    TOTAL_MESSAGES = 30;
    pthread_t user1_th, user2_th, vip1_th, reader_th;
    
    pthread_create(&user1_th, NULL, normal_user1, NULL);
    pthread_create(&user2_th, NULL, normal_user2, NULL);
    pthread_create(&vip1_th, NULL, vip_user1, NULL);
    pthread_create(&reader_th, NULL, reader, NULL);

    pthread_join(user1_th, NULL);
    pthread_join(user2_th, NULL);
    pthread_join(vip1_th, NULL);
    pthread_join(reader_th, NULL);
}


void test_normal_users(void)
{
    TOTAL_MESSAGES = 20;
    pthread_t user1_th, user2_th, reader_th;

    pthread_create(&user1_th, NULL, normal_user1, NULL);
    pthread_create(&user2_th, NULL, normal_user2, NULL);
    pthread_create(&reader_th, NULL, reader, NULL);

    pthread_join(user1_th, NULL);
    pthread_join(user2_th, NULL);
    pthread_join(reader_th, NULL);
}

void test_vip_users(void)
{
    TOTAL_MESSAGES = 20;
    pthread_t vip1_th, vip2_th, reader_th;
    
    pthread_create(&vip1_th, NULL, vip_user1, NULL);
    pthread_create(&vip2_th, NULL, vip_user2, NULL);
    pthread_create(&reader_th, NULL, reader, NULL);

    pthread_join(vip1_th, NULL);
    pthread_join(vip2_th, NULL);
    pthread_join(reader_th, NULL);
}

void test_empty_buffer_vip(void)
{
    TOTAL_MESSAGES = 10;
    pthread_t vip1_th, reader_th;
    
    pthread_create(&reader_th, NULL, reader, NULL);
    sleep(5);

    pthread_create(&vip1_th, NULL, vip_user1, NULL);
    
    pthread_join(reader_th, NULL);
    pthread_join(vip1_th, NULL);
    
}

void test_empty_buffer_normal(void)
{
    TOTAL_MESSAGES = 20;
    pthread_t user1_th, user2_th, reader_th;

    pthread_create(&reader_th, NULL, reader, NULL);
    sleep(5);

    pthread_create(&user1_th, NULL, normal_user1, NULL);
    pthread_create(&user2_th, NULL, normal_user2, NULL);
    
    pthread_join(reader_th, NULL);
    pthread_join(user1_th, NULL);
    pthread_join(user2_th, NULL);
    
}

void test_full_buffer_vip(void)
{
    TOTAL_MESSAGES = 10;
    pthread_t vip1_th, reader_th;
    
    pthread_create(&vip1_th, NULL, vip_user1, NULL);
    sleep(7);

    pthread_create(&reader_th, NULL, reader, NULL);

    pthread_join(vip1_th, NULL);
    pthread_join(reader_th, NULL);
}

void test_full_buffer_normal(void)
{
    TOTAL_MESSAGES = 20;
    pthread_t user1_th, user2_th, reader_th;

    pthread_create(&user1_th, NULL, normal_user1, NULL);
    pthread_create(&user2_th, NULL, normal_user2, NULL);
    sleep(7);

    pthread_create(&reader_th, NULL, reader, NULL);

    pthread_join(user1_th, NULL);
    pthread_join(user2_th, NULL);
    pthread_join(reader_th, NULL);
}

int main()
{
    test_both_users();
    //test_normal_users();
    //test_vip_users();
    //test_empty_buffer_vip();
    //test_empty_buffer_normal();
    //test_full_buffer_vip();
    //test_full_buffer_normal();

    delete queue;
    return 0;
}