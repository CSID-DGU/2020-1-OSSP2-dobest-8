# 2020-1-OSSP2-dobest-8
[OSSP] 한재진 이서연 노수민

## Tetris vs Mouse 

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
+ 나머지 아이템 기능 구현 중

11주차:
+ 모드 변경기능 구현중
+ 무적 아이템, 블럭제거 아이템 구현 완료



## Dependencies

You will need:

+ [SDL 2.0](https://www.libsdl.org/hg.php)
+ [SDL TTF 2.0](https://www.libsdl.org/projects/SDL_ttf/)
+ [SDL image 2.0](https://www.libsdl.org/projects/SDL_image/)

### Ubuntu

Install all dependencies on Ubuntu 14.04 and above:

`sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev`

## Get Source and Build

```
git clone https://github.com/CSID-DGU/2020-1-OSSP2-dobest-8/tetris.git
cd tetris
make
```

## Run the game

To run the game:

`./tetris`.

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
