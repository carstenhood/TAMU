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
class Rack:
    pass
class Machine:
    pass
class Image:
    pass
class Flavor:
    pass
class Instance:
    pass

# global containers (dictionaries)
racks = {}
images = {}
flavors = {}


# ================================================
# helper functions

# set machine's resource usage to maximum availability
def set_resources(machine):
    machine.mem_avail = machine.mem
    machine.disks_avail = machine.disks
    machine.vcpus_avail = machine.vcpus

# adjust machine's resource usage when new instance is added
def occupy_resources(machine, flavor):
    machine.mem_avail -= flavor.ram
    machine.disks_avail -= flavor.disks
    machine.vcpus_avail -= flavor.vcpus

# adjust machine's resource usage when instance is removed
def free_resources(machine, flavor):
    machine.mem_avail += flavor.ram
    machine.disks_avail += flavor.disks
    machine.vcpus_avail += flavor.vcpus


# ================================================
# functions for handling user commands

# ----------------------------------
# P0 file-reading functions

# read hardware configuration items from a given text file
def read_hardware(filename):
    try:
        input_file = open(filename, 'r')
        # machine_count = int(input_file.readline()) # consume first line
        # interpret number of lines given in first line as rack lines
        racks_count = int(input_file.readline())
        for i in range(racks_count):
            try:
                line = input_file.readline()
                values = line.split()
                r = Rack()
                r.name = values[0]
                r.storage_capacity = values[1]
                r.machines = {}
                racks[r.name] = r
            except:
                print "error reading rack line"

        # start parsing machine lines after rack lines
        machines_count = int(input_file.readline())
        for i in range(machines_count):
            try:
                line = input_file.readline()
                values = line.split()
                m = Machine()
                # parse each machine's hardware data items within each line
                m.name = values[0]
                m.ip = values[2]
                m.mem = int(values[3])
                m.disks = int(values[4])
                m.vcpus = int(values[5])
                # initialize variables to track resource usage
                set_resources(m)
                m.instances = {}
                # add machine to its rack's list
                m.rack = values[1]
                racks[m.rack].machines[m.name] = m
            except:
                 print "error reading machine line"
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
                image.space = values[1]
                image.path = values[2]
                images[image.name] = image
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
                flavors[flavor.name] = flavor
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
# P0 output functions

# print hardware configuration of each machine
def show_hardware():
    # first print number of machines
    machine_count = 0
    for rack_key, rack in racks.iteritems():
        machine_count += len(rack.machines)
    print str(machine_count)
    # cycle through machines and print configuration details on each line
    for rack_key, rack in racks.iteritems():
        for m_key, m in rack.machines.iteritems():
            print m.name + ' ' + m.rack + ' ' + m.ip + ' ' + str(m.mem)\
                  + ' ' + str(m.disks) + ' ' + str(m.vcpus)
    return True

# print image data
def show_images():
    # first print number of image configurations in list
    print str(len(images))
    # cycle through images and print details on each line
    for image_key, image in images.iteritems():
        print image.name + ' ' + image.space + ' ' + image.path
    return True

# print flavor information
def show_flavors():
    # first print number of flavors in list
    print str(len(flavors))
    # cycle through flavors and print details on each line
    for flavor_key, flavor in flavors.iteritems():
        print flavor.name + ' ' + str(flavor.ram) + ' '\
              + str(flavor.disks) + ' ' + str(flavor.vcpus)
    return True

def show_all():
    return show_hardware() and show_images() and show_flavors()

# ----------------------------------
# P0 admin functions

# list available resources for each machine
def admin_show_hardware():
    # first print number of machines
    machine_count = 0
    for rack_key, rack in racks.iteritems():
        machine_count += len(rack.machines)
    print "displaying " + str(machine_count) + " server(s):"
    # cycle through machines and print resource availability on each line
    for rack_key, rack in racks.iteritems():
        for m_key, m in rack.machines.iteritems():
            print m.name\
                  + ":\t" + str(m.mem_avail) + "/" + str(m.mem) + " GB mem"\
                  + ",\t" + str(m.disks_avail) + "/" + str(m.disks) + " disks"\
                  + ",\t" + str(m.vcpus_avail) + "/" + str(m.vcpus) + " vcpus"
    return True

