# POE2 TradeDetector
OCR 기반으로 특정 키워드("구매")를 감지하고, 디스코드로 메시지를 자동 전송하는 프로그램 입니다.  
###### 잠시 자리를 비운 사이에 꼭 거래 메시지가 오는 경우가 많아 중요한 거래 요청을 놓치지 않기 위해 제작됨
### [다운로드]([https://www.notion.so/Project-Mote-1240c0421f3b81dbbce1eb1e0baae067?pvs=4](https://drive.google.com/file/d/1ZvTy3_kq-hP-dIx7PVxeHDAO3n31sX47/view?usp=drive_link) ".exe 파일다운")
### [사용방법](USERGUIDE.md "사용방법")  
## 기능 요약

- 화면 절반(좌측)을 5초마다 캡처하여 Tesseract OCR로 분석
- "구매" 텍스트 감지 시 디스코드 웹훅으로 캡처된 이미지와 함께 메시지 전송
- 게임 내 가장 최근 메시지의 [시 : 분] 시간으로 중복 전송 방지
- OpenCV 및 libcurl 사용
<br/><br/>
## 개발환경
###### VS2022 윈도우 어플리케이션에서 win Api와 c/c++으로 제작되었습니다.  
Tesseract OCR
- [tesseract-ocr-w64-5.5](https://github.com/UB-Mannheim/tesseract/wiki "tesseract-ocr 다운로드")

OpenCV4
- [opencv-4.11.0](https://opencv.org/releases/ "OpenCV 다운로드")

## 주의사항
- 본 프로그램은 개인적인 편의를 위해 제작되었으며, **해당 게임사로부터 공식적인 허가를 받지 않았습니다.**
- 본 프로그램을 사용하여 발생할 수 있는 **계정 정지, 제재, 서비스 이용 제한 등 모든 책임은 사용자 본인에게 있습니다.**
- 본 프로그램은 **게임 내 자동화된 입력(매크로) 기능은 포함하고 있지 않습니다.**
$\bf{\small{\color{#808080}(OCR\ 감지\ 및\ 디스코드\ 메시지\ 전송만\ 수행)}}$
