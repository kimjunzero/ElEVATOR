대한상공회의소 AI 시스템 반도체 SW 개발자 과정

STM32를 활용하여 직접 I2C 프로토콜 구현 & 엘레베이터 구현하기

재작기간 : 2025-4-9 ~ 2025-4-11

---

1. FSM
2. 회로도
3. 기능
- I2C 프로토콜
- RTOS 기반 엘레베이터 구현

 4.   설계

---

# 1. FSM
![image.png](attachment:e9a69f5e-0fa2-437c-bf58-0b2d9e1f7372:image.png)
![1](https://github.com/user-attachments/assets/e9a69f5e-0fa2-437c-bf58-0b2d9e1f7372)
---

# 2. 회로도

![image.png](attachment:88eef7e4-2361-430b-aaaa-3a78b4b72248:image.png)

![image.png](attachment:e653fe12-b46f-4a9d-89e3-33abb0b67cd3:image.png)

---

# 3. I2C 프로토콜

- HAL_I2C_Master_Transmit() 같은 고수준 함수 대신 GPIO 직접 제어
- SDA, SCL 핀을 직접 제어하며 타이밍과 신호 생성
- START/STOP 조건, 슬레이브 주소 전송, ACK 체크, 데이터 송수신을 타이밍에 맞춰 비트 단위로 구현
- ACK 응답 처리 → 마스터는 전송 후 SDA를 입력모드로 전환하여 슬레이브의 ACK 응답을 확인

---

# 4. RTOS 엘레베이터 구현

- 기존 non-os 방식(동기식) → RTOS 방식(비동기식 중심)
- 이벤트 발생 시 즉시 반응
- Thread 분리로 유지 보수 쉽고, Mutex, semaphore로 안전하게 공유 가능
- 총 3개의 thread로 구성

```c
void StartTask02(void *argument)
{
  for(;;)
  {
	  dotmatrix_main();
	  osDelay(1);
  }
}

void StartTask03(void *argument)
{
  for(;;)
  {
	  loop();
	  osDelay(1);
  }
}

void StartTask04(void *argument)
{
  for(;;)
  {
	  fnd_display_update();
	  osDelay(1);
  }
}
```

- **Dotmatrix 기능**
1. 해당 층에 도착했을 경우 : 해당 층 숫자 출력
2. 윗 층으로 올라갈 경우 : UP 스크롤
3. 아랫 층으로 내려갈 경우 : Down 스크롤

- 1층 화면(잔상효과 남음)

![image.png](attachment:583dc84e-08d2-4a1b-83cd-97656afd8a3a:image.png)

1. 위로 올라가는 경우

![image.png](attachment:cc5fbb0b-e864-470b-8c93-a69e231ab1bb:image.png)

- **FND - shiftregister 기능**
1. 초기, 층 도착했을 때 : UNDER_BAR 표시
2. 올라갈때 : UP_층
3. 내려갈때 :  dn_층

1. 초기, 층 도착했을 때

![image.png](attachment:98c17e4d-a606-43d9-a7e9-ada4954a4dd1:image.png)

1. 1층 내려가는 FND

![image.png](attachment:58f09e1f-99ee-4a4e-9a75-239277ad3e2d:image.png)

1. 2층으로 올라가는 FND

![image.png](attachment:0dfc9e87-f25f-4630-b093-a2876a87c8de:image.png)

- **stepmotor-photosensor 기능**
1. U자형 포토 센서 사이에 장애물이 지나가면 빛이 차단되어 해당 층 도달 여부 검사.
2. 각 층마다 포토 센서를 배치해 스텝 모터 회전 중 정확한 층 위치 확인
3. 4-step 시퀀스로 4096스텝 = 360도 회전
- 최종 구현 현상

![image.png](attachment:73697471-01a8-4969-8112-e7805d13c4e4:image.png)

---

# 4. 설계

1. **RTOS(Real-Time Operating System) 기반 제어**
- 핵심 개념 : RTOS는 실시간성을 보장하는 운영체제로, 각 기능을 Task로 분리하여 병렬 실행이 가능함.
- 적용
- Task01 : 버튼입력, 모터제어, 포토센서 인식 → 목표층 설정 및 이동
- Task02 : Dot Matrix → 도착층 및 이동 방향 표시
- Task03 : FND → 현재 엘리베이터 상태 출력

1. **I2C 통신 프로토콜**
- 기존 방식 2라인(SDA, SCL)으로 다수의 슬레이브와 통신 가능
- HAL 방식이 아닌 Bit-Banging 방식으로 I2C 구현

# 5. 결론 및 시사점

- I2C의 타이밍 제어 이해.
- 프로토콜 흐름(START → 주소 전송 → ACK→ 데이터 전송 → STOP)을 완전히 숙지.
- 데이터시트를 보고 직접 통신 규약을 구현 가능.
- 시프트 레지스터 사용으로 직렬 → 병렬 원리 이해.
- 하드웨어 동작 원리 + 타이밍 기반 신호 처리 + 비트 연산 로직 이해. 각 모듈을 포팅 작업하므로써 사용법을 익힘.
