#include <schead.h>
#include <sccsv.h>
#include <sclog.h>
#include <tstr.h>

//���ļ��ж�ȡ csv�ļ�����
char* _csv_get(FILE * txt, int * prl, int * pcl)
{
	int c, n;
	int cl = 0, rl = 0;
	TSTR_NEW(ts);

	while((c=fgetc(txt))!=EOF) {
		if('"' == c) {	//������������
			while((c=fgetc(txt))!=EOF) {
				if('"' == c) {
					if((n=fgetc(txt)) == EOF) { //�ж���һ���ַ�
						TSTR_DELETE(ts);
						SL_WARNING("The CSV file is invalid one!");
						return NULL;
					}
					if(n != '"'){ //��Ч�ַ��ٴ�ѹ��ջ
						ungetc(n, txt);
						break;
					}
				}

				// ��ӵõ����ַ�
				tstr_append(ts, c);
			}
			//�����ж�,ֻ����c == '"' �Ż�����,�����Ǵ��
			if('"' != c){
				TSTR_DELETE(ts);
				SL_WARNING("The CSV file is invalid two!");
				return NULL;
			}
		}
		else if(',' == c){
			tstr_append(ts, '\0');
			++cl;
		}
		else if('\r' == c)
			continue;
		else if('\n' == c){
			tstr_append(ts, '\0');
			++cl;
			++rl;
		}
		else { //�����������ֻ������ݾͿ�����
			tstr_append(ts, c);
		}
	}
	
	if(cl % rl){ //��� , ���Ǹ����Ƿ�����
		TSTR_DELETE(ts);
		SL_WARNING("now csv file is illegal! need check!");
		return NULL;
	}
	
	// ������������
	*prl = rl;
	*pcl = cl;
	return ts->str;
}

// �� _csv_get �õ����������¹�������, ִ�����������Ϊ�﷨��ⶼ��ȷ��
sccsv_t _csv_get_new(const char * cstr, int rl, int cl)
{
	int i = 0;
	sccsv_t csv = sm_malloc(sizeof(struct sccsv) + sizeof(char *) * cl);
	
	// ���￪ʼ����������
	csv->rlen = rl;
	csv->clen = cl / rl;
	do {
		csv->data[i] = cstr;
		while(*cstr++) //�ҵ���һ��λ�ô�
			;
	} while(++i<cl);
	
	return csv;
}

/*
 * ���ļ��й���csv����, �����Ҫ���� sccsv_die �ͷ�
 * path		: csv�ļ�����
 *			: ���ع����õ� sccsv_t ����
 */
sccsv_t 
sccsv_new(const char * path)
{
	FILE * txt;
	char * cstr;
	int rl, cl;
	
	DEBUG_CODE({
		if(!path || !*path){
			SL_WARNING("params is check !path || !*path .");
			return NULL;
		}
	});

	// ���ļ�����
	if((txt=fopen(path, "r")) == NULL){
		SL_WARNING("fopen %s r is error!", path);
		return NULL;
	}
	// ������� csv �ļ�����ʧ��ֱ�ӷ���
	cstr = _csv_get(txt, &rl, &cl);
	fclose(txt);

	// �������ս��
	return cstr ? _csv_get_new(cstr, rl, cl) : NULL;
}

/*
 * �ͷ���sccsv_new�����Ķ���
 * csv		: sccsv_new ���ض���
 */
inline void 
sccsv_delete(sccsv_t csv) {
	sm_free(csv);
}

/*
 * ��ȡĳ��λ�õĶ�������
 * csv		: sccsv_t ����, new���ص�
 * ri		: ���ҵ������� [0, csv->rlen)
 * ci		: ���ҵ������� [0, csv->clen)
 *			: ������һ��������,��������� atoi, atof, str_dup �ȴ�����...
 */
inline const char*
sccsv_get(sccsv_t csv, int ri, int ci) {
	DEBUG_CODE({
		if(!csv || ri<0 || ri>=csv->rlen || ci<0 || ci >= csv->clen){
			SL_WARNING("params is csv:%p, ri:%d, ci:%d.", csv, ri, ci);
			return NULL;
		}
	});

	// �������ս��
	return csv->data[ri*csv->clen + ci];
}