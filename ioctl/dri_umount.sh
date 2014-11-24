#!/bin/bash

dri_mod="ioctlS"

#Make sure only root can run this script
if [ "$(id -u)" != "0" ]; then
  echo "[Error] You have no permission to execute."
  exit 1
fi

#Remove main module from kernel
#/sbin/rmmod $main_mod || exit 1
#Remove driver module from kernel
/sbin/rmmod $dri_mod || exit 1

#Delete device node
rm -rf /dev/${dri_mod}
