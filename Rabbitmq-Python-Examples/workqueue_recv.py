#!/usr/bin/python3

import pika
import time

def callback(ch, method, properties, body):
    print(" [x] Received %r" % body)
    time.sleep(body.count(b'.'))
    print(" [x] Done")
    ch.basic_ack(delivery_tag=method.delivery_tag)

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()
	# make queue durable
	channel.queue_declare(queue='task_queue', durable=True)
	print(' [*] Waiting for messages. To exit press CTRL+C')
	# fair dispatch, not round-robin dispatch
	channel.basic_qos(prefetch_count=1)
	channel.basic_consume(queue='task_queue', on_message_callback=callback)

	channel.start_consuming()