import pika


def callback(ch, method, properties, body):
    print('Received Message is {}'.format(body))
    ch.basic_ack(delivery_tag=method.delivery_tag)


if __name__ == '__main__':
    conn = pika.BlockingConnection(
        pika.ConnectionParameters(
            host='172.81.240.3'))
    channel = conn.channel()
    channel.exchange_declare(
        exchange='persistent',
        exchange_type='direct',
        durable=True)

    queue_name = 'persistent_queue'
    channel.queue_declare(queue=queue_name, durable=True)
    channel.queue_bind(
        queue=queue_name,
        exchange='persistent',
        routing_key='info')
    channel.basic_consume(queue=queue_name, on_message_callback=callback)

    channel.start_consuming()
