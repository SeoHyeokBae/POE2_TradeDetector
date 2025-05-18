# v2.0 사용자 가이드
프로그램 제작 당시 사용된 게임 버전은 POE2: 사냥의 서막 0.2.0g 입니다.
###### 이 페이지는 v2.0 버전을 기준으로 작성된 가이드입니다. v1.0 버전에 대한 가이드는 아래 링크를 참조
#### [v1.0 사용 가이드](https://github.com/SeoHyeokBae/POE2_TradeDetector/blob/POE2_TradeDetector_v1.0/USERGUIDE.md)

## 목차
- [사용환경](#사용환경)
- [사용방법](#사용방법)
- [백그라운드 기능](#백그라운드-기능)
- [프로그램 한계](#프로그램-한계)
## 사용환경
###### 더블 모니터에서 메인 모니터 화면 캡처
#### <code>**채팅창 옵션**</code> $\bf{\small{\color{#808080}(POE2\ 0.2.0g버전)}}$  
- **"채팅 입력 시간 보기"** ***필수*** 체크  
<img src=https://github.com/user-attachments/assets/c50548b9-4820-49e7-bedc-4daeaedf7b96 width="521" height="350">

#### <code>**성소 은신처**</code> $\bf{\small{\color{#808080}(어두운\ 배경:\ 문자의\ 정확도를\ 높여줌)}}$
![성소은신처](https://github.com/user-attachments/assets/82649e74-75ed-4b65-895b-87296299b7ef)

## 사용방법
### 1.  디스코드 웹후크
- <code>**디스코드 웹후크**</code>**생성**
  + 디스코드<code>**서버, 채팅 채널**</code>을 생성합니다.  
![서버추가](https://github.com/user-attachments/assets/d1b7322e-f1fd-4b9d-8dc0-fc1643ab1364)
  + 채팅 채널 설정에서 웹후크를 생성한 후 <code>**웹후크 URL을 복사**</code> 합니다.  
![채널편집](https://github.com/user-attachments/assets/6e5af7cd-7399-43e6-89ee-681505f17123)
&nbsp;&nbsp;![연동](https://github.com/user-attachments/assets/b3bcbddb-bd1e-49a9-bc75-dfd81ef9bfd0)  
&nbsp;&nbsp;<img src=https://github.com/user-attachments/assets/d4a2847c-591f-4c32-ac93-6c26ff75317f width="445" height="450">

### 2.  실행
- <code>***POE2_TradeMsg.exe***</code>파일 실행합니다.  
&nbsp;![실행파일](https://github.com/user-attachments/assets/78d59c99-b066-4d66-869b-6421438114ab)  
&nbsp;<img src=https://github.com/user-attachments/assets/90fa767a-6848-4fa8-b1fc-bacd6498babf width="365" height="350">

- <code>**등록**</code> 또는 <code>**변경**</code> 에서 디스코드 <code>**웹후크**</code>주소를 입력합니다.  
![등록](https://github.com/user-attachments/assets/edd279e7-2ec0-4c99-9094-99f54427b65a)  
- <code>**시작**</code>버튼을 눌러 감지를 시작합니다.  
![실행](https://github.com/user-attachments/assets/f6fa7387-1165-4d84-abe0-2d42fdc7e53f)

### 3.  테스트
- <code>**인게임 테스트**</code>$\bf{\small{\color{#808080}(게임에서\ "구매"\ 채팅\ 입력)}}$  
&nbsp;<img src=https://github.com/user-attachments/assets/f9254bb3-5be5-4837-a047-c66ff690bce0 width="450" height="450">

- <code>**게임을 실행하지 않은 경우**</code>$\bf{\small{\color{#808080}(메모장에서\ [시:분]\ "구매"\ 입력)}}$  
&nbsp;<img src=https://github.com/user-attachments/assets/8482996c-1770-4ff6-bee5-7848f634a9c3 width="450" height="461">

### 4. 확인
- <code>**디스코드**</code> 알림 또는 앱을 실행해 수시로 확인이 가능합니다.  
&nbsp;&nbsp;<img src=https://github.com/user-attachments/assets/08c92039-6f84-41c1-aba7-9d6b97a8c9b3 width="311" height="600"></p>

### Tip.
- 텍스트화는 메인 화면 절반 좌측 이미지로 진행됩니다.
- OCR은 배경과 글자의 대비가 클수록 정확도가 높아집니다. $\bf{\small{\color{#808080}(어두운\ 지형\ 권장)}}$ 
- 시작하기 전 채팅창에 ***"구매"*** 가 있는경우<code>**/clear**</code>으로 채팅창을 비워주세요.

## 백그라운드 기능
#### <code>**메시지**</code>
- **5초** 간격으로 메시지를 체크
- **[시:분]** 과 ***"구매"*** 텍스트가 함께 있어야 메시지가 전송됨
- 최근 메시지의 **[시:분]** 을 기록하여 중복 전송을 방지
#### <code>**파일**</code>
- 최초 시작 시 ***"webhookurl.txt"*** 자동 생성 $\bf{\small{\color{#808080}(.txt에서\ url\ 직접\ 수정\ 가능)}}$
- ***"구매"*** 가 감지되고 디스코드로 메시지를 전송할때 ***"screenshot.png"*** 생성 $\bf{\small{\color{#808080}(이후\ 이미지는\ 덮어씀)}}$
#### <code>**로그**</code> $\bf{\small{\color{#808080}(감지\ 시작\ 이후)}}$
- 20분 간격으로 ***"실시간 감지중 ..."*** 로그창에 기록
- 3시간 간격으로 ***"감지 시작"*** 과 마지막 ***"실시간 감지중"*** 을 **제외**한 로그 기록 제거
## 프로그램 한계
###### 항상 정확하게 텍스트를 인식하지 못하는 점을 고려해 만들어짐
#### • &nbsp;이미지를 텍스트화하는 과정에서 한글 인식률이 좋지 않음 $\bf{\small{\color{#808080}("구매"\ 키워드\ 선정\ 이유)}}$
&nbsp; &nbsp;<img src=https://github.com/user-attachments/assets/41a620bb-2dbd-498b-999c-9e259a1299bd width="439" height="400">
&nbsp; &nbsp;<img src=https://github.com/user-attachments/assets/0e05d5e3-3751-47ec-9ac1-9977bd5a6443 width="360" height="400">
#### • &nbsp;분 단위 시간 기록을 하므로 같은 시간에 여러 메시지가 올 경우 누락될 수 있음
#### • &nbsp;*"구매"* 라는 키워드와 함께 **[시:분]** 의 텍스트가 있다면 모두 반응하여 메시지를 보냄
#### • &nbsp;*"구매"* 를 감지 했을때 내부 메시지 시간은 가장 최근 메시지 시간으로 기록
&nbsp; &nbsp;<img src=https://github.com/user-attachments/assets/e7827838-6900-46c6-a7a7-1f7ac0d4186f width="640" height="200">  
&nbsp; &nbsp; 위 경우 프로그램의 내부 최근 메시지 시간은 **[16:39]** 에서 **[16:40]** 으로 변경되어 메시지를 또 전송하게 됨  
&nbsp; &nbsp; $\bf{\small{\color{#808080}(채팅창을\ 켰을\ 경우\ 해당하며\ 프로그램을\ 사용하는\ 보통의\ 상황에서는\ "구매"메시지가\ 사라진\ 이후이기\ 때문에\ 기록되지\ 않음)}}$
