#pragma once
#define WM_FDREAD 12345
enum ProtocolDefine {
	// 로그인
	ISGAMING = 7,
	FULLUSER = 8,
	LOGOUT = 9,
	NOTLOGIN = 10,
	LOGIN = 11,

	// 유저 정보 받기
	REQUESTUSERNAME = 12, // 12 PlayerName
	SENDUSERNAME = 13, // 12 (int)PlayerNumber OtherPlayer1 OtherPlayer2 ...

	// 채팅
	CHATTING = 14, // 14 PlayerName Data 형식

	// 게임 레디
	NOTREADY = 15, // 15 PlayerName
	READY = 16, // 16 PlayerName

	// 플레이어 게임 준비
	INITCOMPLETE = 17, // 17 PlayerName

	// 게임 정보
	// 플레이어
	PLAYERISGREEN = 18, // 18 Player RED
	PLAYERISPURPLE = 19, // 19 Player BLUE
	POS = 20, // 20 PlayerName x y z 형식
	DIRECTION = 21, // 21 PlayerName x y z 형식
	BULLET = 22,  // 22 PlayerName x y z dirx diry dirz
	DEAD = 23, // 23 PlayerName

	// 게임 제어
	STARTGAME = 24, // 24 StartGame, 모든 유저가 INITCOMPLETE 끝나면 실행
	RESULTGAME = 25, // 25 ResultGame
	WAITGAME = 26, // 26 WaitGame, 
				   // 게임 대기, 보통 모든 유저가 
				   // INITCOMPLETE가 실행되지 않았을때 실행한다.
    CRITICALISSUE = 27, // 27 CriticalIssue, 서버에 오류가 발생하여 전원 종료
	// 메뉴 판
	REQUESTPLAYERKILLDEATH = 30, // 30 PlayerName kill Death
	SENDPLAYERKILLDEATH = 31,
};