#include <tstr.h>

/*
 * ���Ա���
 * ���� �ַ���ģ��,����ͨ��
 *
 * ϣ��, VS�ܶ�C�и����֧��
 */
int main_tstring(int argc, char* argv[])
{
	//���ڶ��� �����ڴ�
	tstr_t tstr = tstr_new("123");
	printf("str:%s, len:%d, size:%d.\n", tstr->str, tstr->len, tstr->size);

	tstr_append(tstr, 'A');
	printf("str:%s, len:%d, size:%d.\n", tstr->str, tstr->len, tstr->size);
	tstr_appends(tstr, "11111111111111111111111111111111111111111111111111111111111112���������\"���");
	printf("str:%s, len:%d, size:%d.\n", tstr->str, tstr->len, tstr->size);

	tstr_delete(tstr);

	//��ջ�ϲ���
	TSTR_NEW(ts);
	tstr_append(ts, 'w');
	printf("str:%s, len:%d, size:%d.\n", ts->str, ts->len, ts->size);
	tstr_appends(ts, "AN,but WAI!");
	printf("str:%s, len:%d, size:%d.\n", ts->str, ts->len, ts->size);
	TSTR_DELETE(ts);

	system("pause");
	return 0;
}