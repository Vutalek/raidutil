#ifndef CMDOUTPUT_H
#define CMDOUTPUT_H

void trim_and_copy_to_cmd_if_found(char* str, char* need_to_find);

char* build_progressbar_string(int width, int percent);

#endif
