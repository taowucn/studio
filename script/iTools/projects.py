#!/usr/bin/python

import os
import sys

def read_project():
	XML = "manifest.xml"
	logs = {}
	record_file = "proj"
	reponame = "ipc_cc"
	
	file = open (XML, 'r')
	for line in file:
		if line.find('name="') > 0 :
			start = line.find('name="') + len('name="')
		 	stop = line.find('"', start)
		 	path = line[start:stop]
		 	logs[path] = "0"
	file.close()
	
	file = open(record_file, 'w')
	for i in sorted(logs.keys(), key = str.lower):
		aLine = '%s/%s\n' % (reponame, i)
		file.writelines(aLine)
	file.close()

def main(argv):
	read_project()

	
if __name__ == '__main__':
	main(sys.argv[1:])

