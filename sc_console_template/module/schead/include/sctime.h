#ifndef _H_SIMPLEC_SCTIMEUTIL
#define _H_SIMPLEC_SCTIMEUTIL

#include <time.h>
#include <stdbool.h>

// ΪVisual Studio����һЩ��linux������˼·
#if defined(_MSC_VER)

#include <Windows.h>

/*
 * ���ص�ǰ�õ���ʱ��ṹ��, �߷�linux�ϵ���
 * pt	: const time_t * , �����ʱ���ָ��
 * ptm	: struct tm * , �����ʱ��ṹ��
 *		: ���� ptm ֵ
 */
#define localtime_r(pt, ptm) localtime_s(ptm, pt), ptm

/*
 * Linux sys/time.h �л�ȡʱ�亯����Windows��һ����ֲʵ��
 * tv	:	���ؽ������������΢����
 * tz	:	������ʱ��,��window���������û���ò�����
 * 		:   Ĭ�Ϸ���0
 */
extern int gettimeofday(struct timeval * tv, void * tz);

#endif


// ����ÿ���ǿ�ʼΪ 0ʱ0��0��
#define _INT_MINSECOND		(60)
#define _INT_HOURSECOND		(3600)
#define _INT_DAYSECOND		(24UL*_INT_HOURSECOND)
#define _INT_DAYSTART		(8UL*_INT_HOURSECOND)
// ����ÿ���µĿ�ʼʱ��
#define _INT_DAYNEWSTART	(0UL*_INT_HOURSECOND + 0*_INT_MINSECOND + 0)

// struct tm �� tm_year, tm_mon �õ�ƫ����
#define _INT_YEAROFFSET		(1900)
#define _INT_MONOFFSET		(1)

// ����ʱ�䴮����
#define _INT_STULEN			(32)
typedef char stime_t[_INT_STULEN];

/*
 * �� [2016-7-10 21:22:34] ��ʽ�ַ���ת��ʱ���
 * tstr	: ʱ�䴮�ָ���ֻ���ǵ��ֽڵ�.
 * pt	: ���صõ���ʱ���
 * otm	: ���صõ���ʱ��ṹ��
 *		: ��������ַ���ת�ɵ�ʱ���, -1��ʾ����ʧ��
 */
extern bool stu_gettime(stime_t tstr, time_t * pt, struct tm * otm);

/*
 * �жϵ�ǰʱ����Ƿ���ͬһ���.
 * lt : �ж�ʱ��һ
 * rt : �ж�ʱ���
 *    : ����true��ʾ��ͬһ��, ����false��ʾ����
 */
extern bool stu_tisday(time_t lt, time_t rt);

/*
 * �жϵ�ǰʱ����Ƿ���ͬһ�ܵ�.
 * lt : �ж�ʱ��һ
 * rt : �ж�ʱ���
 *    : ����true��ʾ��ͬһ��, ����false��ʾ����
 */
extern bool stu_tisweek(time_t lt, time_t rt);

/*
 * ��ʱ���ת��ʱ�䴮 [2016-7-10 22:38:34]
 * nt	: ��ǰ��ת��ʱ���
 * tstr	: �����ת��ʱ���λ��
 *		: ���ش���tstr���׵�ַ
 */
extern char * stu_gettstr(time_t nt, stime_t tstr);

/*
 * �õ���ǰʱ��� [2016-7-10 22:38:34]
 * tstr	: �����ת��ʱ���λ��
 *		: ���ش���tstr���׵�ַ
 */
extern char * stu_getntstr(stime_t tstr);

/*
 * �жϵ�ǰʱ�䴮�Ƿ���ͬһ���.
 * ls : �ж�ʱ��һ
 * rs : �ж�ʱ���
 *    : ����true��ʾ��ͬһ��, ����false��ʾ����
 */
extern bool stu_sisday(stime_t ls, stime_t rs);

/*
 * �жϵ�ǰʱ�䴮�Ƿ���ͬһ�ܵ�.
 * ls : �ж�ʱ��һ
 * rs : �ж�ʱ���
 *    : ����true��ʾ��ͬһ��, ����false��ʾ����
 */
extern bool stu_sisweek(stime_t ls, stime_t rs);

#endif // !_H_SIMPLEC_SCTIMEUTIL