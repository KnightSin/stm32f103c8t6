#ifndef USER_USART_CONTROL_H
#define USER_USART_CONTROL_H
extern xTaskHandle USART_HEART_BEAT;//发送给串口的心跳信息
extern SemaphoreHandle_t serial_sem;//串口资源互斥量
void usart_heart_beat();

#endif