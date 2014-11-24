#!/bin/bash

mod_path="."
dri_mod="ioctlS"
mode="664"

#Make sure only root can run this script
if [ "$(id -u)" != "0" ]; then
  echo "[Error] You have no permission to execute."
  exit 1
fi

#Install driver module
/sbin/insmod $mod_path/$dri_mod.ko || exit 1

#Get our driver major number
dri_major=$(awk -v MODULE=$dri_mod '$2==MODULE {print $1;}' /proc/devices)

#Get root group
group="staff"
grep -q '^staff:' /etc/group || group="wheel"

#Create device node to access 
rm -f /dev/${dri_mod}
mknod /dev/${dri_mod} c $dri_major 0

chgrp $group /dev/${dri_mod}
chmod $mode /dev/${dri_mod}

