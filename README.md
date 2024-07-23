# RaidUtil
Данная утилита упрощает администрирование RAID массивов.

На текущий момент реализованы три команды:
- статус RAID массивов;
- информация о конкретном диске;
- замена дисков.

## Статус RAID массивов
`./raid status`
Данная команда выводит иинформацию обо всех RAID массивах, которые указаны в файле /proc/mdstat.
Пример вывода:
\```
Found 2 RAID-arrays:
/dev/md126
/dev/md127

Information on each RAID array.

/dev/md126:
Array Size : 204667904 (195.19 GiB 209.58 GB)
Used Dev Size : 204667904 (195.19 GiB 209.58 GB)
Persistence : Superblock is persistent
State : clean
Active Devices : 2
Working Devices : 2
Failed Devices : 0
Spare Devices : 0
Disks status:
[0] /dev/sda5 active
[1] /dev/sda7 active

/dev/md127:
Array Size : 102333440 (97.59 GiB 104.79 GB)
Used Dev Size : 102333440 (97.59 GiB 104.79 GB)
Persistence : Superblock is persistent
State : clean
Active Devices : 2
Working Devices : 2
Failed Devices : 0
Spare Devices : 0
Disks status:
[0] /dev/sda3 active
[1] /dev/sda4 active
\```
