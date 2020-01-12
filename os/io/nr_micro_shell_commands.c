/**
 * @file      nr_micro_shell_commands.c
 * @author    Nrush
 * @version   V0.1
 * @date      28 Oct 2019
 * *****************************************************************************
 * @attention
 * 
 * MIT License
 * 
 * Copyright (C) 2019 Nrush. or its affiliates.  All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Includes ------------------------------------------------------------------*/

#include "nr_micro_shell.h"
#include "string.h"
#include "ctype.h"
#include "ff.h"
#include "xprintf.h"
#include "FreeRTOSConfig.h"
//��ʾ��ǰĿ¼
#define PWD_LEN	20
static TCHAR cur_dir[PWD_LEN]="/";

#if 0
//�ò����ˣ��Ѿ�ʵ�ֹ���
//�Կո�Ϊ��־���з�����
//argv����Ҫ�����ڴ�,���Ե����������Ҫ�ͷ�������ڴ�
void shell_command_parse(char *cmd,char **argv,int *count){
	char *p=cmd;
	int i=0;
	int slen=0;
	argv = malloc((3*sizeof(unsigned int *));
	while(*p){
		if(*p == ' '){
			slen = p-cmd;
			argv[i]=malloc(slen+1);
			memcpy(argv[i],cmd,slen);
			argv[i][slen]=0;
			i++;
			cmd=p+1;
			p=cmd;
		}
		p++;
	}
	//�������һ������
	slen = p-cmd;
	argv[i]=malloc(slen+1);
	memcpy(argv[i],cmd,slen);
	argv[i][slen]=0;
	*count=i+1;
}


#endif

void shell_mkfs_cmd(char argc,char *argv){
	extern FATFS *fs;
	//extern const int work_buff_len;
	/*!!!!!!!!!!attetion!!!!!!!!!!*/
	
	
	
	
	//�����д���кܴ��������extern char work[512];��д��������������������ģ���work�������Լ�������һ��work?\
	������Ŀǰshell taskֻ��128byte��ջ��˵�����û�з���ջ������Ǿ��������˱��ģ���work
	extern char work[512];
	FRESULT fr;
	fr = f_mkfs("FLASH", 0, work, sizeof work);
	if(fr == FR_OK){shell_printf("mk fs success\n");}
	else {configASSERT(!fr);}
	fr = f_mount(fs, "FLASH",1);
	if(fr == FR_OK){shell_printf("mount fs success\n");}
	else {configASSERT(!fr);}
}

void shell_mkdir_cmd(char argc,char *argv){
	FRESULT fr;
	if(argc > 1)
	{
		//����Ŀ¼
			fr = f_mkdir(&argv[argv[1]]);
			if(fr == FR_EXIST){shell_printf("directory has exist...");return;}
			configASSERT(!fr);
	}
	else{
		shell_printf("useage: mkdir [path]\r\n");
	}	
}
void shell_pwd_cmd(char argc,char *argv){
		FRESULT fr;
    fr = f_getcwd(cur_dir, PWD_LEN);  /* Get current directory path */
		configASSERT(!fr);
		shell_printf("%s\n",cur_dir);
}
extern shell_st nr_shell;//�ı�·����
void shell_cd_cmd(char argc,char *argv){
	FRESULT fr;
	if(argc > 1)
	{
		//�л�Ŀ¼
			fr = f_chdir(&argv[argv[1]]);
			if(fr == FR_NO_PATH){shell_printf("directory not exist...");return;}
			configASSERT(!fr);
			//���µ�ǰĿ¼
			fr = f_getcwd(cur_dir, PWD_LEN);  /* Get current directory path */(0,0);
			configASSERT(!fr);
			//ͬ��Ŀ¼��
			strcpy(nr_shell.user_name+strlen(NR_SHELL_USER_NAME),cur_dir);
			fr = strlen(nr_shell.user_name);
			nr_shell.user_name[fr]='~';
			nr_shell.user_name[fr+1]=0;
	}
	else{
		shell_printf("useage: cd [path]\r\n");
	}	
}
FRESULT scan_files (char* path){
    FRESULT res;
    UINT i;
		DIR * dir = (DIR*)malloc(sizeof(DIR));
		FILINFO* fno = (FILINFO*)malloc(sizeof(FILINFO));		/* Pointer to file information to return */

    res = f_opendir(dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(dir, fno);                   /* Read a directory item */
            if (res != FR_OK || fno->fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno->fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                xsprintf(&path[i], "/%s", fno->fname);
								shell_printf("%s  ",&path[i]);		//��ʾ��Ŀ¼
               // res = scan_files(path);                    /* Enter the directory */
               // if (res != FR_OK) break;
                //path[i] = 0;
            } else {                                       /* It is a file. */
								if(!strcmp("/",path)){
										shell_printf("%s  ",fno->fname);
								}
								else{
										//shell_printf("%s/%s\n", path, fno->fname);
										shell_printf("%s  ",fno->fname);
								}
            }
        }
        f_closedir(dir);
    }
		free(fno);
		free(dir);
    return res;
}


