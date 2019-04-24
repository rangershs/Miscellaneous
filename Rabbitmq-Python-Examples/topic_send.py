#!/usr/bin/python3

import pika
import sys
import time

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()

	channel.exchange_declare(exchange='topic_logs', exchange_type='topic')

	# routing_key = sys.argv[1] if len(sys.argv) > 2 else 'anonymous.info'
	# message = ' '.join(sys.argv[2:]) or 'Hello World!'
	# (*) can substitute for exactly one word.
	# (#) can substitute for one or more words.
	routing_key = 'topic.info'
	for i in range(0, 120):
		message = 'topic route ' + str(i)
		channel.basic_publish(
		    exchange='topic_logs', routing_key=routing_key, body=message)
		print(" [x] Sent %r:%r" % (routing_key, message))
		time.sleep(0.5)

	connection.close()