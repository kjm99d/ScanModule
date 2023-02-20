# ScanFile
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

특정 프로세스에 연결된 모듈 정보를 파일경로와 파일버전으로 수집합니다.

## 사용 가이드
1. 프로그램 실행
2. 실행 후 모듈을 검사할 PID(Process ID) 입력
3. 프로그램 경로에 존재하는 `ScanModule.csv` 확인

해당 파일 정보에는 `모듈경로, 버전` 포맷으로 나타납니다.


## 개발환경
- Visual Studio 2019
- C++17
- Windows SDK 버전 (v10.0.10914.0)
