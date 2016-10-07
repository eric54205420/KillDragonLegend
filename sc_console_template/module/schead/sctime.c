#include <sctime.h>

// ΪVisual Studio����һЩ��linux������˼·
#if defined(_MSC_VER)

/*
 * Linux sys/time.h �л�ȡʱ�亯����Windows��һ����ֲʵ��
 * tv	:	���ؽ������������΢����
 * tz	:	������ʱ��,��window���������û���ò�����
 * 		:   Ĭ�Ϸ���0
 */
int
gettimeofday(struct timeval * tv, void* tz) {
	struct tm st;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	st.tm_year = wtm.wYear - _INT_YEAROFFSET;
	st.tm_mon = wtm.wMonth - _INT_MONOFFSET; // window�ļ�������Щ
	st.tm_mday = wtm.wDay;
	st.tm_hour = wtm.wHour;
	st.tm_min = wtm.wMinute;
	st.tm_sec = wtm.wSecond;
	st.tm_isdst = -1; // ����������ʱ

	tv->tv_sec = (long)mktime(&st); // 32λʹ������ǿת
	tv->tv_usec = wtm.wMilliseconds * 1000; // ����ת��΢��

	return 0;
}

#endif

// ��ʱ�䴮����ȡ����������ʱ����
static bool _stu_gettm(stime_t tstr, struct tm * otm) {
	int * py, * es;
	char c;
	int sum;

	if ((!tstr) || !(c = *tstr) || c < '0' || c > '9')
		return false;

	py = &otm->tm_year;
	es = &otm->tm_sec;
	sum = 0;
	while ((c = *tstr) && py >= es) {
		if (c >= '0' && c <= '9') {
			sum = 10 * sum + c - '0';
			++tstr;
			continue;
		}

		*py-- = sum;
		sum = 0;

		// ȥ�������ַ�, һֱ�ҵ���һ������
		while ((c = *++tstr) && (c<'0' || c>'9'))
			;
	}
	// �Ƿ�, ����������
	if (py != es)
		return false;

	*es = sum; // �������������
	return true;
}

/*
 * �� [2016-7-10 21:22:34] ��ʽ�ַ���ת��ʱ���
 * tstr	: ʱ�䴮�ָ���ֻ���ǵ��ֽڵ�.
 * pt	: ���صõ���ʱ���
 * otm	: ���صõ���ʱ��ṹ��
 *		: ��������ַ���ת�ɵ�ʱ���, -1��ʾ����ʧ��
 */
bool
stu_gettime(stime_t tstr, time_t * pt, struct tm * otm) {
	time_t t;
	struct tm st;

	// �ȸ�Ч������������ʱ����
	if (!_stu_gettm(tstr, &st))
		return false;

	st.tm_year -= _INT_YEAROFFSET;
	st.tm_mon -= _INT_MONOFFSET;
	// �õ�ʱ���, ʧ�ܷ���false
	if ((t = mktime(&st)) == -1)
		return false;

	// �������ս��
	if (pt)
		*pt = t;
	if (otm)
		*otm = st;

	return true;
}

/*
 * �жϵ�ǰʱ����Ƿ���ͬһ���.
 * lt : �ж�ʱ��һ
 * rt : �ж�ʱ���
 *    : ����true��ʾ��ͬһ��, ����false��ʾ����
 */
bool
stu_tisday(time_t lt, time_t rt) {
	// �õ��Ǹ��Եڼ����
	lt = (lt + _INT_DAYSTART - _INT_DAYNEWSTART) / _INT_DAYSECOND;
	rt = (rt + _INT_DAYSTART - _INT_DAYNEWSTART) / _INT_DAYSECOND;
	return lt == rt;
}

/*
 * �жϵ�ǰʱ����Ƿ���ͬһ�ܵ�.
 * lt : �ж�ʱ��һ
 * rt : �ж�ʱ���
 *    : ����true��ʾ��ͬһ��, ����false��ʾ����
 */
bool
stu_tisweek(time_t lt, time_t rt) {
	time_t mt;
	struct tm st;

	lt -= _INT_DAYNEWSTART;
	rt -= _INT_DAYNEWSTART;

	if (lt < rt) { //�õ����ʱ��, ������lt��
		mt = lt;
		lt = rt;
		rt = mt;
	}

	// �õ�lt ��ʾ�ĵ�ǰʱ��
	localtime_r(&lt, &st);

	// �õ���ǰʱ�䵽��һ����ʱ���
	st.tm_wday = 0 == st.tm_wday ? 7 : st.tm_wday;
	mt = (st.tm_wday - 1) * _INT_DAYSECOND + st.tm_hour * _INT_HOURSECOND
		+ st.tm_min * _INT_MINSECOND + st.tm_sec;

	// [min, lt], lt = max(lt, rt) �ͱ�ʾ��ͬһ����
	return rt >= lt - mt;
}

/*
 * ��ʱ���ת��ʱ�䴮 [2016-7-10 22:38:34]
 * nt	: ��ǰ��ת��ʱ���
 * tstr	: �����ת��ʱ���λ��
 *		: ���ش���tstr���׵�ַ
 */
char * stu_gettstr(time_t nt, stime_t tstr) {
	struct tm st;
	localtime_r(&nt, &st);
	strftime(tstr, sizeof(stime_t), "%F %X", &st);
	return tstr;
}

/*
 * �õ���ǰʱ��� [2016-7-10 22:38:34]
 * tstr	: �����ת��ʱ���λ��
 *		: ���ش���tstr���׵�ַ
 */
char * stu_getntstr(stime_t tstr) {
	return stu_gettstr(time(NULL), tstr);
}

/*
 * �жϵ�ǰʱ����Ƿ���ͬһ���.
 * ls : �ж�ʱ��һ
 * rs : �ж�ʱ���
 *    : ����true��ʾ��ͬһ��, ����false��ʾ����
 */
bool
stu_sisday(stime_t ls, stime_t rs) {
	time_t lt, rt;
	// ����ʧ��ֱ�ӷ��ؽ��
	if (!stu_gettime(ls, &lt, NULL) || !stu_gettime(rs, &rt, NULL))
		return false;

	return stu_tisday(lt, rt);
}

/*
 * �жϵ�ǰʱ����Ƿ���ͬһ�ܵ�.�����Ż�
 * ls : �ж�ʱ��һ
 * rs : �ж�ʱ���
 *    : ����true��ʾ��ͬһ��, ����false��ʾ����
 */
bool
stu_sisweek(stime_t ls, stime_t rs) {
	time_t lt, rt;
	// ����ʧ��ֱ�ӷ��ؽ��
	if (!stu_gettime(ls, &lt, NULL) || !stu_gettime(rs, &rt, NULL))
		return false;

	return stu_tisweek(lt, rt);
}