# 2020-1-OSSP2-dobest-8
[OSSP] 한재진 이서연 노수민

## Upgrade Tetris 

![tetris](https://user-images.githubusercontent.com/45312118/85222004-63bd5a00-b3f3-11ea-9b6f-be32c4ba80eb.gif)
![intro](https://user-images.githubusercontent.com/45312118/85222079-c31b6a00-b3f3-11ea-8f24-7dadebf88b42.png)

업그레이드 된 테트리스 게임으로, 싱글모드와 멀티모드가 준비되어있다.
싱글모드에서는 일반적인 테트리스 게임을 즐길 수 있다.
멀티모드는 대결모드로 마우스 유저와 테트리스 유저가 대결을 하는 모드이다. 마우스 유저는 테트리스 유저를 피해 도망다니며, 아이템을 사용하여 테트리스 유저를방해를 할 수 있다.
테트리스 유저는 마우스 유저를 잡기위해 테트로미노(블록)을 이용해서 잘 쌓거나 움직여야한다.

Development environment:
```
Linux written in C++ 
with SDL 2.0.
```

Reference source:
```
https://github.com/chaficnajjar/21st-century-tetris
```

## The progress of the project

6,7주차:
+ Reference source 분석 및 아이디어 회의

8주차:
+ 마우스 클래스 생성, 마우스 구현
+ 마우스 존 생성(마우스 유저는 안에서만 이동 가능)

9주차:
+ 마우스 유저와 테트리스 유저가 충돌 시 테트리스 유저의 승리 판정 구현
+ 테트리스 블럭이 마우스 존의 80퍼센트를 차지할 시에 마우스의 승리
+ 승리 판정에 따른 결과를 출력하는 승리창 구현

10주차:
+ 사용자가 게임 창을 원하는 대로 늘릴 수 있도록 resize 기능을 구현
+ 테트리스 유저가 마우스 존 보다 높게 테트로미노를 쌓을 시 마우스 유저의 승리 판정 구현
+ 무적 아이템 기능 구현

11주차:
+ 홀드 기능 구현
+ 테트로미노를 파괴하는 아이템 기능 구현
+ 중간 발표 준비 및 중간 발표

12주차:
+ 이미지 로고 수정
+ 인트로 이미지 수정
+ 시험 준비 및 시험

13주차:
+ 테트로미노를 임의의 위치에 놓을 수 있도록 하는 아이템 기능 구현
+ 마우스 포인터 이미지 삽입 및 아이템을 먹었을 시 이미지 색 변경으로 남은 시간 표시

14주차:
+ 아이템 이미지 로드
+ 사용자가 멀티모드 or 싱글모드 선택할 수 있도록 모드 구현

15주차:
+ help모드 구현
+ 아이템, 게임오버 효과음 삽입
+ 메뉴 버튼 및 메뉴 구현
+ 최종 발표 및 최종 보고서 준비

## Dependencies

You will need:

+ [SDL 2.0](https://www.libsdl.org/hg.php)
+ [SDL TTF 2.0](https://www.libsdl.org/projects/SDL_ttf/)
+ [SDL image 2.0](https://www.libsdl.org/projects/SDL_image/)
+ [SDL mixer 2.0](https://www.libsdl.org/projects/SDL_mixer/)

### Ubuntu

Install all dependencies on Ubuntu 14.04 and above:

`sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev`

## Get Source and Build

```
git clone https://github.com/CSID-DGU/2020-1-OSSP2-dobest-8/tetris.git
cd tetris
make
```

## Run the game

To run the game:

`./tetris`.

## How to play

Up arrow/w        -> Rotates the current tetromino
 
Left arrow/a      -> Moves the current tetromino to the left

Right arrow/d     -> Moves the current tetromino to the right

Down arrow/s      -> Speeds up the current tetromino

Spacebar          -> Zooms the current tetromino to the bottom

Shift             -> Hold tetromino

p                 -> Pauses/resumes game

New Game          -> Starts new game

Quit              -> Quits

Below is for multi-mode.

Mouse Left Click 
And Drag          -> Move Mouse user

Mouse Right Click -> Use Create Blcok Item

## Multimode Items

Destroy Block Item
-If you eat Negative icon item, you can remove blocks.
Crash into the block you want.

Create Block Item
-If you eat Plus icon item, you can remove blocks.
Click Right Mouse button.

God Item
-If you eat Shield icon item,
You don't die if you crash the block for a certain period of time.

## License and Credits

```
The MIT License (MIT)

Copyright (c) 2014 Chafic Najjar

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

[irrKlang](http://www.ambiera.com/irrklang/index.html) for sound.
