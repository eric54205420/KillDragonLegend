#ifndef _H_SIMPLE_SCLOG
#define _H_SIMPLE_SCLOG

#include "schead.h"

//-------------------------------------------------------------------------------------------|
// ��һ���� ���õĲ�����
//-------------------------------------------------------------------------------------------|

//
//������־�з�,��Ҫ�õ������������crontab , �����´����Լ�дһ��������.
#define _INT_LITTLE				(64)		//����ʱ���IP����
#define _INT_LOG				(1024<<3)	//���8k��־

#define _STR_SCLOG_DIR			"logs"		//��־���·��Ŀ¼,�������Ҫ��Ҫ���ó�""
#define _STR_SCLOG_LOG			"sc.log"	//��ͨlog��־ DEBUG,INFO,NOTICE,WARNING,FATAL�������
#define _STR_SCLOG_WFLOG		"sc.log.wf"	//����Ƚϸߵ���־��� FATAL��WARNING

#define _STR_SCLOG_FATAL		"FATAL"	    //����,���ʹ��
#define _STR_SCLOG_WARNING		"WARNING"	//����,ǰ��ʹ�ô���,�����	
#define _STR_SCLOG_NOTICE		"NOTICE"	//ϵͳʹ��,һ����һ���������,ʹ�������־
#define _STR_SCLOG_INFO			"INFO"		//��ͨ����־��ӡ
#define _STR_SCLOG_TRACE		"TRACE"		//�����õ���־��ǵ�ǰ��־�Ŀ�ʼ�ͽ���
#define _STR_SCLOG_DEBUG		"DEBUG"	    //�����õ���־��ӡ,�ڷ�������Щ��־�ᱻ�����

/**
*	fstr : Ϊ��ʶ�� ���� _STR_SCLOG_FATAL, ������˫�����������Ĵ�
**
** ƴ��һ�� printf �����ʽ��
**/
#define SCLOG_PUTS(fstr) "[" fstr "][%s:%d:%s][logid:%u][reqip:%s][mod:%s]"

/**
*	fstr : ֻ���� _STR_SCLOG_* ��ͷ�ĺ�
**	fmt	 : ������""�������ĺ�.��������ĸ�ʽ��
**	...  : ��ӳfmt������
**
**  ƴ������ʹ�õĺ�,Ϊsl_printf ����һ��ģ��
**/
#define SCLOG_PRINTF(fstr, fmt, ...) \
	sl_printf(SCLOG_PUTS(fstr) fmt "\n", __FILE__, __LINE__, __func__, \
		sl_getlogid(), sl_getreqip(), sl_getmod(), ##__VA_ARGS__)


/**
*	FATAL... ��־��ӡ��
**	fmt	: ����ĸ�ʽ��,��Ҫ""��������
**	...	: ����Ĳ���,������fmt
**/
#define SL_FATAL(fmt,	...)	SCLOG_PRINTF(_STR_SCLOG_FATAL,		fmt, ##__VA_ARGS__)
#define SL_WARNING(fmt, ...)	SCLOG_PRINTF(_STR_SCLOG_WARNING,	fmt, ##__VA_ARGS__)
#define SL_NOTICE(fmt,	...)	SCLOG_PRINTF(_STR_SCLOG_NOTICE,		fmt, ##__VA_ARGS__)
#define SL_INFO(fmt,	...)	SCLOG_PRINTF(_STR_SCLOG_INFO,		fmt, ##__VA_ARGS__)

// ����״̬��,�ر�SL_DEBUG ��,��Ҫ���±���,û�иĳ�����ʱ���ж�,��������ҪΧ�Ƶ������ֶ������
#if defined(_DEBUG)
#	define SL_TRACE(fmt, ...)   SCLOG_PRINTF(_STR_SCLOG_TRACE,		fmt, ##__VA_ARGS__)
#	define SL_DEBUG(fmt, ...)   SCLOG_PRINTF(_STR_SCLOG_DEBUG,		fmt, ##__VA_ARGS__)
#else
#	define SL_TRACE(fmt, ...)    /* �����ѵ�����123*/
#	define SL_DEBUG(fmt, ...)	 /* ������ */
#endif

//-------------------------------------------------------------------------------------------|
// �ڶ����� ����־��Ϣ�������get��set,������������Ϣ���ʵ��
//-------------------------------------------------------------------------------------------|

/**
*	�̵߳�˽�����ݳ�ʼ��
**
** mod   : ��ǰ�߳�����
** reqip : �����ip
** logid : �����Ψһ��ʶid, Ĭ��0
** return :	_RT_OK ��ʾ����,_RF_EM�ڴ�������
**/
extern int sl_init(const char mod[_INT_LITTLE], const char reqip[_INT_LITTLE], unsigned logid);

/**
*	��ȡ��־��Ϣ���Ψһ��logid
**/
unsigned sl_getlogid(void);

/**
*	��ȡ��־��Ϣ�������ip��,����NULL��ʾû�г�ʼ��
**/
const char* sl_getreqip(void);

/**
*	��ȡ��־��Ϣ�������,����NULL��ʾû�г�ʼ��
**/
const char* sl_getmod(void);


//-------------------------------------------------------------------------------------------|
// �������� ����־ϵͳ������������ӿڲ���
//-------------------------------------------------------------------------------------------|

/**
*	��־ϵͳ�״�ʹ�ó�ʼ��,�ҶԶ�ӳ��־�ļ�·��,����ָ��·��
**/
extern void sl_start(void);

/**
*	���������ϣ����ʹ��,��һ���ڲ��޶�������־�������.�Ƽ�ʹ����Ӧ�ĺ�
**��ӡ��Ӧ�������־����ӳ���ļ���.
**
**	format		: ������""���������ĺ�,��ͷ������ [FALTAL:%s]��˴���
**				[WARNING:%s]ǰ�˴���, [NOTICE:%s]ϵͳʹ��, [INFO:%s]��ͨ��Ϣ,
**				[DEBUG:%s] ����������
**
** return	: ����������ݳ���
**/
int sl_printf(const char* format, ...);

#endif // !_H_SIMPLE_SCLOG