#!/usr/bin/python3

import pika
import sys
import time

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()

	channel.exchange_declare(exchange='logs', exchange_type='fanout')

	# message = ' '.join(sys.argv[1:]) or "info: Hello World!"
	for i in range(0, 100):
		message = 'Info: Hello World!' + ' ' + str(i)
		channel.basic_publish(exchange='logs', routing_key='', body=message)
		print(" [x] Sent %r" % message)
		time.sleep(0.5)

	connection.close()