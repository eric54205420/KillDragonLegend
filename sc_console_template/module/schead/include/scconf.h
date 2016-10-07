#ifndef _H_SIMPLEC_SCCONF
#define _H_SIMPLEC_SCCONF

#include <schead.h>

/*
 * �����������ļ���ȡ�ӿ�,
 * д����,��ȡ����,��Ҫָ�����õ�·��
 * 
 * ���ù��� , ���� php ��������.
 * ����, �ļ��п�����������:

	 $heoo = "Hello World\n";

	 $yexu = "\"�����\",
	 �Һܺ�.лл!";

	 $end = "coding future 123 runing, ";
 
 * 
 * �������ͨ��, �����ļ���ȡ����.sc_get("heoo") = > "���!"
 */

// ����Ĳ���ȫ����, ���ô������
typedef void * sconf_t;

/*
 * ͨ���ƶ�����·���������������, ʧ�ܷ���NULL 
 * path	: ��������·��
 *		: ���ؽ���������ö��� 
 */
extern sconf_t sconf_new(const char * path);

/*
 * ����sconf_new���صĶ���
 */
extern void sconf_delete(sconf_t conf);

/*
 * �õ������о�������
 * conf	: sconf_new ���ص����ö���
 * key	: �����ֵ
 *		: �ɹ��õ��������õĴ�, ʧ�ܻ򲻴��ڷ���NULL
 */
extern const char * sconf_get(sconf_t conf, const char * key);

// ������ϵͳ --- ��������·��
#define _STR_MCCONF_PATH "./main/config/config.ini"

/*
 * ����������ϵͳ, ֻ����ϵͳ������ʱ��ִ��һ��
 *		: ���ش����õ������ö���
 */
extern sconf_t mconf_start(void);

/*
 * �õ������ö��������õ�����, �����ڳ�������ʱ����ִ�� mconf_new
 * key	: �����õ�keyֵ
 *		: �õ������������õ�����
 */
extern const char * mconf_get(const char * key);

#endif // !_H_SIMPLEC_SCCONF