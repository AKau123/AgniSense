# AgniSense
# 🚦 QNX-Based Real-Time Traffic Congestion Control System using SDV & V2V Communication

## 📌 Overview
This project presents a real-time traffic congestion control system based on a Software Defined Vehicle (SDV) architecture using QNX Real-Time Operating System (RTOS). The system integrates sensing, decision-making, communication, and actuation to enable intelligent traffic management and improve road safety.

It uses Raspberry Pi 4 running QNX RTOS along with ultrasonic sensors, ESP32 modules, and Bluetooth communication to simulate real-world vehicle behavior and coordination.

---

## 🎯 Problem Statement
Modern traffic systems suffer from congestion, delayed human reaction, and lack of communication between vehicles. Existing systems are non-deterministic and fail to provide real-time response, making them unsuitable for safety-critical applications.

---

## 💡 Proposed Solution
The proposed system enables:
- Real-time sensing using ultrasonic sensors  
- Fast decision-making using QNX RTOS  
- Vehicle-to-Vehicle (V2V) communication  
- Intelligent traffic control and emergency prioritization  

---

## ⚙️ Key Features
- ✅ Real-time monitoring and fast response (<50 ms)  
- ✅ Deterministic execution using QNX RTOS  
- ✅ State-based decision model (S0–S3)  
- ✅ V2V communication using Bluetooth (HC-05 + ESP32)  
- ✅ Emergency vehicle priority handling  
- ✅ Integrated sensing, communication, and actuation  

---

## 🧠 System Architecture
The system follows a layered architecture:

1. **QNX Microkernel Layer** – Real-time scheduling and IPC  
2. **Sensing Layer** – Ultrasonic sensors  
3. **Perception Layer** – Data filtering and processing  
4. **Decision Layer** – FSM-based control logic  
5. **Communication Layer** – V2V using Bluetooth  
6. **Actuation Layer** – Motor control and alerts  

📷 *(Add your architecture diagram image here)*

---

## 🔄 Methodology
1. Sensor detects distance  
2. Data processed in QNX RTOS  
3. Traffic state determined (S0–S3)  
4. Decision made based on risk  
5. Data transmitted via Bluetooth  
6. ESP32 receives and relays data  
7. Vehicle movement controlled via motor driver  
8. System repeats continuously  

---

## 🧰 Hardware Components
- Raspberry Pi 4 (QNX RTOS)
- ESP32 (2 units)
- Ultrasonic Sensors (HC-SR04)
- HC-05 Bluetooth Module
- L298N Motor Driver
- DC Motors
- TFT Display (Non-SDV Unit)
- Power Supply (Power bank & battery)

---

## 💻 Software Components
- QNX RTOS  
- C Programming  
- Multi-threading using `pthread`  
- UART Communication (`termios`)  
- QNX System Libraries (`sys/neutrino`)  

---

## 🔌 System Workflow
- Sensor → QNX Processing → Decision → Communication → Actuation  
- Continuous feedback loop for real-time adaptation  

---

## 📊 Results
The system was tested under different scenarios:

### 🚗 Clear Road
- Vehicle moves at normal speed  

### 🚦 Traffic Condition
- Vehicle slows down gradually  

### 🚨 Emergency / Obstacle
- Immediate stop and alert generation  

### 📈 Performance
- Response Time: < 50 ms  
- Stable communication  
- Accurate decision-making  

---

## 🚀 Applications
- Smart Traffic Management Systems  
- Emergency Vehicle Priority Systems  
- Collision Avoidance Systems  
- Autonomous Vehicle Safety  
- Intelligent Transportation Systems  

---

## 🔮 Future Scope
- Integration with GPS and camera systems  
- AI/ML-based predictive traffic analysis  
- Vehicle-to-Everything (V2X) communication  
- Deployment in real-world vehicles  
- Cloud-based traffic monitoring  

---

## 👨‍💻 Contributors
- G.V.S.K.Akhil  
- E.Deepsika Reddy
- Sk Sarah
