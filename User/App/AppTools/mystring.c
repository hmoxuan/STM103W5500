/**
 * mystring.c file for STM32F103.
 * Describtion: 字符串相关操作函数集
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

#include "mystring.h"
#include "myportmacro.h"
#include <string.h>
#include <stdio.h>

static uint8 dest[100]="adajlngijnlfahogjlhs";
static uint8 src[20]="1234567891234657891";

/*字符串测试函数*/
void StringTest(void)
{
	strcat_oppo(dest,src);
	printf("%s\n",dest);
}


/**
  * @fun    strcat_oppo
  * @brief  将src字符串连接到dest字符串的前面
  * @param  dest:指向目的字符串的指针
  * @param  src	:指向源字符串的指针
  * @retval None
  */
void strcat_oppo(uint8 *dest,const uint8 *src)
{
	uint16	dlength,slength;
	dlength=(uint16)strlen((const char*)dest);
	slength=(uint16)strlen((const char*)src);
	StrInvertSeq(dlength,dest);
	StrInvertSeq(slength,(uint8 *)src);
	strcat((char*)dest,(const char*)src);
	StrInvertSeq(dlength+slength,dest);
}

/**
  * @fun     clcstr
  * @brief  字符串清空
  * @param  src:指向源字符串的指针
  * @param  length:字符串长度
  * @retval None
  */
void clcstr(uint8 *src,uint16 length)
{
	for(;length>0;length--)
		src[length-1]='\0';
}

/**
  * @fun     IntToStr
  * @brief  无符号整数转换为字符串
  * @param  num	:字符串长度
  * @param  str	:指向字符串的指针
  * @retval str	:返回字符串的指针
  */
uint8 *IntToStr(uint16 num,uint8 *str)
{
	uint8 i=0;
	while(num){
		str[i]=num%10+'0';
		num/=10;
		i++;
	}
	str[i]='\0';
	StrInvertSeq(i,str);
	return str;
}


/**
  * @fun    LongIntToStr
  * @brief  无符号长整数转换为字符串
  * @param  num	:字符串长度
  * @param  str	:指向字符串的指针
  * @retval str	:返回字符串的指针
  */
uint8 *LongIntToStr(uint32 num,uint8 *str)
{
	uint16 i=0;
	while(num){
		str[i]=num%10+'0';
		num/=10;
		i++;
	}
	str[i]='\0';
	StrInvertSeq(i,str);
	return str;
}

/**
  * @fun    IntToHexStr
  * @brief  无符号整数转换为16进制表示的字符串
  * @param  num	:要进行转换的数值变量
  * @param  str	:指向2字节字符串的指针
  * @retval str	:返回2字节字符串的指针
  *	@date	2014/6/6
  */
uint8 *IntToHexStr(uint8 num,uint8 *str)
{
	uint8	i;
	uint8	tdu[2];
	for(i=0,tdu[0]=num/0x10,tdu[1]=num%0x10;i<2;i++)
		str[i]=tdu[i]/10?tdu[i]%10+'A':tdu[i]%10+'0';
	return str;
}

/**
  * @fun     StrInvertSeq
  * @brief  字符串逆序调整，不涉及字符串结束标志
  * @param  strnum:字符串长度
  * @param  str	:指向字符串的指针
  * @retval None
  */
void StrInvertSeq(uint16 strnum,uint8 *str)
{
	uint8  c;
	uint16 i=0;
	uint16 num=strnum/2;
	for(strnum--;i<num;i++,strnum--){
		c=str[i];
		str[i]=str[strnum];
		str[strnum]=c;
	}
}

/**
  * @fun    BinToHex_Str
  * @brief  将二进制串转换为16进制的字符串
  * @param  dest:指向装载字符串的指针
  * @param  src	:指向二进制串的指针
  * @param  length:二进制串的长度
  * @retval dest:返回字符串的指针
  *	@date	2014/6/6
  */
uint8 *BinToHex_Str(uint8 *dest,const uint8 *src,uint16 length)
{
	uint16	i;
	for(i=0;i<length;i++)
		IntToHexStr(src[i],&dest[i*2]);
	return dest;
}

/**
  * @fun     strrchr
  * @brief  定位字符串中最后出现的指定字符
  * @param  str	:指向字符串的指针
  * @param  ch	:字符
  * @retval uint16:返回值
  */
char *_strrchr(const char *str,int ch)
{
	char *start=(char *)str;
	while(*str++); /*get the end of the string*/
	while(--str != start && *str != (char)ch); /**/
	if(*str == (char)ch)
		return((char *)str);
	return NULL;
}

/**
  * @fun    strncpy
  * @brief  复制num个字符串到dest
  * @param  dest:指向装载字符串的指针
  * @param  src	:指向源字符串的指针
  * @param  length:字符串的长度
  * @retval dest:返回字符串的指针
  *	@date	2014/7/19
  */
uint8 *_strncpy(uint8 *dest,const uint8 *src,uint16 length)
{
	uint16	i;
	for(i=0;i<length;i++)	dest[i]=src[i];
	return dest;
}

