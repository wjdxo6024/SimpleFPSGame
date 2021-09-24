#pragma once
#define WM_FDREAD 12345
enum ProtocolDefine {
	// �α���
	ISGAMING = 7,
	FULLUSER = 8,
	LOGOUT = 9,
	NOTLOGIN = 10,
	LOGIN = 11,

	// ���� ���� �ޱ�
	REQUESTUSERNAME = 12, // 12 PlayerName
	SENDUSERNAME = 13, // 12 (int)PlayerNumber OtherPlayer1 OtherPlayer2 ...

	// ä��
	CHATTING = 14, // 14 PlayerName Data ����

	// ���� ����
	NOTREADY = 15, // 15 PlayerName
	READY = 16, // 16 PlayerName

	// �÷��̾� ���� �غ�
	INITCOMPLETE = 17, // 17 PlayerName

	// ���� ����
	// �÷��̾�
	PLAYERISGREEN = 18, // 18 Player RED
	PLAYERISPURPLE = 19, // 19 Player BLUE
	POS = 20, // 20 PlayerName x y z ����
	DIRECTION = 21, // 21 PlayerName x y z ����
	BULLET = 22,  // 22 PlayerName x y z dirx diry dirz
	DEAD = 23, // 23 PlayerName

	// ���� ����
	STARTGAME = 24, // 24 StartGame, ��� ������ INITCOMPLETE ������ ����
	RESULTGAME = 25, // 25 ResultGame
	WAITGAME = 26, // 26 WaitGame, 
				   // ���� ���, ���� ��� ������ 
				   // INITCOMPLETE�� ������� �ʾ����� �����Ѵ�.
    CRITICALISSUE = 27, // 27 CriticalIssue, ������ ������ �߻��Ͽ� ���� ����
	// �޴� ��
	REQUESTPLAYERKILLDEATH = 30, // 30 PlayerName kill Death
	SENDPLAYERKILLDEATH = 31,
};