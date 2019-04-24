#!/usr/bin/python3

import pika
import sys
import time

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()
	# make queue durable
	channel.queue_declare(queue='task_queue', durable=True)

	for i in range(1, 11):
		# message = ' '.join(sys.argv[1:]) or "Hello World!"
		message = 'Message' + ('.' * i)
		channel.basic_publish(
		    exchange='',
		    routing_key='task_queue',
		    body=message,
		    properties=pika.BasicProperties(
		        delivery_mode=2,  # make message persistent
		    ))
		print(" [x] Sent %r" % message)
		time.sleep(1)

	connection.close()