# POE2 TradeDetector
OCR 기반으로 특정 키워드("구매")를 감지하고, 디스코드로 메시지를 자동 전송하는 프로그램 입니다.  
###### 잠시 자리를 비운 사이에 꼭 거래 메시지가 오는 경우가 많아 중요한 거래 요청을 놓치지 않기 위해 제작됨
###### 휴대폰의 디스코드 앱에서 메시지 알림을 받거나 수시로 확인 가능

### [다운로드](https://drive.google.com/file/d/1r09fH6bfeFH4dOkgVIfhDzu4hL3TuC5o/view?usp=drive_link ".exe 파일다운")
### [사용방법](USERGUIDE.md "사용방법")  
###### 2025.05.13: POE2에서 채팅로그가 게임 경로에 저장된다는 것을 알았음... 화면캡처->텍스트 분석(업데이트 예정)
## 시연 영상
https://github.com/user-attachments/assets/a443489d-1e08-4f84-89dc-0c3cf547d10a

## 기능 요약

- 화면 절반(좌측)을 5초마다 캡처하여 Tesseract OCR로 분석
- "구매" 텍스트 감지 시 디스코드 웹훅으로 캡처된 이미지와 함께 메시지 전송
- 게임 내 가장 최근 메시지의 [시 : 분] 시간으로 중복 전송 방지
- OpenCV 및 libcurl 사용
<br/><br/>
## 개발환경
###### 윈도우11, VS2022 윈도우 어플리케이션에서 win Api와 c/c++으로 제작되었습니다.  
<img src="https://curl.se/logo/curl-logo.svg" alt="curl" width="10%">

- [libcurl 8.9.1](https://github.com/curl/curl "curl github")
## 출처
오브 아이콘
- [POE2 Wiki](https://www.poe2wiki.net/wiki/Category:Currency_item_icons)

## 주의사항
- 본 프로그램은 개인적인 편의를 위해 제작되었으며, **해당 게임사로부터 공식적인 허가를 받지 않았습니다.**
- 본 프로그램을 사용하여 발생할 수 있는 **계정 정지, 제재, 서비스 이용 제한 등 모든 책임은 사용자 본인에게 있습니다.**
- 본 프로그램은 **게임 내 자동화된 입력(매크로) 기능은 포함하고 있지 않습니다.**
$\bf{\small{\color{#808080}(디스코드\ 메시지\ 전송만\ 수행)}}$
