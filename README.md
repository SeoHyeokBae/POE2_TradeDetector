# POE2 TradeDetector_v2.0
실시간 채팅 로그를 모니터링하여 구매 메시지를 감지하고, 해당 내용을 **디스코드로 자동 전송하는 프로그램입니다.**  
###### 잠시 자리를 비운 사이에 꼭 거래 메시지가 오는 경우가 많아 중요한 거래 요청을 놓치지 않기 위해 제작됨
<img src="https://github.com/user-attachments/assets/a40076cc-5e0f-4d83-9e76-ebf860182e3e" width="27%">
<img src="https://github.com/user-attachments/assets/ddbde01f-febd-4e8d-beb1-8b5a10e4f5ec" width="67%">  

$\bf{\small{\color{#808080}(휴대폰의\ 디스코드\ 앱에서\ 메시지\ 알림을\ 받거나\ 수시로\ 확인\ 가능)}}$  

### [다운로드](https://drive.google.com/file/d/1r09fH6bfeFH4dOkgVIfhDzu4hL3TuC5o/view?usp=drive_link ".exe 파일다운")
### [사용방법](USERGUIDE.md "사용방법")  

###### ~~OCR 기반으로 특정 키워드("구매")를 감지하고, 디스코드로 메시지를 자동 전송하는 프로그램 입니다.~~ $\bf{\small{\color{#808080}(v1.0)}}$   
###### ~~2025.05.13: POE2에서 채팅로그가 게임 경로에 저장된다는 것을 알았음... 화면캡처->텍스트 분석~~ $\bf{\small{\color{#808080}(2025.05.19.v2.0\ Release)}}$  
##### [v1.0 둘러보기](https://github.com/SeoHyeokBae/POE2_TradeDetector/tree/POE2_TradeDetector_v1.0)

## 기능 요약
- 실시간 채팅이 기록되는 **채팅 로그** 파일 분석
- ***"구매하고 싶습니다"***, ***"@수신"*** 텍스트가 포함되면 디스코드 웹훅으로 메시지 전송
- 메시지는 가격, 아이템, 구매자 내용을 포함하여 전송
- 메시지에 판매되는 커런시에 맞춘 이미지 포함 전송 가능
<br/><br/>
## 개발환경
###### 윈도우11, VS2022 윈도우 어플리케이션에서 win Api와 c/c++으로 제작되었습니다.  
<img src="https://curl.se/logo/curl-logo.svg" alt="curl" width="10%">

- [libcurl 8.9.1](https://github.com/curl/curl "curl github")
<br/><br/>
## 출처
오브 아이콘
- [POE2 Wiki](https://www.poe2wiki.net/wiki/Category:Currency_item_icons)
<br/><br/>
## 주의사항
- 본 프로그램은 개인적인 편의를 위해 제작되었으며, **해당 게임사로부터 공식적인 허가를 받지 않았습니다.**
- 본 프로그램을 사용하여 발생할 수 있는 **계정 정지, 제재, 서비스 이용 제한 등 모든 책임은 사용자 본인에게 있습니다.**
- 본 프로그램은 **게임 내 자동화된 입력(매크로) 기능은 포함하고 있지 않습니다.**
$\bf{\small{\color{#808080}(디스코드\ 메시지\ 전송만\ 수행)}}$
