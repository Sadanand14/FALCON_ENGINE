import yaml
import sys
from shutil import copyfile
import os

#https://stackoverflow.com/questions/21473076/pyyaml-and-unusual-tags
def ReadSceneFile(filepath):

	with open(filepath, "r") as file_desc:
		data = yaml.load_all(file_desc,Loader = yaml.Loader)
		for d in data:
			print (type(d))
	return data

def CleanFile(filepath):
	file_desc = open(filepath,"r")
	result = str()
	for lineNumber,line in enumerate(file_desc.readlines()):
		if lineNumber == 1:
			continue
		if line.startswith('--- !u!'):
			result+= '--- ' + line.split(' ')[2]
		else:
			result += line
	file_desc.close()
	

	#write cleaned file
	file_desc = open(filepath,'w')
	file_desc.write(result)
	file_desc.close()

		
if __name__ == "__main__":
	filepath = sys.argv[1]
	# creating copy of the file
	copyfile(filepath,"test.yaml")
	
	CleanFile("test.yaml")
	data = ReadSceneFile("test.yaml")
	#os.remove("test.yaml")
		#input("Hit enter")