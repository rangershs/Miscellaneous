#!/usr/bin/python3

import pika
import time

if __name__ == '__main__':

	connection = pika.BlockingConnection(
	    pika.ConnectionParameters(host='172.81.240.3'))
	channel = connection.channel()

	channel.queue_declare(queue='hello')

	num = 0
	while True:
		if num >= 100:
			break;
		channel.basic_publish(exchange='', routing_key='hello', body='Hello Rabbitmq!')
		print(" [x] Sent 'Hello World!'")
		num += 1
		time.sleep(0.5)

	connection.close()