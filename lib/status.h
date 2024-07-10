#ifndef STATUS_H
#define STATUS_H

char** find_raid_arrays(int* num);

void raid_array_status(char* arr);

void copy_to_cmd_if_found_status_str(char* str, char* need_to_find);

void disk_from_array_short_status(char* mdadm_detail_disk_str);

#endif
