/*
 * @filename imxdownload.c
 * @author 汤鑫
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./imxdownload.h"

#define SHELL_CMD_SIZE (100)
#define BIN_OFFSET (3072)

enum
{
    ddr_256 = 0, // 256
    ddr_512 = 1, // 512
};

int main(int argc, char const *argv[])
{
    unsigned char *file_buf = NULL;
    unsigned char *sheel_cmd_buf = NULL;
    FILE *file = NULL;
    char file_name[64] = {0};
    char device_name[64] = {0};
    char download_file_name[32] = {0};
    int file_size = 0;
    int write_bytes = 0;
    int ddr_type = ddr_512; // default

    if (argc != 3 && argc != 4)
    {
        printf("pleause use command \"%s <-512MB or -256MB> <source_bin> <sd_device>\"\n", argv[0]);
        return -1;
    }
    else
    {
        strcpy(file_name, argv[1]);
        strcpy(device_name, argv[2]);
    }
    if (argc == 4)
    {
        for (size_t i = 0; i < argc; i++)
        {
            char const *param = argv[i];
            if (param[0] != '-')
                continue;
            if (strcmp(param, "-256MB"))
                ddr_type = ddr_256;
            else if (strcmp(param, "-512MB"))
                ddr_type = ddr_512;
        }
        strcpy(file_name, argv[2]);
        strcpy(device_name, argv[3]);
    }

    file = fopen(file_name, "rb");
    if (file == NULL)
    {
        printf("can't open file %s\n", file_name);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("file %s size : %d\n", file_name, file_size);
    file_buf = (unsigned char *)malloc(file_size + BIN_OFFSET);
    if (file_buf == NULL)
    {
        printf("malloc error\n");
        return -1;
    }
    memset(file_buf, 0, file_size + BIN_OFFSET);
    fread(file_buf + BIN_OFFSET, 1, file_size, file);

    fclose(file);

    if (ddr_type == ddr_256)
    {
        printf("Board DDR size : %dMB\n", 256);
        memcpy(file_buf, imx6_256mb_ivtdcd_table, sizeof(imx6_256mb_ivtdcd_table));
    }
    else if (ddr_type == ddr_512)
    {
        printf("Board DDR size : %dMB\n", 512);
        memcpy(file_buf, imx6_512mb_ivtdcd_table, sizeof(imx6_512mb_ivtdcd_table));
    }

    sheel_cmd_buf = (unsigned char *)malloc(SHELL_CMD_SIZE);
    if (sheel_cmd_buf == NULL)
    {
        printf("malloc error in %d line\n", __LINE__);
        free(file_buf);
        return -1;
    }
    memset(sheel_cmd_buf, 0, SHELL_CMD_SIZE);

    sprintf(download_file_name, "%s_load.imx", strtok(file_name, "."));
    printf("delete old %s\n", download_file_name);

    memset(sheel_cmd_buf, 0, SHELL_CMD_SIZE);
    sprintf(sheel_cmd_buf, "rm -rf %s", download_file_name);
    system(sheel_cmd_buf);

    memset(sheel_cmd_buf, 0, SHELL_CMD_SIZE);
    printf("touch new %s file\n", download_file_name);
    sprintf(sheel_cmd_buf, "touch %s", download_file_name);
    system(sheel_cmd_buf);

    file = fopen(download_file_name, "wb");
    if (file == NULL)
    {
        printf("can't open file %s\n", download_file_name);
        free(file_buf);
        free(sheel_cmd_buf);
        return -1;
    }
    write_bytes = fwrite(file_buf, 1, file_size + BIN_OFFSET, file);
    if (write_bytes != (file_size + BIN_OFFSET))
    {
        printf("write buf to %s error\n", download_file_name);
        fclose(file);
        free(file_buf);
        free(sheel_cmd_buf);
        return -1;
    }
    fclose(file);
    free(file_buf);

    /*
     * param:
     *   iflag: 根据给定值进行读
     *   oflag: 根据给定值进行写
     *   if   : 要读取的标准输入
     *   of   : 要写入的标准输出
     *   bs   : 一次要写入或读取的最大字节数，覆盖ibs和obs
     *   seek : 在输出时调过N个obs的大小
     * value：
     *   dsync: 对数据采用同步I/O读取
     */
    sprintf(sheel_cmd_buf, "sudo dd iflag=dsync oflag=dsync if=%s of=%s bs=512 seek=2", download_file_name, device_name);
    printf("download %s to %s\n", download_file_name, device_name);
    system(sheel_cmd_buf);
    free(sheel_cmd_buf);

    return 0;
}
