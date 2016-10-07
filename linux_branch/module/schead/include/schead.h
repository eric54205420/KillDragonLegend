﻿#ifndef _H_SIMPLEC_SCHEAD
#define _H_SIMPLEC_SCHEAD

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <scalloc.h>
#include <sctime.h>

/*
 * error		=> 以后再说
 * 跨平台的丑陋从这里开始
 * __GNUC		=> linux 平台特殊操作
 * __MSC_VER	=> window 平台特殊操作
 */
#ifdef __GNUC__  // 下面是依赖GCC编译器实现

#include <unistd.h>
#include <sys/time.h>
#include <termio.h>

// 统一的程序睡眠宏, 单位是毫秒颗粒度
#define SLEEPMS(m) \
		usleep(m * 1000)

// 屏幕清除宏, 依赖系统脚本
#define CONSOLECLS() \
		system("printf '\ec'")

/*
 * 得到用户输入的一个字符
 *		: 返回得到字符
 */
extern int sh_getch(void);

#elif _MSC_VER // 下面是依赖Visual Studio编译器实现

#include <Windows.h>
#include <direct.h> 
#include <conio.h>

// window 删除目录宏
#define rmdir		_rmdir

// window 上用_getch 替代了getch, 这里为了让其回来
#define sh_getch	_getch

#define CONSOLECLS() \
		system("cls")

#define SLEEPMS(m) \
		Sleep(m)	

#else
	#error "error : Currently only supports the Visual Studio and GCC!"
#endif


 /*
  * 错误定义枚举 用于判断返回值状态的状态码 RT_*表示返回标志
  *	使用举例 :
  
    int flag = scconf_get("pursue");
	if(flag < RT_SuccessBase) {
		sclog_error("get config %s error! flag = %d.", "pursue", flag);
		exit(EXIT_FAILURE);
	}
  
  * 这里是内部 使用的通用返回值 标志. >=0 表示成功, <0 表示失败的情况
  */
typedef enum {
	RT_SuccessBase	= 00,				//结果正确的返回宏
	RT_ErrorBase	= -1,				//错误基类型, 所有错误都可用它, 在不清楚的情况下
	RT_ErrorParam	= -2,				//调用的参数错误
	RT_ErrorMalloc	= -3,				//内存分配错误
	RT_ErrorFopen	= -4,				//文件打开失败	
	RT_ErrorClose	= -5,				//文件描述符读取关闭, 读取完毕也会返回这个
} flag_e;

/*
 * 定义一些通用的函数指针帮助,主要用于基库的封装中
 * 有构造函数, 释放函数, 比较函数等
 */
typedef void *	(* new_f)();
typedef void	(* die_f)(void * node);
// cmp_f 最好 是 int cmp(const void * ln, const void * rn); 标准结构
typedef int		(* cmp_f)();
// 循环操作函数, arg 外部参数, node 内部节点
typedef flag_e	(* each_f)(void * node, void * arg);

/* 栈上辅助操作宏 */
#if !defined(_H_ARRAY_HELP)

// 获取数组长度,只能是数组类型或""字符串常量,后者包含'\0'
#define LEN(arr) (sizeof(arr) / sizeof(*(arr)))

 // 置空操作, v必须是个变量
#define BZERO(v) \
	memset(&(v), 0, sizeof(v))

/*
 * 比较两个结构体栈上内容是否相等,相等返回true,不等返回false
 * a	: 第一个结构体值
 * b	: 第二个结构体值
 *		: 相等返回true, 否则false
 */
#define STRUCTCMP(a, b) \
	(!memcmp(&a, &b, sizeof(a)))

#define _H_ARRAY_HELP
#endif

/* 比较大小辅助宏 */
#if !defined(_H_EQUAL)

// 浮点数据判断宏帮助, __开头表示不希望你使用的宏
#define __DIFF(x, y)				((x)-(y))					//两个表达式做差宏
#define __IF_X(x, z)				((x)<z && (x)>-z)			//判断宏,z必须是宏常量
#define EQ(x, y, c)					EQ_ZERO(__DIFF(x,y), c)		//判断x和y是否在误差范围内相等

