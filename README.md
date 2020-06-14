# vnx-search

# Dependencies
vnx-keyvalue (>=1.2.0)

> libcurl4-openssl-dev libicu-dev libxml2-dev libglibmm-2.4-dev mm-common

# Notes
Increase max open file limit in /etc/sysctl.conf:
> fs.file-max = 262144

Then add this in /etc/security/limits.conf:
> *         hard    nofile      262144
> *         soft    nofile      262144