//mk command
void shell_mk_cmd(char argc,char *argv){
	FIL fp;
	FRESULT fr;
	if(argc > 1)
	{
		//�����ļ�
			fr = f_open(&fp, &argv[argv[1]], FA_CREATE_NEW);
			if(fr == FR_EXIST){shell_printf("file has exist...");f_close(&fp);return;}
			configASSERT(!fr);
			f_close(&fp);
	}
	else{
		shell_printf("useage: mk [filename]\r\n");
	}	
}
//rm command
void shell_rm_cmd(char argc,char *argv){
	FRESULT fr;
	if(argc > 1){
		//�ṩ·��
		fr = f_unlink(&argv[argv[1]]);
		if(fr == FR_NO_FILE){shell_printf("no such file or directory\n");return;}
		configASSERT(!fr);
	}
	else{
		shell_printf("useage: rm [path]\r\n");
	}
}
//cat file context command
void shell_cat_cmd(char argc,char *argv){
#define buff_len 32
	FIL fp;
	unsigned int i = 1;
	FRESULT fr;
	char *buff;
	if(argc > 1)
	{
			//���ļ�
			fr = f_open(&fp, &argv[argv[1]],FA_OPEN_EXISTING|FA_READ);
			if(fr == FR_NO_FILE){
				f_close(&fp);
				shell_printf("no such file\n");
				return;
			}
			if(fr !=FR_OK){
				f_close(&fp);	
				shell_printf(" %d \n",fr);				
				configASSERT(!fr);
				return;
			}
			buff = malloc(buff_len);
			while(0 != f_gets(buff,buff_len,&fp)){
					shell_printf("%s",buff);
			}
			shell_printf("\n");
			f_close(&fp);
			free(buff);
	}
	else{
		shell_printf("useage: cat [filename]\r\n");
	}	
	
}
//echo command  write file command
void shell_echo_cmd(char argc,char *argv){
		FIL fp;
	 unsigned int i = 0,j=0;
	int slen =0;
	FRESULT fr;
	if(argc == 3)
	{
			//���ļ�
			fr = f_open(&fp, &argv[argv[1]],FA_OPEN_APPEND|FA_WRITE);
			if(fr == FR_NO_FILE){
				f_close(&fp);
				//û�����½�
				fr = f_open(&fp, &argv[argv[1]], FA_CREATE_NEW | FA_WRITE);
			}
			if(fr !=FR_OK){
				f_close(&fp);
				shell_printf("write error:%d\n",fr);
				return;
			}
			//f_puts(&(argv[argv[2]]),&fp);
			slen = strlen(&(argv[argv[2]]));
			f_write(&fp,&(argv[argv[2]]),slen,&i);
			shell_printf("%s write %d byte,context %s\n",&argv[argv[1]],i,&(argv[argv[2]]));
			f_close(&fp);
	}
	else if(argc==2){//��ͬ���½�һ���ļ������ߴ�һ���ļ����ǲ�׷������
		//���ļ�
			fr = f_open(&fp, &argv[argv[1]],FA_OPEN_APPEND|FA_WRITE);
			if(fr == FR_NO_FILE){
				f_close(&fp);
				//û�����½�
				fr = f_open(&fp, &argv[argv[1]], FA_CREATE_NEW | FA_WRITE);
			}
			if(fr !=FR_OK){
				f_close(&fp);
				shell_printf("write error:%d\n",fr);
				return;
			}
			f_close(&fp);
	}
	else{
		shell_printf("useage: echo [filename] <option>[string]\r\n");
	}		
}
/**
 * @brief ls command
 */
