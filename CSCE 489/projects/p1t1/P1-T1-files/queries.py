from mongo_connect import connectMongo
import constants
import pymongo
import json
import pprint


collection = connectMongo()

##### FIND ALL ENTRIES IN THE DATABASE #####
# Assuming RQ0 is the query to find all entries in the database
RQ0 = collection.find()
for data in RQ0:
	pprint.pprint(data)

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

