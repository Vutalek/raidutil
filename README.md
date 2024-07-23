# RaidUtil
Данная утилита упрощает администрирование RAID массивов.

На текущий момент реализованы три команды:
- статус RAID массивов;
- информация о конкретном диске;
- замена дисков.

## Статус RAID массивов
`./raid status`

Данная команда выводит информацию обо всех RAID массивах, которые указаны в файле /proc/mdstat.

Пример вывода:
```
./raid status

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
```

## Информация о конкретном диске
`./raid get <device-name>`

Выводит информацию об указанном диске.

Пример вывода:
```
./raid get /dev/sda5

Array UUID : ed011c1b:30b28470:9a4330f8:e06b2237
Avail Dev Size : 409335808 sectors (195.19 GiB 209.58 GB)
State : active
Device UUID : e996d58f:039fec7d:c8fe0024:14c0f6c1
Device Role : Active device 0
Array State : active
```

## Замена дисков
`./raid replace <old-device> <new-device>`

Осуществляет замену диска в RAID массиве. Утилита сначал ищет массив, в котором размещён old-device.
Затем утилита добавляет в массив new-device, осуществляется замену дисков и удаляет старый диск из массива. Перед удалением, утилита выводит на экран прогресс бар процесса синхронизации нового диска.

Пример вывода:
```
./raid replace /dev/sda5 /dev/sda6

mdadm: added /dev/sda6
mdadm: Marked /dev/sda5 (device 0 in /dev/md126) for replacement
mdadm: Marked /dev/sda6 in /dev/md126 as replacement for device 0
Recovery in progress [##                                               ] 5.0%
```

## Зависимости
Утилитой можно пользоваться исключительно на unix-like системах.

Необходимые зависимости:
- mdadm;
- make.

## Установка
Сначала нобходимо скачать репозиторий на компьютер с помощью архива, либо с помощью команды `git clone`.

Затем необходимо открыть консоль в папке с проектом и ввести следующую команду:

`sudo make install`

Произойдёт компиляция и установка исполняемого файла в /usr/local/bin.

### Деинсталляция

Для деинсталляции, необходимо написать:

`sudo make uninstall`

### Другая директория установки

Если утилиту необходимо установить в другое место, то необходимо исправить Makefile, изменив строчку

`PREFIX = <directory>`

### Перекомпиляция

Для перекомпиляции проекта необходимо динсталлировать утилиту командой

`sudo make uninstall`

и удалить результат ыкомпиляции командой

`sudo make clean`