void shell_ls_cmd(char argc,char *argv)
{
	unsigned int i = 0;
	FRESULT fr;
	if(argc > 1)
	{
		if(!strcmp("cmd",&argv[argv[1]]))
		{
			
			for(i=0;static_cmd[i].cmd[0] != '\0';i++)
			{
				shell_printf(static_cmd[i].cmd);
				shell_printf("\r\n");
			}
		}
		else if(!strcmp("-v",&argv[argv[1]]))
		{
			shell_printf("ls version 1.0.\r\n");
		}
		else if(!strcmp("-h",&argv[argv[1]]))
		{
			shell_printf("useage: ls [options]\r\n");
			shell_printf("options: \r\n");
			shell_printf("\t -h \t: show help\r\n");
			shell_printf("\t -v \t: show version\r\n");
			shell_printf("\t cmd \t: show all commands\r\n");
		}
	}
	else{
		fr = f_getcwd(cur_dir, PWD_LEN);  /* Get current directory path */
		configASSERT(!fr);
    fr = scan_files(cur_dir);
		shell_printf("\n");
		configASSERT(!fr);
	}

}

/**
 * @brief test command
 */
void shell_test_cmd(char argc, char *argv)
{
	unsigned int i;
    shell_printf("test command:\r\n");
	for(i=0;i<argc;i++)
	{
		shell_printf("paras %d: %s\r\n",i,&(argv[argv[i]]));
	}
}


//��������
void shell_reboot_cmd(char argc, char *argv){
	extern void reboot();
	int i =1000;
	while(i--);//����ͣ��
	reboot();
}


//vi�༭������
void shell_vi_cmd(char argc, char *argv){
#define buff_len 256
	FIL fp;
	unsigned int i = 0;
	int slen =0;
	char c;
	FRESULT fr;
	if(argc == 2){
			//���ļ�
			fr = f_open(&fp, &argv[argv[1]],FA_OPEN_APPEND|FA_WRITE);
			if(fr == FR_NO_FILE){
				f_close(&fp);
				//û�����½�
				fr = f_open(&fp, &argv[argv[1]], FA_CREATE_NEW | FA_WRITE);
			}
			if(fr !=FR_OK){
				f_close(&fp);
				shell_printf("open file error:%d\n",fr);
				return;
			}
			//����༭ģʽ
			//Ӧ�ýӹ���������ֱ������esc����
			//f_puts(&(argv[argv[2]]),&fp);
		while(1){
				c = xgetc();//��ȡ1���ַ�
				if(c == NR_QUIT_VI_CHAR){//esc
						c = NR_SHELL_END_CHAR;
						slen+=nr_ansi.counter;
						ansi_get_char(c, &nr_ansi);
						f_puts(nr_ansi.current_line,&fp);
						ansi_clear_current_line(&nr_ansi);//д�����յ�ǰ��
						break;
				}
				if(c == NR_SHELL_END_CHAR){//����ǰ�ȼ�¼counter
					slen+=nr_ansi.counter;
				}
				if(ansi_get_char(c, &nr_ansi)==NR_SHELL_END_CHAR){//����
					f_puts(nr_ansi.current_line,&fp);
					f_puts("\r\n",&fp);//д�뻻��
					ansi_clear_current_line(&nr_ansi);//д�����յ�ǰ��
				}
			}
		f_sync(&fp);
		f_close(&fp);	
		shell_printf("write %d character\r\n",slen);
	}
	else{
		shell_printf("useage: vi [filename]\r\n");
	}
}


//run command
void shell_run_cmd(char argc,char *argv){
	FRESULT fr;
	if(argc > 1){
		shell_printf("wait for more...\r\n");
	}
	else{
		shell_printf("useage: run [filename]\r\n");
		shell_printf("run a script");
	}
}

/**
 * @brief command list
 */
const static_cmd_st static_cmd[] =
{
   {"ls",shell_ls_cmd},
   {"test",shell_test_cmd},
	 {"pwd",shell_pwd_cmd},
	 {"mk",shell_mk_cmd},
	 {"echo",shell_echo_cmd},
	 {"cat",shell_cat_cmd},
	 {"rm",shell_rm_cmd},
	 {"mkdir",shell_mkdir_cmd},
	 {"cd",shell_cd_cmd},
	 {"mkfs",shell_mkfs_cmd},
	 {"reboot",shell_reboot_cmd},
	 {"vi",shell_vi_cmd},
	 {"run",shell_run_cmd},
   {"\0",NULL}
};

shell_st nr_shell ={
        NR_SHELL_USER_NAME,
        static_cmd,
};

/******************* (C) COPYRIGHT 2019 Nrush *****END OF FILE*****************/
