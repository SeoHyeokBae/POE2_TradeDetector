# v2.0 사용자 가이드
프로그램 제작 당시 사용된 게임 버전은 <code>**POE2: 사냥의 서막 0.2.0g**</code> 입니다.
###### 이 페이지는 v2.0 버전을 기준으로 작성된 가이드입니다. v1.0 버전에 대한 가이드는 아래 링크를 참조
#### [v1.0 사용 가이드](https://github.com/SeoHyeokBae/POE2_TradeDetector/blob/POE2_TradeDetector_v1.0/USERGUIDE.md)

## 목차
- [사용방법](#사용방법)
- [백그라운드 기능](#백그라운드-기능)
- [Tip](#Tip)

## 사용방법
### 1.  디스코드 웹후크
- <code>**디스코드 웹후크**</code>**생성**
  + 디스코드<code>**서버, 채팅 채널**</code>을 생성합니다.  
![서버추가](https://github.com/user-attachments/assets/d1b7322e-f1fd-4b9d-8dc0-fc1643ab1364)
  + 채팅 채널 설정에서 웹후크를 생성한 후 <code>**웹후크 URL을 복사**</code> 합니다.  
![채널편집](https://github.com/user-attachments/assets/6e5af7cd-7399-43e6-89ee-681505f17123)
&nbsp;&nbsp;![연동](https://github.com/user-attachments/assets/b3bcbddb-bd1e-49a9-bc75-dfd81ef9bfd0)  
&nbsp;&nbsp;<img src=https://github.com/user-attachments/assets/d4a2847c-591f-4c32-ac93-6c26ff75317f width="445" height="450">

### 2.  경로 및 URL 설정
- <code>***POE2_TradeMsg.exe***</code>파일을 실행합니다.  
&nbsp;![실행파일](https://github.com/user-attachments/assets/78d59c99-b066-4d66-869b-6421438114ab)  
&nbsp;<img src=https://github.com/user-attachments/assets/29c7c17d-5d8a-4612-8705-17613e0426cc width="65%">

&nbsp;&nbsp;&nbsp;&nbsp;2-1. <code>**찾아보기**</code> 에서 채팅로그 파일인 <code>**KakaoClient.txt**</code>경로를 선택합니다.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; $\bf{\small{\color{#808080}(설치할\ 때\ 따로\ 변경이\ 없었다면)}}$ 드라이브:\Daum Games\Path of Exile2\logs\KakaoClient.txt  
   
&nbsp;&nbsp;&nbsp;&nbsp;2-2. <code>**등록**</code> 또는 <code>**변경**</code> 에서 디스코드 <code>**웹후크**</code>주소를 입력합니다.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;![등록](https://github.com/user-attachments/assets/edd279e7-2ec0-4c99-9094-99f54427b65a)  
- <code>**시작**</code>버튼을 눌러 감지를 시작합니다.  
&nbsp;<img src=https://github.com/user-attachments/assets/643a9fa8-cb23-41e0-be9f-a24c3af98295 width="65%">

### 3.  테스트
- 테스트용  <code>**test.txt**</code> 텍스트 파일을 생성
- 아래 텍스트 입력후 저장 $\bf{\small{\color{#808080}(ctrl\ +\ s)}}$  
  <code>@수신 구매자: 안녕하세요, 1 chaos(으)로 올려놓은 Dawn of the Hunt 리그의 테스트 아이템(을)를 구매하고 싶습니다</code>
&nbsp;<img src=https://github.com/user-attachments/assets/2247163c-c0de-498a-9189-db58c139433f width="70%">  
&nbsp;![스크린샷 2025-05-20 185733](https://github.com/user-attachments/assets/ad993af9-a16a-4fd2-9bcb-77c22b116620)



### 4. 확인
- <code>**디스코드**</code> 알림 또는 앱을 실행해 수시로 확인이 가능합니다.  
&nbsp;&nbsp;<img src=https://github.com/user-attachments/assets/8af8ac36-aceb-46c5-ba8a-0085439ca10b width="30%"></p>

## 백그라운드 기능
#### <code>**메시지**</code>
- 시스템로그와 메시지를 ***"구매하고 싶습니다"***, ***"@수신"*** 두 텍스트로 구매 메시지 판단
- 디스코드 메시지로 전송되는 내용은 채팅로그의 내용을 가격, 아이템, 구매자로 파씽하여 작성
- ***with Currency Image*** 에 사용되는 이미지는 ***"chaos"***, ***"devine"***, ***"exalted"*** 텍스트에 결정 $\bf{\small{\color{#808080}(세\ 종류의\ 커런시만\ 지원)}}$
#### <code>**파일**</code>
- 최초 시작 시 ***"path&url.txt"*** 자동 생성 $\bf{\small{\color{#808080}(.txt에서\ path\ 및\ url\ 직접\ 수정\ 가능)}}$
- ***"path&url.txt"*** 파일 첫째줄은 채팅 로그 경로, 둘째줄은 웹훅 url 을 순서로함
- 입력된 경로와 url은 저장되어 이후 프로그램 실행시 자동입력
#### <code>**프로그램 로그**</code>
- 시작, 중단 [시:분:초] 단위 기록됨
- 디스코드로 메시지 전송 ***"전송 실패"*** 표기됨 $\bf{\small{\color{#808080}(웹훅\ 주소가\ 잘못된\ 경우)}}$
<br></br>
## Tip
- 짧은 시간에 많은 양의 메시지는 문제가 될 수 있습니다.
- 프로그램 목적에 맞게 컴퓨터에서 자리를 비울때만 사용해주세요.
- <code>***with Currency Image***</code> 모드는 단순히 미리보기 알림 가독성용 입니다.   
![withcurrency](https://github.com/user-attachments/assets/929ccae7-d1cb-44cd-ae5d-2937b39dfe62)
