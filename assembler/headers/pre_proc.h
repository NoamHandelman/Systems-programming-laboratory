#ifndef PRE_PROC_H
#define PRE_PROC_H

#include <stdio.h>
#include "./data_struct.h"

/**
 * @brief The main function for the pre proccess stage.
 * @param input_filename the path to the input file.
 * @param macro_list The list of macros that will be collected in the pre proccess file.
 * @param proccess_status The status of the proccess.
 * @return the path for the am file.
 */

char *exec_preproc(const char *input_filename, Macro **macro_list, int *proccess_status);

#endif
