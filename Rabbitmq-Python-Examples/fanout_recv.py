#!/usr/bin/python3

import pika

def callback(ch, method, properties, body):
	print(" [x] %r" % body)

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()

	channel.exchange_declare(exchange='logs', exchange_type='fanout')
	# once the consumer connection is closed, the queue should be deleted.
	result = channel.queue_declare('', exclusive=True)
	queue_name = result.method.queue

	channel.queue_bind(exchange='logs', queue=queue_name)

	print(' [*] Waiting for logs. To exit press CTRL+C')
	# auto acknowledge
	channel.basic_consume(
	    queue=queue_name, on_message_callback=callback, auto_ack=True)

	channel.start_consuming()