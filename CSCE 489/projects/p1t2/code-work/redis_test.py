# messenger.py
# Project 1 Task 1
# due 10/6/2017

# Carsten Hood
# UIN: 922009787
# CSCE 489-599 Cloud Computing
# Fall 2017

import redis
#import json
#import pprint

r = redis.StrictRedis(host='localhost', port=6379, db=0)
#print dir(r)
r.set('foo', 'bar')
print r.get('foo')