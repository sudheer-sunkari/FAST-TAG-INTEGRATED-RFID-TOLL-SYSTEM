# 🚗 FAST-TAG Integrated RFID Toll System

## 📌 Overview

This project implements an **automated toll collection system using RFID technology**.

The system detects a vehicle approaching the toll gate using a **GP2D12 distance sensor**. Once the vehicle is detected, the system asks the driver to scan the RFID card.

The RFID reader sends the card information to the **LPC2148 microcontroller** through serial communication. The controller checks the card details and retrieves the corresponding vehicle information and available balance stored in **EEPROM**.

If the RFID card is valid and sufficient balance is available, the toll amount is deducted automatically and the toll gate is opened using a **DC motor**.

The system also provides options for:

* 🚗 Automatic vehicle detection
* 🏷️ RFID card identification
* 💰 Automatic toll amount deduction
* 💳 Fast-Tag balance management
* 🔋 Manual recharge using a keypad
* ✍️ Manual toll deduction in case of RFID issues
* 🚧 Automatic toll gate opening and closing

The project demonstrates the use of **RFID, UART, I2C, EEPROM, LCD, keypad, distance sensor, interrupts, and DC motor control** in an embedded system.

---

## 🎯 Aim

To design and implement an **automated toll collection system using RFID technology**.

The system identifies vehicles using RFID cards, verifies the available balance, deducts the toll amount, and automatically controls the toll gate.

---

## ⚙️ System Architecture

The following block diagram illustrates the overall architecture of the **FAST-TAG Integrated RFID Toll System**.

<img width="1189" height="896" alt="Block_diagram_" src="https://github.com/user-attachments/assets/f051853f-916b-442d-a6d1-08f5126573e7" />

The **LPC2148 microcontroller** acts as the main controller and manages all the peripheral modules.

---

## 🧠 System Modules

### 1️⃣ Vehicle Detection Module

The **GP2D12 distance sensor** detects the presence of a vehicle near the toll gate.

When the vehicle comes within approximately **10 to 15 cm**, the system displays:

```text
Welcome to Toll Plaza
Waiting for Card....
```

The system then waits for the RFID card to be scanned.

---

### 2️⃣ RFID Card Module

The RFID reader identifies the vehicle using an RFID card.

When the card is placed near the RFID reader:

* 🏷️ The reader reads the card information.
* 📡 The data is sent to the controller through serial communication.
* ⚡ UART communication operates at **9600 baud rate**.
* 🧠 The LPC2148 processes the received card number.

The RFID reader sends card data when the card is placed near it.

---

### 3️⃣ Card Verification Module

After receiving the RFID card number, the controller checks whether the card matches the predefined Fast-Tag information.

The system retrieves:

* 🏷️ Fast-Tag ID
* 🚗 Vehicle Number
* 💰 Available Balance

This information is stored in **EEPROM** and displayed on the LCD.

---

### 4️⃣ Automatic Toll Deduction Module

Once the RFID card is verified, the system deducts the toll amount from the available Fast-Tag balance.

```text
Available Balance
        │
        ▼
   Toll Deduction
        │
        ▼
 Updated Balance
```

After successful deduction, the system proceeds to open the toll gate.

---

### 5️⃣ EEPROM Memory Module

The **AT24C256 EEPROM** is used to store vehicle-related information.

Stored information can include:

* 🏷️ RFID / Fast-Tag ID
* 🚗 Vehicle Number
* 💰 Available Balance

The controller communicates with EEPROM using the required communication interface and retrieves the information when the corresponding RFID card is scanned.

---

### 6️⃣ LCD Display Module 📟

A **20×4 LCD** is used to display system information.

The LCD displays:

* 🚗 Vehicle detection status
* 🏷️ RFID card information
* 🚘 Vehicle number
* 💰 Available balance
* 💸 Toll deduction status
* 🚧 Gate status
* ⚠️ Error or waiting messages

Example display:

```text
FAST-TAG TOLL SYSTEM

ID: 12345678
VEHICLE: AP XX XXXX
BALANCE: ₹500
```

---

### 7️⃣ Keypad Module ⌨️

A **4×4 matrix keypad** is used for manual user input.

The keypad is mainly used for:

* 💰 Entering recharge amount
* ✍️ Manual toll amount processing
* 🔢 Entering required numeric values

---

### 8️⃣ Recharge Module 💳

The system provides a manual recharge facility.

When **Switch 1** is activated:

1. The recharge option is selected.
2. The user enters the recharge amount using the keypad.
3. The available balance is updated.
4. The updated balance can be displayed.

