#ifndef STATUS_H
#define STATUS_H

void status();

char** find_raid_arrays(int* num);

void raid_array_status(char* arr);

void disk_from_array_short_status(char* mdadm_detail_disk_str);

#endif
