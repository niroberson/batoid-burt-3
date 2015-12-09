import os
import json

# Define the data storage directory, make sure this directory exists
base = os.getcwd()
data = os.path.join(base, 'sim_data')
if not os.path.exists(data):
		print('Created directory: ' + data)
		os.makedirs(data)

# Read in from config all of the parameters
config_path = os.path.join(base, 'config.json')
with open(config_path) as config_file:
	config = json.load(config_file)

# Generate C input files that combines parameters

# Generate strings that combines all parameters


# Create a list of all the paths of the directories to create
scaffold = []
for parm in ['w1', 'w2', 'w3', 'w4', 'w5']:
	this_path = os.path.join(data, parm)
	scaffold.append(this_path)

# Create all paths in scaffold
for directory in scaffold:
	if not os.path.exists(directory):
		print('Created directory: ' + directory)
		os.makedirs(directory)

# Copy the general case into each directory