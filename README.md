# SimpleFPSGame
-SimpleFPSGame은 여러 DirectX9을 공부한 자료를 토대로 만들어진 게임입니다.

-WASD와 R(재장전), 그리고 마우스의 입력을 받습니다.

-둠과 같은 초기 FPS 느낌의 게임을 제작하려고 했으나 미완성으로 남게 되었습니다. 미완성된 부분은 맵 충돌과 네트워크를 이용한 게임 플레이입니다.

-FBX Load와 버텍스 로드 부분이 매우 비효율적으로 되어 있습니다. 이 부분은 DirectX 11 Renderer에서 개선했습니다.

-DirectX 11과 비교하여 성능이 낮습니다. Renderer에서 불러오는 Dragon의 경우 이 게임에선 매우 낮은 프레임으로 렌더링됩니다.

2021-11-15
-프로젝트가 제대로 컴파일 안된다는 메세지를 받았습니다. 일단 동적 libfbxsdk.dll을 추가하셔야 합니다. 하지만 그래도 안될 경우
-프로젝트를 배포하기 위한 전에 미리 만든 설치 프로젝트로 빌드한 설치 파일을 구글 드라이브 링크를 통해 배포하겠습니다. 감사합니다.
https://drive.google.com/file/d/1YB1Zrgsr-xMLektZ6WD2g42aPPxoYhBV/view?usp=sharing