This feature allows the Fast-Tag balance to be recharged manually.

---

### 9️⃣ Manual Toll Deduction Module ✍️

Sometimes an RFID card may have an issue or may not be detected properly.

In this situation, **Switch 2** is used.

The toll amount can be manually entered using the keypad, allowing the toll collection process to continue.

---

### 🔟 Toll Gate Control Module 🚧

A **DC motor** is used to represent the toll gate.

After successful card verification and balance deduction:

* 🔓 DC motor rotates clockwise → **Gate Opens**
* 🚗 Vehicle passes through the toll gate
* 🔒 When the vehicle moves away from the sensor, the motor rotates in the opposite direction → **Gate Closes**

The project specifies that the gate closes when the vehicle moves approximately **15 cm away from the sensor**.

---

## 🧰 Hardware Requirements

| Component                | Description                                         |
| ------------------------ | --------------------------------------------------- |
| 🧠 LPC2148               | Main ARM7 microcontroller                           |
| 🏷️ RFID Reader          | Reads RFID card information                         |
| 💳 RFID Cards            | Used for vehicle identification                     |
| 📟 20×4 LCD              | Displays system information                         |
| ⌨️ 4×4 Matrix Keypad     | Used for manual input                               |
| 🔘 Switches              | Used for recharge and manual deduction              |
| 💾 AT24C256              | EEPROM for storing information                      |
| 🚧 DC Motor              | Controls toll gate simulation                       |
| 📡 GP2D12 Sensor         | Detects vehicle presence                            |
| 🔌 USB-to-UART Converter | Used for serial communication/programming interface |

---

## 💻 Software Requirements

* 💻 Embedded C
* 🛠️ Keil µVision / Keil C Compiler
* ⚡ Flash Magic

---

## 🛠️ Project Modules

### 📟 LCD Module

Used to display:

* Welcome messages
* RFID card details
* Vehicle number
* Available balance
* Toll deduction information
* Gate status

---

### 📡 UART Module

Used for communication between:

```text
RFID Reader
      │
      │ Serial Communication
      ▼
   LPC2148
```

The RFID card number is received through **UART communication at 9600 baud rate**.

---

### 💾 I2C Module

Used for communication with the **AT24C256 EEPROM**.

The EEPROM stores the required Fast-Tag and vehicle information.

---

### ⌨️ Keypad Module

Used for:

* Recharge amount entry
* Manual toll amount entry
* Numeric input operations

---

### 📡 Distance Sensor Module

The GP2D12 sensor continuously monitors the area near the toll gate.

```text
Vehicle Near
     │
     ▼
Sensor Detects Vehicle
     │
     ▼
Display:
"Welcome to Toll Plaza"
"Waiting for Card...."
```

---

### 🏷️ RFID Module

Responsible for:

* Reading RFID cards
* Sending card data
* Identifying the Fast-Tag
* Communicating with the LPC2148

---

### 🚧 DC Motor Module

Responsible for toll gate operation.

```text
Valid RFID Card
       │
       ▼
Balance Deducted
       │
       ▼
Motor Clockwise
       │
       ▼
🚧 GATE OPEN
       │
       ▼
Vehicle Moves Away
       │
       ▼
Motor Anti-Clockwise
       │
       ▼
🚧 GATE CLOSED
```

---

### 🚨 Interrupt Module

Two switches are used for special operations:

**🔘 Switch 1**

* Used for Fast-Tag recharge.

**🔘 Switch 2**

* Used for manual toll deduction when RFID-related issues occur.

---

## 🔄 Project Workflow

### 1️⃣ Vehicle Detection

The GP2D12 distance sensor detects a vehicle approaching the toll gate.

### 2️⃣ Welcome Message

When the vehicle is detected, the LCD displays:

```text
Welcome to Toll Plaza
Waiting for Card....
```

### 3️⃣ RFID Card Scanning

The driver places the RFID card near the RFID reader.

### 4️⃣ Card Data Reception

The RFID reader sends the card information to the LPC2148 through UART communication.

### 5️⃣ Card Verification

The microcontroller checks the received RFID card number.

### 6️⃣ Retrieve Vehicle Information

The controller retrieves:

* 🏷️ Fast-Tag ID
* 🚗 Vehicle Number
* 💰 Available Balance

### 7️⃣ Toll Amount Deduction

The required toll amount is deducted from the available balance.

### 8️⃣ Gate Opening

The DC motor rotates clockwise to represent opening the toll gate.

