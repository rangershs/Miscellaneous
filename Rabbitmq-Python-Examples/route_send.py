#!/usr/bin/python3

import pika
import sys
import time

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()

	channel.exchange_declare(exchange='direct_logs', exchange_type='direct')

	# severity = sys.argv[1] if len(sys.argv) > 1 else 'info'
	# message = ' '.join(sys.argv[2:]) or 'Hello World!'
	severities = ('info', 'warning', 'error')
	for i in range(0, 120):
		index = i % 3
		severity = severities[index]
		message = 'Route log message' + ' ' + str(i)
		channel.basic_publish(
		    exchange='direct_logs', routing_key=severity, body=message)
		print(" [x] Sent %r:%r" % (severity, message))
		time.sleep(0.5)

	connection.close()