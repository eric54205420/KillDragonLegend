#ifndef _H_SIMPLEC_BRAVEDRAGON
#define _H_SIMPLEC_BRAVEDRAGON

#include <schead.h>

// ������˵��������
extern void bd_main(void);

// ����ı�str, ÿ��ӡһ���ַ�ͣ��tn����, ���ͣ��tb����
extern void bd_animates(const char * str, int tn, int tb);

// �������������Ϣ, errs:������Ϣ, title����, levs����ȼ�, iscls�Ƿ����ͷ������
extern void bd_errs(const char * errs, const char * title,
					const char * levs, bool iscls);

// ���Ƹ�ǿ�Ĵ�ӡ����
extern void bd_animatesa(const char * str, const char * title, 
						 bool isclsh, bool isclsb, int tn, int tb);

// �����ӡ����, �������ű���
extern void bd_butis(const char * str, const char * title);

// ��ӡ��������Ϣ
extern void bd_mms(char str[], int len);

#endif // !_H_SIMPLEC_BRAVEDRAGON