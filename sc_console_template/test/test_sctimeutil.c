#include <schead.h>

/*
 * ������Ҫ���� sctimeutil.h �ӿ��ܷ�����ʹ��
 * 
 */
int main_sctimeutil(int argc, char * argv[]) {
	stime_t tstr;
	bool rt;
	time_t t;
	struct tm st;

	// ע������ȴ�����
	INIT_PAUSE();

	printf("Now begin time is : %s\n", stu_getntstr(tstr));

	rt = stu_gettime("2016-07-13 20:04:07", &t, &st);
	printf("rt = %d, t = %lld, sec = %d\n", rt, t, st.tm_sec);

	// ����ת��
	rt = stu_gettime("2016��7��13��20:56:53", &t, &st);
	printf("rt = %d, t = %lld, year = %d, sec = %d\n", rt, t, st.tm_year, st.tm_sec);

	rt = stu_sisday("2016��7��13��20:57:56", "2016��08��13��20:58:00");
	printf("rt = %d\n", rt);

	printf("Now e n d time is : %s\n", stu_getntstr(tstr));
	return 0;
}