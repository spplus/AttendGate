#include "cmdbase.h"

class CmdImpl	:public CmdBase
{
public:
	void		exec(sClientMsg* msg);
private:
	// �����ȡ�û�
	void		getUser(sClientMsg* msg);

	// ����û�
	void		putUser(sClientMsg* msg);

	// ����򿨼�¼
	void		getLogData(sClientMsg* msg);

	// �����û�ʹ��
	void		enableUser(sClientMsg* msg);

	// ɾ���û�
	void		delUser(sClientMsg* msg);

	// ������״̬
	void		setDoorStatus(sClientMsg* msg);
};