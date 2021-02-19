#ifndef QUEUE_H
#define QUEUE_H

#include "monitor.h"
#include <iostream>
#include <string>

class Message
{
private:
    std::string m_text;     /* Text of message */
    Message* m_next;        /* Pointer to next message in queue */
    int m_priority;         /* Priority of message */
    int m_num_of_mess;      /* Number indicating when message was putted to queue */
    int m_num_of_generation;/* Number indicating when message was generated */

public:
    Message(std::string text, int priority, int num_of_generation) : 
        m_text{text}, 
        m_next{nullptr}, 
        m_priority{priority}, 
        m_num_of_mess{0}, 
        m_num_of_generation{num_of_generation}
        {}

    ~Message() {}

    std::string getText()
    {
        return m_text;
    }

    int getPriority()
    {
        return m_priority;
    }

    Message* getNext()
    {
        return m_next;
    }

    void setNext(Message* mess)
    {
        m_next = mess;
    }

    void print()
    {
        std::cout << "[" << m_num_of_generation << "]" << "[" << m_num_of_mess << "]" << " " << m_text <<  std::endl;
    }

    void setNumOfMess(int num_of_mess)
    {
        m_num_of_mess = num_of_mess;
    }

    bool isValid()
    {
        return (m_text.length() >= 8 && m_text.length() <= 16);
    }
};


class Queue : private Monitor
{

    int m_current_size;         /* Current number of messages in queue */
    Message* m_last_vip_mess;   /* Pointer to last VIP message in queue */
    Message* m_head;            /* Pointer to first message in queue */
    Message* m_tail;            /* Pointer to last message in queue */
    int m_max_size;             /* Maximal number of messages in queue */
    int m_send_mess;            /* Number of sent messages */
    int m_read_mess;            /* Number of read messages */
    Condition m_empty;          
    Condition m_full;

public:
    Queue(int queue_size) : 
        m_current_size{0}, 
        m_last_vip_mess{nullptr}, 
        m_head{nullptr}, 
        m_tail{nullptr}, 
        m_max_size{queue_size}, 
        m_send_mess{0}, 
        m_read_mess{0}, 
        m_empty{}, 
        m_full{}
        {}

    ~Queue()
    {
        delete m_head;
    }

    void putMessage(Message *message)
    {
        enter();

        message->setNumOfMess(m_send_mess);
        m_send_mess++;

        if (message->isValid())
        {
            if (m_current_size == m_max_size)
            {
                wait(m_full);
            }
            
            if (m_current_size == 0)
            {
                // First message
                m_head = message;
                m_tail = message;
                if (message->getPriority() == 1)
                {              
                    m_last_vip_mess = message;
                }
            }
            else
            {
                if (message->getPriority() == 1)
                {
                    if (m_last_vip_mess == nullptr)
                    {
                        // First VIP message
                        message->setNext(m_head);
                        m_head = message;
                    }
                    else
                    {
                        if (m_tail->getPriority() == 1)
                        {
                            // Only VIP messages in queue
                            m_tail->setNext(message);
                            m_tail = message;
                        }
                        else
                        {
                            // Both types of messages already in queue
                            message->setNext(m_last_vip_mess->getNext());
                            m_last_vip_mess->setNext(message);
                        }   
                    }
                    m_last_vip_mess = message;
                }
                else
                {
                    // Normal message
                    m_tail->setNext(message);
                    m_tail = message;
                }
            }

            m_current_size++;
            if (m_current_size == 1)
            {
                signal(m_empty);
            }
        }
        else
        {
            std::cout << "ERROR:";
            message->print();
            std::cout << "WRONG MESSAGE LENGTH" << std::endl;
            m_read_mess++;
        }
        
        leave();
    }

    void getMessage()
    {
        enter();
        if (m_current_size == 0)
        {
            wait(m_empty);
        }
        if (m_head != nullptr)
        {
            Message* temp = m_head;
            m_head->print();
            m_head = m_head->getNext();
            delete temp;

            if (m_head != nullptr)
            {
                if (m_head->getPriority() == 0)
                {
                    m_last_vip_mess = nullptr;
                }
            }
            if (m_current_size == 1)
            {
                m_last_vip_mess = nullptr;
            }
        }

        m_read_mess++;
        m_current_size--;

        if (m_current_size == m_max_size - 1)
        {
            signal(m_full);
        }
        leave();
    }

    void print()
    {
        std::cout << "|";
        Message* curr = m_head;
        while (curr != nullptr)
        {
            curr->print();
            std::cout << '-';
            curr = curr->getNext();
        }
        std::cout << "|\n";
    }

    int getReadMess()
    {
        return m_read_mess;
    }
};

#endif // QUEUE_H