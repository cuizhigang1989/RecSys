#ifndef _COMMONTOOLS_H_
#define _COMMONTOOLS_H_
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>

const uint32_t  MAX_FIELD_NUM = 128;
const uint32_t  MAX_STRING_LENGTH= 1024;

/*
*	get_file_size ����ļ���С
*	@param fp [in/out]	�ļ�������Ѿ��򿪣������Ὣָ�������ļ�ͷ
*	@return �ļ���С
*
*/
uint64_t get_file_size(FILE * fp);

/*
*	count_char �ַ�����ĳ�ַ�������
*	@param buf [in] �ַ�������NULL
*	@param ch [in] �ַ�
*	@return	�ַ����ַ����г��ֵĴ���
*/
int count_char(const char * buf,const char ch);

/*
*     get_md5_64 �����ַ�����md5ֵ��64bit
*     @param str [in] �ַ���
*     @return md5ֵ
*
*/
uint64_t get_md5_64(const char * str);

/*
*     split_string �з��ַ�����Դ�ַ����ᱻ\0�ֿ�
*     @param chr [in] �з��ַ�
*     @param src [in] �з�Դ�ַ���
*     @param dstlimit [in] �зֽ������
*     @param dstlist [out] �зֽ��
*     @param dstnum [out] �зֽ�����������Ϊdstlimit
*/
int split_string(char chr, char * src,uint32_t dstlimit,char ** dstlist,uint32_t * dstnum);

/**
 *�и��ַ���
 *@param  original    [in]    ��Ҫ�зֵ��ַ���
 *@param  separator    [in]    �ָ���
 *@param  col_limit   [in]    ����������Ŀ��,0����ȫ����
 *@param  result  [out]   ����б�
 **/
void split_string(const std::string& original, char separator, uint32_t col_limit, std::vector<std::string>* result);

/**
 *�и��ַ���
 *@param  original    [in]    ��Ҫ�зֵ��ַ���
 *@param  separator    [in]    �ָ���
 *@param  result  [out]   ����б�
 **/
void split_string(const std::string& original, char separator, std::vector<std::string>* result);

int fetch_time(char * buf,uint32_t bufsize);

char * ltrim(char * ptr,const char * ptr_end);
char * rtrim(char * ptr,const char * ptr_start);

/*
*     get_cat_high ���һ����ҵ������Լ�����
*     @return category��һ������
*/
uint32_t get_cat_high(uint32_t category);

std::string get_full_path(const char* dir, const char* filename);

#define ADR_LOG(log_out,log_type,...) {\
char timebuf[1000];\
fetch_time(timebuf,1000);\
fprintf(log_out,"%s\4%s\4",#log_type,timebuf);\
fprintf(log_out,__VA_ARGS__);\
}

#define ADR_ACCESS(...) ADR_LOG(stdout,LM_ACCESS, __VA_ARGS__)
#define ADR_WARNING(...) ADR_LOG(stderr,LM_WARNING, __VA_ARGS__)
#define ADR_INFO(...) ADR_LOG(stderr,LM_INFO, __VA_ARGS__)
#define ADR_ERROR(...) ADR_LOG(stderr,LM_ERROR, __VA_ARGS__)

/*�����ļ�����*/
#define ERROR_LOADFILE_FILE (-0x20001)
/*�����ڴ�������*/
#define ERROR_LOADFILE_MALLOC (-0x20002)
/*�����ļ���ȡ����*/
#define ERROR_LOADFILE_READ (-0x20003)
/*�����ļ���С���󣬷�itemtype������*/
#define ERROR_LOADFILE_SIZE (-0x20004)

/**
 * �����ļ���������
 * @param[in] filename �ļ���
 * @param[out] buf ���ػ�������������ڴ�
 * @return >=0 ��ȡ���� other ʧ��
 */
template <class itemtype>
int64_t load_file(const char * filename,itemtype * & buf)
{
        //���ļ�
        FILE * fp=fopen(filename,"r");///�ļ����
        if(fp == NULL){
                return ERROR_LOADFILE_FILE;
        }
        //��ȡ�ļ���С
        int64_t file_size = get_file_size(fp);///�ļ���С
        if(file_size <= 0)
                return ERROR_LOADFILE_FILE;
        //����ļ���С�Ƿ����Ҫ��
        if(file_size % sizeof(itemtype) != 0){
                fclose(fp);
                return ERROR_LOADFILE_SIZE;
        }
        int64_t item_num = file_size / sizeof(itemtype);///����

        //�����ڴ�
        buf = (itemtype*)malloc(file_size);
        if(buf == NULL){
                fclose(fp);
                return ERROR_LOADFILE_MALLOC;
        }
        //�����ļ�
        int64_t ret = fread(buf,sizeof(itemtype),file_size/sizeof(itemtype),fp);///��������ֵ
        fclose(fp);
        //�������С������ȷ�򷵻ض�������
        if(ret == item_num)
                return item_num;
        //�������������ͷ��ڴ�
        free(buf);
        buf = NULL;
        return ERROR_LOADFILE_READ;
}

#endif
