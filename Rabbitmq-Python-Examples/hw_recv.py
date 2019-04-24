#!/usr/bin/python3

import pika

def callback(ch, method, properties, body):
	print(" [x] Received %r" % body)

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()

	channel.queue_declare(queue='hello')

	channel.basic_consume(
	    queue='hello', on_message_callback=callback, auto_ack=True)

	print(' [*] Waiting for messages. To exit press CTRL+C')
	channel.start_consuming()