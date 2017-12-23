# messenger.py
# Project 1 Task 1
# due 10/6/2017

# Carsten Hood
# UIN: 922009787
# CSCE 489-599 Cloud Computing
# Fall 2017

from mongo_connect import connectMongo
import constants
import pymongo
import json
import pprint
import redis

collection = connectMongo()

##### FIND ALL ENTRIES IN THE DATABASE #####
# Assuming RQ0 is the query to find all entries in the database
RQ0 = collection.find()
for data in RQ0:
	pprint.pprint(data)

pprint.pprint("hello")

######## FIND ENTRIES WITH CONDITION #######
######## collection.find(CONDITION) #######
######## E.g., collection.find({"Name" : "Alice"}) #######

######## UPDATE ENTRIES WITH CONDITION ########
######## collection.update_one(CONDITION, _update_) #######
######## collection.update_many(CONDITION, _update_)
######## E.g., collection.find({"Name" : "Alice"}, {"$inc" : {"age" : 1} })

######## DELETE ENTRIES WITH CONDITION ########
######## collection.delete_one(CONDITION) #######
######## collection.delete_many(CONDITION)
######## E.g., collection.find({"Name" : "Alice"})

######## AGGREGATE ENTRIES WITH PIPELINE ########
######## collection.aggregate(PIPELINE) ########


r = redis.Redis()


subscribing = False;
topic = "";

while True:
	try:
		if subscribing:
			print("Sub")
			for item in p.listen():
				print(item)

		cmd = raw_input('Enter your command: ')
		print(cmd)
		cmd_parts = cmd.split(" ")
		print(cmd_parts)
		if cmd_parts[0] == "set":
			to_set = ' '.join(cmd_parts[2:])
			r.set(cmd_parts[1], to_set)
		elif cmd_parts[0] == "get":
			res = r.get(cmd_parts[1])
			print res
		elif cmd_parts[0] == "pub":
			to_pub = ' '.join(cmd_parts[2:])
			res = r.publish(cmd_parts[1], to_pub)
			print res
		elif cmd_parts[0] == "sub":
			subscribing = True;
			p = r.pubsub()
			res = p.subscribe([cmd_parts[1]])
			print res
		elif cmd_parts[0] == "quit":
			break;
		else:
			print("Input format wrong");

	except KeyboardInterrupt:
		subscribing = False

