# vnx-search
An open-source Search Engine based on VNX middleware.

## Dependencies
vnx-base (>=1.8.2)

`libcurl4-openssl-dev libicu-dev libxml2-dev libglibmm-2.4-dev libjemalloc-dev zlib1g-dev libmicrohttpd-dev mm-common`

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