### 9️⃣ Vehicle Movement

The vehicle passes through the toll gate.

### 🔟 Gate Closing

When the vehicle moves away from the sensor, the DC motor rotates in the opposite direction to close the gate.

This overall sequence follows the implementation flow described in the project document.

---

## 📊 System Output

### 🚗 Vehicle Detected

```text
Welcome to Toll Plaza
Waiting for Card....
```

### 🏷️ RFID Card Verified

```text
FAST-TAG ID: XXXXXXXX
VEHICLE: AP XX XXXX
BALANCE: ₹XXX
```

### 💰 After Toll Deduction

```text
TOLL DEDUCTED
GATE OPEN
```

### 🚧 Vehicle Leaves

```text
THANK YOU
GATE CLOSED
```

---

## 🧩 System Overview

The **FAST-TAG Integrated RFID Toll System** is designed to automate the toll collection process using RFID technology.

The system works through the following flow:

```text
🚗 Vehicle Approaches
        │
        ▼
📡 GP2D12 Sensor Detects Vehicle
        │
        ▼
📟 LCD: "Welcome to Toll Plaza"
        │
        ▼
🏷️ RFID Card Scanned
        │
        ▼
📡 RFID Data Received through UART
        │
        ▼
🧠 LPC2148 Verifies Card
        │
        ▼
💾 Retrieve Vehicle & Balance Details
        │
        ▼
💰 Toll Amount Deducted
        │
        ▼
🚧 DC Motor Opens Gate
        │
        ▼
🚗 Vehicle Passes Through
        │
        ▼
📡 Sensor Detects Vehicle Moving Away
        │
        ▼
🚧 DC Motor Closes Gate
```

---

## 🚀 Features

✔ Automated toll collection system

✔ RFID-based vehicle identification

✔ Automatic vehicle detection

✔ Fast-Tag balance verification

✔ Automatic toll amount deduction

✔ EEPROM-based vehicle information storage

✔ 20×4 LCD status display

✔ Manual recharge facility

✔ Manual toll deduction option

✔ Automatic toll gate opening

✔ Automatic toll gate closing

✔ UART communication

✔ I2C communication

✔ Interrupt-based switch handling

---

## ✨ Applications

This project can be applied or extended to:

* 🚧 Automated toll plazas
* 🏙️ Smart transportation systems
* 🅿️ RFID-based parking systems
* 🚗 Vehicle access control
* 🏢 Smart gate automation
* 💳 Contactless payment systems
* 🌐 IoT-based toll monitoring systems

---

## 💡 Future Enhancements

The project can be enhanced by adding:

📷 **Camera / Number Plate Recognition**
Automatically identify the vehicle number.

📱 **GSM Module**
Send SMS notifications after toll deduction.

🌐 **IoT Integration**
Upload toll and vehicle information to a cloud server.

📍 **GPS Tracking**
Track vehicle location.

💳 **Online Recharge System**
Allow users to recharge Fast-Tag balance through a mobile or web application.

📊 **Cloud Database**
Store vehicle and transaction history online.

📱 **Mobile Application**
Display:

* Current balance
* Toll transaction history
* Recharge details
* Vehicle information

🚨 **Security Alerts**
Generate alerts for invalid or unauthorized RFID cards.

---

## 🏁 Conclusion

The **FAST-TAG Integrated RFID Toll System** demonstrates an automated approach to toll collection using **RFID technology and the LPC2148 microcontroller**.

The system detects vehicles, reads RFID cards, retrieves vehicle and balance information, deducts the toll amount, and controls the toll gate automatically.

The project also provides additional functionality such as:

* 💰 Manual Fast-Tag recharge
* ✍️ Manual toll deduction
* 📟 Real-time LCD display
* 🚧 Automatic gate control
* 💾 EEPROM-based information storage

This project demonstrates practical knowledge of:

**Embedded C • LPC2148 • UART • I2C • RFID • EEPROM • LCD • Keypad • Distance Sensor • Interrupts • DC Motor Control**

---

## 👨‍💻 Author

**SUNKARI SUDHEER**

🎓 Electronics and Communication Engineering

🔧 Embedded Systems Enthusiast

💻 Developer of **FAST-TAG Integrated RFID Toll System**

---

## 📜 License

This project is developed for **academic and learning purposes**.

You are free to study, modify, and enhance the project with proper credit.

---

## ⭐ Support

If you find this project useful, consider giving it a **⭐ Star** on GitHub!

**Happy Coding! 🚗🏷️🚧**
