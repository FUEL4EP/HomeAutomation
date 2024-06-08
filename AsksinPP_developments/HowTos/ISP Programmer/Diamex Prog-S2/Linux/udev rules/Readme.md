# installation

- save 59-diamex.rules to /etc/udev/rules.d/59-diamex.rules
- ensure that linux user belongs to group dialout
- execute sudo chmod 644 /etc/udev/rules.d/59-diamex.rules 
- ls -la shows e.g.
> -rw-rw-r-- 1 root   242 Jun  6 14:24 59-diamex.rules

- restart the udev service or reboot, e.g.:
> sudo /etc/init.d/udev restart
