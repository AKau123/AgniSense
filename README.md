# AgniSense
# 🚦 QNX-Based Real-Time Traffic Congestion Control System using SDV & V2V Communication

## Introduction  
This project focuses on building a real-time traffic congestion control system using a Software Defined Vehicle (SDV) approach powered by QNX RTOS. It uses a Raspberry Pi 4 along with sensors and communication modules to simulate how vehicles can sense their surroundings, make quick decisions, and coordinate with other vehicles. The goal is to improve traffic flow and enhance road safety through intelligent and real-time control.

---

## Problem Statement  
Current traffic systems lack real-time response and coordination between vehicles. Most systems depend on human reaction, which leads to delays, congestion, and accidents. There is a need for a system that can respond quickly, share information, and manage traffic more efficiently.

---

## Proposed Solution  
This system:
- Monitors surroundings using sensors  
- Processes data in real time using QNX RTOS  
- Communicates with other vehicles using V2V  
- Controls vehicle movement based on traffic conditions  

---

## Key Features  
- Real-time monitoring with fast response (<50 ms)  
- Deterministic performance using QNX RTOS  
- State-based decision model (S0–S3)  
- V2V communication using Bluetooth  
- Emergency vehicle prioritization  

---

## 🧠 System Architecture  
The system is organized into layers for better clarity and control:  
**Sensing → Processing → Decision → Communication → Actuation**

---

## 🔄 Methodology  
1. Sensor measures distance  
2. Data is processed in QNX  
3. Traffic state is determined  
4. Decision is made  
5. Information is shared via Bluetooth  
6. Vehicle movement is adjusted  

---

## 🧰 Hardware Components  
- Raspberry Pi 4 (QNX RTOS)  
- ESP32 modules  
- Ultrasonic sensors  
- HC-05 Bluetooth module  
- L298N motor driver  
- DC motors  
- TFT display  

---

## 💻 Software Components  
- QNX RTOS  
- C programming  
- Multi-threading (`pthread`)  
- UART communication  

---

## 📊 Results  
- Smooth operation in clear road conditions  
- Gradual speed control during traffic  
- Immediate stop in emergency situations  
- Response time below 50 ms  

---

## 🚀 Applications  
- Smart traffic systems  
- Emergency vehicle management  
- Collision avoidance  
- Intelligent transportation  

---

## 🔮 Future Scope  
- GPS and camera integration  
- AI-based traffic prediction  
- V2X communication  
- Real-world deployment  

---

---
## 👨‍💻 Contributors
- G.V.S.K.Akhil  
- E.Deepsika Reddy
- Sk Sarah
