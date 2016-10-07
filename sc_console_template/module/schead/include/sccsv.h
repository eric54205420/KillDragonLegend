#ifndef _H_SIMPLEC_SCCSV
#define _H_SIMPLEC_SCCSV

/*
 *  ������һ������ csv �ļ��� �򵥽�����.
 * ���ܹ����������з��ļ�����,������������.
 */
typedef struct sccsv {		//�ڴ�ֻ���ڶ���
	int rlen;				//��������,����[0, rlen)
	int clen;				//��������,����[0, clen)
	const char * data[];	//��������һά����,ϣ�����Ƕ�ά�� rlen*clen
} * sccsv_t;

/*
 * ���ļ��й���csv����, �����Ҫ���� sccsv_die �ͷ�
 * path		: csv�ļ�����
 *			: ���ع����õ� sccsv_t ����
 */
extern sccsv_t sccsv_new(const char * path);

/*
 * �ͷ���sccsv_new�����Ķ���
 * csv		: sccsv_new ���ض���
 */
extern inline void sccsv_delete(sccsv_t csv);

/*
 * ��ȡĳ��λ�õĶ�������,������� �Ƽ�����Ϊ������, window�ϲ�֧��
 * csv		: sccsv_t ����, new���ص�
 * ri		: ���ҵ������� [0, csv->rlen)
 * ci		: ���ҵ������� [0, csv->clen)
 *			: ������һ��������,��������� atoi, atof, str_dup �ȴ�����...
 */
extern inline const char* sccsv_get(sccsv_t csv, int ri, int ci);

#endif // !_H_SIMPLEC_SCCSV