# returns "yes" if given machine can accommodate a new virtual server of given flavor
def admin_can_host(machine_name, flavor_name):

    # search for machine with given name in racks' machine lists & abort if not found
    machine = None
    for rack_key, rack in racks.iteritems():
        if machine_name in rack.machines:
            machine = rack.machines[machine_name]
            break
    if machine is None:
        print "error: no machine \"" + machine_name + "\""
        return "fail"

    # identify flavor with given name in flavor list & abort if not found
    flavor = None
    if flavor_name in flavors:
        flavor = flavors[flavor_name]
    if flavor is None:
        print "error: no flavor \"" + flavor_name + "\""
        return "fail"

    # print whether machine's resources can accommodate a server of type flavor
    if machine.mem_avail < flavor.ram \
            or machine.disks_avail < flavor.disks \
            or machine.vcpus_avail < flavor.vcpus:
        return "no"
    else:
        return "yes"

    return "fail"

# ----------------------------------
# P4 Part A functions

# creates an instance with given flavor to boot on given image
def server_create(image_name, flavor_name, instance_name):

    # cancel operation if desired instance name already exists
    for rack_key, rack in racks.iteritems():
        for m_key, m in rack.machines.iteritems():
            if instance_name in m.instances:
                print "instance " + instance_name + " already exists"\
                      + "; choose a unique instance name"
                return False

    # check that desired flavor and image exist
    flavor = None
    if flavor_name in flavors:
        flavor = flavors[flavor_name]
    else:
        print "flavor " + flavor_name + " not found"
        return False
    image = None
    if image_name in images:
        image = images[image_name]
    else:
        print "image " + image_name + " not found"
        return False

    # create instance according to image and flavor
    instance = Instance()
    instance.name = instance_name
    instance.image = image_name
    instance.flavor = flavor_name

    # find machine to host instance
    machine_found = False
    for rack_key, rack in racks.iteritems():
        # exit loops of machine has been found
        if machine_found:
            break
        for m_key, m in rack.machines.iteritems():
            if admin_can_host(m.name, flavor_name) == "yes":
                # add instance to machine
                machine_found = True
                instance.machine = m.name
                m.instances[instance_name] = instance
                occupy_resources(m, flavor)
                print "now hosting " + instance.name + \
                      " on " + instance.machine
                break
    if not machine_found:
        print "could not locate resources to host " + instance_name
        return False

    # return true to indicate successful completion
    return True

# finds and deletes a given instance and liberates its resources
def server_delete(instance_name):
    instance_found = False
    for r_key, r in racks.iteritems():
        if instance_found: break
        for m_key, m in r.machines.iteritems():
            if instance_name in m.instances:
                instance_found = True
                flavor = flavors[m.instances[instance_name].flavor]
                # remove instance from machine
                del m.instances[instance_name]
                free_resources(m, flavor)
                print instance_name + " successfully deleted"
                break
    if not instance_found:
        print "instance " + instance_name + " not found"
        return False
    return True

# print each instance and its associated image and flavor
def server_list():
    # cycle through machines and print instances on each line
    for r_key, r in racks.iteritems():
        for m_key, m in r.machines.iteritems():
            for i_key, i in m.instances.iteritems():
                print i.name + " " + i.image + " " + i.flavor
    return True

# print each instance and its host hardware, i.e., machine, rack, & IP address
def admin_show_instances():
    # cycle through machines and print instances on each line
    for r_key, r in racks.iteritems():
        for m_key, m in r.machines.iteritems():
            for i_key, i in m.instances.iteritems():
                print i.name + " " + m_key + " " + m.ip + " " + r_key
    return True

# ----------------------------------
# P4 Part B functions