// float判断定义的宏
#define _FLOAT_ZERO				(0.000001f)						//float 0的误差判断值
#define EQ_FLOAT_ZERO(x)		__IF_X(x, _FLOAT_ZERO)			//float 判断x是否为零是返回true
#define EQ_FLOAT(x, y)			EQ(x, y, _FLOAT_ZERO)			//判断表达式x与y是否相等

// double判断定义的宏
#define _DOUBLE_ZERO			(0.000000000001)				//double 0误差判断值
#define EQ_DOUBLE_ZERO(x)		__IF_X(x, _DOUBLE_ZERO)			//double 判断x是否为零是返回true
#define EQ_DOUBLE(x,y)			EQ(x, y, _DOUBLE_ZERO)			//判断表达式x与y是否相等

#define _H_EQUAL
#endif 

// 4.0 控制台错误输出控制
#if !defined(_H_CERR)

// 控制台打印错误信息, fmt必须是双引号括起来的宏
#define CERR(fmt, ...) \
    fprintf(stderr, "[%s:%s:%d][error %d:%s]" fmt "\n",\
         __FILE__, __func__, __LINE__, errno, strerror(errno), ##__VA_ARGS__)

// 控制台打印错误信息并退出, t同样fmt必须是 ""括起来的字符串常量
#define CERR_EXIT(fmt,...) \
	CERR(fmt, ##__VA_ARGS__), exit(EXIT_FAILURE)

// 执行后检测,如果有错误直接退出
#define IF_CHECK(code) \
	if((code) < 0) \
		CERR_EXIT(#code)

#define _H_CERR
#endif

// scanf 健壮的多次输入宏
#ifndef SAFETY_SCANF
#define _STR_SAFETY_SCANF "Input error, please according to the prompt!"
#define SAFETY_SCANF(scanf_code, ...) \
	while(printf(__VA_ARGS__), scanf_code){\
		while('\n' != getchar())\
			;\
		puts(_STR_SAFETY_SCANF);\
	}\
	while('\n' != getchar())
#endif /*!SAFETY_SCANF*/

// 简单的time帮助宏
#ifndef TIME_PRINT
#define _STR_TIME_PRINT "The current code block running time:%lf seconds\n"
#define TIME_PRINT(code) \
	do {\
		clock_t __st, __et;\
		__st=clock();\
		code\
		__et=clock();\
		printf(_STR_TIME_PRINT, (0.0 + __et - __st) / CLOCKS_PER_SEC);\
	} while(0)
#endif // !TIME_PRINT

/*
 * 这里是一个 在 DEBUG 模式下的测试宏
 *
 * 用法 :
 * DEBUG_CODE({
 *		puts("debug start...");
 * });
 */
#ifndef DEBUG_CODE
# ifdef _DEBUG
#	define DEBUG_CODE(code) \
		do code while(0)
# else
#	define DEBUG_CODE(code) 
# endif	//	! _DEBUG
#endif	//	! DEBUG_CODE

// 等待的宏 是个单线程没有加锁 | "请按任意键继续. . ."
#define _STR_PAUSEMSG "Press any key to continue . . ."
extern void sh_pause(void);
#ifndef INIT_PAUSE

#	ifdef _DEBUG
#		define INIT_PAUSE() atexit(sh_pause)
#	else
#		define INIT_PAUSE()	/* 别说了,都重新开始吧 */
#	endif

#endif // !INIT_PAUSE

/*
 * c 如果是空白字符返回 true, 否则返回false
 * c : 必须是 int 值,最好是 char 范围
 */
#define sh_isspace(c) \
	((c==' ')||(c>='\t'&&c<='\r'))

//12.0 判断是大端序还是小端序,大端序返回true
extern bool sh_isbig(void);

/**
*	sh_free - 简单的释放内存函数,对free再封装了一下
**可以避免野指针
**pobj:指向待释放内存的指针(void*)
**/
extern void sh_free(void ** pobj);

#endif// ! _H_SIMPLEC_SCHEAD
