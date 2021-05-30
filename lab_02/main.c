#include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>
#include "interface.h"
#include "file_func.h"

int main()
{
    int res = 1, rc = 1;
    char *file_name = "in_file.txt";
    FILE *f = fopen(file_name, "a+");
    struct student array[MAX_ARR];
    struct key keys[MAX_ARR];
    int count = read_struct(f, array);
    while (res > 0 && res < 10 && rc)
    {
        app_hint();
        rc = scanf("%d", &res);
        if (rc)
            switch (res)
            {
                case 1:
                    output_data_all(count, array);
                    break;
                case 2:
                    output_data_certain(count, array);
                    break;
                case 3:
                    add_data(&count, array, f);
                    break;
                case 4:
                    delete_by_field(&count, array);
                    break;
                case 5:
                    key_init(count, array, keys);
                    sort_key_by_surname(count, keys);
                    print_keys(count, keys);
                    break;
                case 6:
                    key_init(count, array, keys);
                    sort_key_by_surname(count, keys);
                    output_data_by_key(count, keys, array);
                    break;
                case 7:
                    sort_data_by_surname(count, array);
                    output_data_all(count, array);
                    break;
                case 8:
                    key_efficiency(count, array);
                    break;
                case 9:
                    type_of_sorting_efficiency(count, array);
                    break;
                default:
                    end_hint();
            }
        else
            end_hint();
    }
    fclose(f);
    return res;
}