# iterate across rack's instances and attempt to migrate them to new machines
def admin_evacuate(rack_name):
    success = False
    if rack_name in racks:
        for machine_key, machine in racks[rack_name].machines.iteritems():
            for i_key, i in machine.instances.items():
                # iterate through racks to migrate instance
                machine_found = False
                for rack_key, rack in racks.iteritems():
                    # skip rack that is being evacuated (instance is already on it)
                    if rack_key == rack_name:
                        continue
                    # if machine found then move to next instance
                    if machine_found:
                        break
                    # seek machine to migrate instance to
                    for m_key, m in rack.machines.iteritems():
                        flavor = flavors[i.flavor]
                        if admin_can_host(m.name, flavor.name) == "yes":
                            # remove instance from old machine
                            del machine.instances[i_key]
                            free_resources(machine, flavor)
                            # add instance to machine
                            machine_found = True
                            i.machine = m.name
                            m.instances[i.name] = i
                            occupy_resources(m, flavor)
                            print "migrated " + i.name + \
                                  " to " + i.machine
                            # return success if at least one instance is evacuated
                            success = True
                            break
                if not machine_found:
                    print "could not locate resources to migrate " + i.name
    else:
        print "rack " + rack_name + " not found"

    return success

# remove machine from rack (and remove any hosted instances)
def admin_remove(machine):
    machine_found = False
    # locate machine in racks
    for r_key, rack in racks.iteritems():
        if machine in rack.machines:
            machine_found = True
            # remove machine
            rack.machines[machine].instances.clear()
            del rack.machines[machine]
            print "machine " + machine + " removed"
            return True
    if not machine_found:
        print "machine " + machine + " not found"
    return False

# add machine to rack and establish variables
def admin_add(mem, num_disks, vcpus, ip, rack_name, machine):
    # check that new machine name is unique
    for rack_key, rack in racks.iteritems():
        if machine in rack.machines:
            print "machine " + machine + " already exists" \
                  + "; choose a unique machine name"
            return False
    # check that rack name exists
    if rack_name in racks:
        # add new machine to rack
        m = Machine()
        m.name = machine
        m.ip = ip
        m.mem = mem
        m.disks = num_disks
        m.vcpus = vcpus
        set_resources(m)
        m.instances = {}
        m.rack = rack_name
        racks[m.rack].machines[m.name] = m
        print "machine " + machine + " added"
        return True
    else:
        print "rack " + rack_name + " not found"
        return False

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
    try:
        # P0 commands
        if "admin show hardware" in command:
            success = admin_show_hardware()
        elif "admin can host" in command:
            args = command.rsplit(' ', 2)
            machine_name = args[-2]
            flavor_name = args[-1]
            result = admin_can_host(machine_name, flavor_name)
            success = result != "fail"
            if success: print result
        elif "config --hardware" in command:
            filename = command.rsplit(' ', 1)[1]
            success = read_hardware(filename)
        elif "config --images" in command:
            filename = command.rsplit(' ', 1)[1]
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

        # P4 Part A commands
        elif "server create" in command:
            args = command.split(' ')
            image_name = args[-4]
            flavor_name = args[-2]
            instance_name = args[-1]
            success = server_create(image_name, flavor_name, instance_name)
        elif "server delete" in command:
            instance_name = command.rsplit(' ', 1)[1]
            success = server_delete(instance_name)
        elif "server list" in command:
            success = server_list()
        elif "admin show instances" in command:
            success = admin_show_instances()

        # P4 Part B commands
        elif "admin evacuate" in command:
            rack_name = command.rsplit(' ', 1)[1]
            success = admin_evacuate(rack_name)
        elif "admin remove" in command:
            machine = command.rsplit(' ', 1)[1]
            success = admin_remove(machine)
        elif "admin add" in command:
            args = command.split(' ')
            mem = int(args[-10])
            num_disks = int(args[-8])
            vcpus = int(args[-6])
            ip = args[-4]
            rack_name = args[-2]
            machine = args[-1]
            success = admin_add(mem, num_disks, vcpus, ip,
                                rack_name, machine)

        # exit commands and invalid commands
        elif command == "exit" or command == "quit":
            print "terminating program"
            break
        else:
            print "error: invalid command"

    except:
        print "error: command failed"

    # log whether command was successfully processed
    success_text = "SUCCESS" if success else "FAILURE"
    log = command + " (" + success_text + ")\n"
    logfile.write(str(log))

logfile.close()

# end of program