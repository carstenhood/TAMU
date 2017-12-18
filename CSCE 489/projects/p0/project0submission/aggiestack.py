# aggiestack.py
# Project 0
# due 9/8/2017

# Carsten Hood
# UIN: 922009787
# CSCE 489-599 Cloud Computing
# Fall 2017

# ================================================
# global variables

# data structures
class Machine:
    pass
class Image:
    pass
class Flavor:
    pass
class Instance:
    pass

# global containers
machine_list = []
image_list = []
flavor_list = []
instances = []

# ================================================
# functions for handling user commands

# ----------------------------------
# file-reading functions

# read hardware configuration items from a given text file
def read_hardware(filename):
    try:
        input_file = open(filename, 'r')
        machine_count = int(input_file.readline()) # consume first line
        for line in input_file:
            try:
                # parse each machine's hardware data items within each line
                m = Machine()
                values = line.split()
                m.name = values[0]
                m.ip = values[1]
                m.mem = int(values[2])
                m.disks = int(values[3])
                m.vcpus = int(values[4])
                # initialize variables to track resource usage
                m.mem_avail = m.mem
                m.disks_avail = m.disks
                m.vcpus_avail = m.vcpus
                machine_list.append(m)
            except:
                 print "error reading line: \"" + line.rstrip() + "\""
        input_file.close()
    except (IOError, OSError):
        print "error: unable to open file \"" + filename + "\""
        return False
    except:
        print "unknown error"
        return False
    return True

# read image configuration items from a given text file
def read_images(filename):
    try:
        input_file = open(filename, 'r')
        image_count = int(input_file.readline()) # consume first line
        for line in input_file:
            try:
                # parse data items within each line
                image = Image()
                values = line.split()
                image.name = values[0]
                image.path = values[1]
                image_list.append(image)
            except:
                 print "error reading line: \"" + line.rstrip() + "\""
        input_file.close()
    except (IOError, OSError):
        print "error: unable to open file \"" + filename + "\""
        return False
    except:
        print "unknown error"
        return False
    return True

# read flavor configuration items from a given text file
def read_flavors(filename):
    try:
        input_file = open(filename, 'r')
        flavor_count = int(input_file.readline()) # consume first line
        for line in input_file:
            try:
                # parse each flavor's data items within each line
                flavor = Flavor()
                values = line.split()
                flavor.name = values[0]
                flavor.ram = int(values[1])
                flavor.disks = int(values[2])
                flavor.vcpus = int(values[3])
                flavor_list.append(flavor)
            except:
                 print "error reading line: \"" + line.rstrip() + "\""
        input_file.close()
    except (IOError, OSError):
        print "error: unable to open file \"" + filename + "\""
        return False
    except:
        print "unknown error"
        return False
    return True

# ----------------------------------
# output functions

# print hardware configuration of each machine
def show_hardware():
    # first print number of machines
    print str(len(machine_list))
    # cycle through machines and print configuration details on each line
    for m in machine_list:
        print m.name + ' ' + m.ip + str(m.mem) + ' '\
                           + str(m.disks) + ' ' + str(m.vcpus)
    return True

# print image data
def show_images():
    # first print number of image configurations in list
    print str(len(image_list))
    # cycle through images and print details on each line
    for image in image_list:
        print image.name + ' ' + image.path
    return True

# print flavor information
def show_flavors():
    # first print number of flavors in list
    print str(len(flavor_list))
    # cycle through flavors and print details on each line
    for flavor in flavor_list:
        print flavor.name + ' ' + str(flavor.ram) + ' '\
              + str(flavor.disks) + ' ' + str(flavor.vcpus)
    return True

def show_all():
    return show_hardware() and show_images() and show_flavors()

# ----------------------------------
# admin functions

# list available resources for each machine
def admin_show_hardware():
    # first print number of machines
    print "displaying " + str(len(machine_list)) + " server(s):\n"
    # cycle through machines and print resource availability on each line
    for m in machine_list:
        print "server: \"" + m.name + "\", IP address: " + m.ip
        print str(m.mem_avail) + " / " + str(m.mem) + " GB memory available"
        print str(m.disks_avail) + " / " + str(m.disks) + " disks available"
        print str(m.vcpus_avail) + " / " + str(m.vcpus) + " vcpus available"
        print
    return True

# returns true if given machine can accommodate a new virtual server of given flavor
def admin_can_host(machine_name, flavor_name):

    # identify machine with given name in machine list & abort if not found
    machine = next((m for m in machine_list if m.name == machine_name), None)
    if not machine:
        print "error: no machine \"" + machine_name + "\""
        return False

    # identify flavor with given name in flavor list & abort if not found
    flavor = next((f for f in flavor_list if f.name == flavor_name), None)
    if not flavor:
        print "error: no flavor \"" + flavor_name + "\""
        return False

    # print whether machine's resources can accommodate a virtual server of type flavor
    if machine.mem_avail < flavor.ram \
            or machine.disks_avail < flavor.disks \
            or machine.vcpus_avail < flavor.vcpus:
        print "no"
    else:
        print "yes"

    # return true to indicate successful completion (regardless of yes/no)
    return True

# ================================================
# main code

# open/generate text file for logging command input & successfulness
logfile = open('aggiestack-log.txt', 'w')

# lines printed at start of program
print "aggiestack is running"
print "enter \"exit\" or \"quit\" to terminate"

# process user-entered commands until program terminates
while True:
    # read user input command
    command = raw_input("\nenter command: ").strip()

    # initialize variable to denote successfulness of command
    success = False

    # call appropriate functions based on parsing command
    if "admin show hardware" in command:
        success = admin_show_hardware()
    elif "admin can_host" in command:
        args = command.rsplit(' ',2)
        machine_name = args[-2]
        flavor_name = args[-1]
        success = admin_can_host(machine_name, flavor_name)
    elif "config --hardware" in command:
        filename = command.rsplit(' ',1)[-1]
        success = read_hardware(filename)
    elif "config --images" in command:
        filename = command.rsplit(' ',1)[1]
        success = read_images(filename)
    elif "config --flavors" in command:
        filename = command.rsplit(' ', 1)[1]
        success = read_flavors(filename)
    elif "show hardware" in command:
        success = show_hardware()
    elif "show images" in command:
        success = show_images()
    elif "show flavors" in command:
        success = show_flavors()
    elif "show all" in command:
        success = show_all()
    elif command == "exit" or command == "quit":
        print "terminating program"
        break
    else:
        print "error: invalid command"

    # log whether command was successfully processed
    success_text = "SUCCESS" if success else "FAILURE"
    log = command + " (" + success_text + ")\n"
    logfile.write(str(log))

logfile.close()

# end of program