#pragma once
enum ButtonEventDefine
{
	Null = 0,
	// Yes No Cancel 등 이벤트
	Yes = 10,
	No,
	Cancel,
	Summit,

	// 멀티플레이
	// 플레이어 레디
	Ready = 1000,
	NotReady = 1001,

	// 페이지 이동시 버튼 이벤트
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

