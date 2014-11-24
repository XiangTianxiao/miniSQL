#ifndef _BUFFER_MANAGER_H
#define _BUFFER_MANAGER_H

#include <string>;
using namespace std;



//��ȡblock���ɹ�����true��ʧ�ܷ���false
bool bRead(block* b, string filename, int offset);
//д��block���ɹ�����true��ʧ�ܷ���false
bool bwrite(block* b, string filename, int offset);
//���ļ��н����µ�block��Ҳ�������ļ�ĩβ���һ���µĿ飬����offset

int bNewBlock(string filename);
//ɾ��һ���飬�ɹ�true��ʧ�ܷ���false
//��ν��ɾ��������ɾ��һ���飬֮�����Ķ���ǰ�ƶ�һλ��
//�����ڿ�Ŀ�ͷ�н�֮���Ϊɾ����
bool bDeleteBlock(string filename, int offset);

//����������������д������
void bFlush(string filename);
//����ɾ�������
void bClear(string filename);

//��һ����̶����������ڣ��ɹ�true��ʧ��false
bool bPin(string filename, int offset);
//����ӻ���������
bool bUnPin(string filename, int offset);

#endif