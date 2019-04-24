import pika
import time

if __name__ == '__main__':
    conn = pika.BlockingConnection(
        pika.ConnectionParameters(
            host='172.81.240.3'))
    channel = conn.channel()
    channel.exchange_declare(
        exchange='persistent',
        exchange_type='direct',
        durable=True)

    for i in range(0, 90):
        message_body = 'Test Persistent ' + str(i)
        channel.basic_publish(
            exchange='persistent',
            routing_key='info',
            body=message_body,
            properties=pika.BasicProperties(
                delivery_mode=2))
        print('sent message {}'.format(i))
        time.sleep(1)

    conn.close()
