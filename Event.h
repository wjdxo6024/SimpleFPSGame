#pragma once
enum ButtonEventDefine
{
	Null = 0,
	// Yes No Cancel �� �̺�Ʈ
	Yes = 10,
	No,
	Cancel,
	Summit,

	// ��Ƽ�÷���
	// �÷��̾� ����
	Ready = 1000,
	NotReady = 1001,

	// ������ �̵��� ��ư �̺�Ʈ
	ToMainScene = 10000,
	ToSinglePlay,
	ToMultiPlay,
	ToMultiPlayRoom,
	ToOption,
	ToExit,
	BackScene
};

class Event
{
};

