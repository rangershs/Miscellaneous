#!/usr/bin/python3

import pika
import sys

def callback(ch, method, properties, body):
    print(" [x] %r:%r" % (method.routing_key, body))

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()
	# set persistent flag in exchange, queue, and message to make message durable.
	channel.exchange_declare(exchange='topic_logs', exchange_type='topic')

	result = channel.queue_declare('', exclusive=True)
	queue_name = result.method.queue

	# (*) can substitute for exactly one word.
	# (#) can substitute for one or more words.
	binding_keys = sys.argv[1:]
	if not binding_keys:
	    sys.stderr.write("Usage: %s [binding_key]...\n" % sys.argv[0])
	    sys.exit(1)

	for binding_key in binding_keys:
	    channel.queue_bind(
	        exchange='topic_logs', queue=queue_name, routing_key=binding_key)

	print(' [*] Waiting for logs. To exit press CTRL+C')

	channel.basic_consume(
	    queue=queue_name, on_message_callback=callback, auto_ack=True)

	channel.start_consuming()