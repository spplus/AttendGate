#include "cmdbase.h"
#include "buff/msgbody.pb.h"
#include "defines.h"
class CmdImpl	:public CmdBase
{
public:
	void		exec(sClientMsg* msg);
private:
	// �����ȡ�û�
	void		getUser(sClientMsg* msg);

	// ��ȡ�û��б�
	void		getUserList(sClientMsg* msg);

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

	// �����豸
	int			connect(PBNS::AttendBean attend);
};