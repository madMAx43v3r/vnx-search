# vnx-search
An open-source Search Engine based on VNX middleware.

## Dependencies
vnx-base (>=1.6.8)

`libcurl4-openssl-dev libicu-dev libxml2-dev libglibmm-2.4-dev libjemalloc-dev mm-common`

## Notes
Increase max open file limit in /etc/sysctl.conf:
```
fs.file-max = 262144
```

Then add this in /etc/security/limits.conf:
```
*         hard    nofile      262144
*         soft    nofile      262144
```
