# Modbus/TCP Attack Simulation Suite

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Windows-blue.svg)](https://www.microsoft.com/windows)
[![Language](https://img.shields.io/badge/Language-C-green.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

## 📋 Overview

This repository contains the source code for a **Modbus/TCP attack simulation suite** developed for educational and research purposes. The tool demonstrates security vulnerabilities in the Modbus/TCP protocol.

**Research Paper:**
> "Experimental Evaluation and Defense-in-Depth Mitigation of Modbus/TCP Security Vulnerabilities in Industrial Control Systems"
> 
> **Authors:** Mahmoud A. Khalifa, Ahmad Taher Azar, Walid El-Shafai  
> **Institution:** Automated Systems and Computing Lab (ASCL), Prince Sultan University, Saudi Arabia  
> **Contact:** mzian@psu.edu.sa

---

## ⚠️ DISCLAIMER

**THIS SOFTWARE IS FOR EDUCATIONAL AND AUTHORIZED SECURITY RESEARCH ONLY.**

- Only use on systems you own or have explicit permission to test
- Unauthorized access to computer systems is **illegal**
- The authors are not responsible for any misuse

---

## 🎬 Video Demonstration

| Video | Description | Link |
|-------|-------------|------|
| **Part 1** | Compilation Tutorial | [YouTube](https://youtu.be/YOUR_VIDEO_ID_1) |
| **Part 2** | Attack Execution Demo | [YouTube](https://youtu.be/YOUR_VIDEO_ID_2) |

---

## 📁 Repository Contents

```
modbus-tcp-attack-suite/
├── Modbus_attack.c      ← Main attack source code
├── compile.bat          ← One-click compilation script
├── README.md            ← This file
├── LICENSE              ← MIT License
└── images/              ← Screenshots from research
    ├── modbussim_config.png
    ├── normal_off_state.png
    ├── normal_on_state.png
    ├── active_attack.png
    ├── post_attack.png
    └── wireshark_capture.png
```

---

## 🎯 Attack Phases (MITRE ATT&CK for ICS)

| Phase | Function Code | MITRE Technique | Description |
|-------|---------------|-----------------|-------------|
| **1. Reconnaissance** | FC01 (Read Coils) | T0888 | Enumerate device state |
| **2. Coil Manipulation** | FC05 (Write Coil) | T0855 | Toggle actuators ON/OFF |
| **3. Process Monitoring** | FC03 (Read Registers) | T0801 | Extract process data |
| **4. Setpoint Tampering** | FC06 (Write Register) | T0836 | Modify control parameters |

---

# 🚀 QUICK START GUIDE

## Step 1: Install GCC Compiler

### Option A: Install MSYS2 (Recommended)

1. Download MSYS2 from: https://www.msys2.org/
2. Run the installer, accept defaults
3. Open "MSYS2 MINGW64" terminal
4. Run these commands:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc
   ```
5. Add to Windows PATH:
   - Open: Control Panel → System → Advanced → Environment Variables
   - Edit `Path` variable
   - Add: `C:\msys64\mingw64\bin`
6. Verify in Command Prompt:
   ```bash
   gcc --version
   ```

## Step 2: Download This Repository

```bash
git clone https://github.com/ASCL-PSU/modbus-tcp-attack-suite.git
cd modbus-tcp-attack-suite
```

Or download ZIP and extract.

## Step 3: Compile the Code

### Option A: Double-click compile.bat

### Option B: Command line
```bash
gcc Modbus_attack.c -o modbus_attack.exe -lws2_32
```

## Step 4: Run the Attack

```bash
modbus_attack.exe 192.168.1.100
```

Replace `192.168.1.100` with your target ModBus_SIM IP address.

---

# 🔬 COMPLETE LAB SETUP GUIDE

This section explains how to set up the complete testing environment as used in our research paper.

## Required Software

| Software | Purpose | Download |
|----------|---------|----------|
| ModBus_SIM (ModRSsim2) | Modbus/TCP Server Simulator | [SourceForge](https://sourceforge.net/projects/modrssim2/) |
| Wireshark | Network Traffic Capture | [wireshark.org](https://www.wireshark.org/download.html) |
| MinGW-w64 | C Compiler | [MSYS2](https://www.msys2.org/) |

---

## 📡 Setting Up ModBus_SIM (Target Server)

### Step 1: Download and Extract
1. Go to: https://sourceforge.net/projects/modrssim2/
2. Download `ModRSsim2.zip`
3. Extract to any folder (e.g., `C:\ModBus_SIM`)

### Step 2: Run ModRSsim2
1. Double-click `ModRSsim2.exe`
2. The simulator window will open

### Step 3: Configure Network Settings
1. Go to **Setup** → **Ethernet TCP/IP Settings**
2. Configure as shown:

![ModBus_SIM Configuration](images/modbussim_config.png)

| Setting | Value |
|---------|-------|
| **Port** | 502 |
| **Max Connections** | 10 |
| **Socket Timeout** | 5000 |

3. Click **OK**

### Step 4: Start the Server
1. Click **Start** button (or press F5)
2. Status bar should show: "Listening on port 502"
3. Note your IP address (shown in window title or use `ipconfig`)

### Step 5: Understanding the Display

**Normal OFF State** - All coils cleared (0x0000):
![Normal OFF State](images/normal_off_state.png)

**Normal ON State** - All coils set (0xFFFF):
![Normal ON State](images/normal_on_state.png)

---

## 🦈 Setting Up Wireshark (Traffic Capture)

### Step 1: Download and Install
1. Go to: https://www.wireshark.org/download.html
2. Download "Windows x64 Installer"
3. Run installer
4. **Important:** Install Npcap when prompted (required for capture)

### Step 2: Start Capture
1. Open Wireshark
2. Select your network interface:
   - **Ethernet** - if using wired connection
   - **Wi-Fi** - if using wireless
   - **Loopback** - if testing on same machine (127.0.0.1)

### Step 3: Apply Modbus Filter
In the filter bar, type:
```
modbus || tcp.port == 502
```

Press Enter to apply.

### Step 4: Capture Traffic
1. Click the blue shark fin button to start capture
2. Run your attack
3. Click red square to stop capture
4. Save: File → Save As → `attack_capture.pcapng`

### Understanding Wireshark Capture

![Wireshark Capture](images/wireshark_capture.png)

**What you'll see:**
- **Transaction ID** - Identifies request/response pairs
- **Protocol ID** - Always 0x0000 for Modbus
- **Function Code** - FC01, FC03, FC05, FC06
- **Register/Coil Addresses** - Target addresses
- **Data Values** - Plaintext values (NO ENCRYPTION!)

---

## 🎯 Running the Complete Attack Simulation

### Step 1: Prepare Your Environment

Open **3 windows** side by side:

```
+------------------+------------------+------------------+
|                  |                  |                  |
|   ModBus_SIM     |    Wireshark     |  Command Prompt  |
|   (Target)       |   (Capture)      |   (Attack)       |
|                  |                  |                  |
+------------------+------------------+------------------+
```

### Step 2: Start ModBus_SIM
1. Run ModRSsim2.exe
2. Configure port 502
3. Click Start
4. Note the IP address

### Step 3: Start Wireshark Capture
1. Open Wireshark
2. Select network interface
3. Apply filter: `tcp.port == 502`
4. Click Start Capture (blue fin)

### Step 4: Launch the Attack
```bash
cd C:\path\to\modbus-tcp-attack-suite
modbus_attack.exe 192.168.1.100
```

### Step 5: Observe Results

**In ModBus_SIM:**
- Watch coils toggle ON/OFF rapidly
- See register values change to 1234

**Active Attack** - Coils forced ON:
![Active Attack](images/active_attack.png)

**Post Attack** - System state after attack cycle:
![Post Attack](images/post_attack.png)

**In Wireshark:**
- See all Modbus packets in plaintext
- Observe function codes (01, 03, 05, 06)
- Note: ALL DATA IS VISIBLE (no encryption!)

### Step 6: Stop and Analyze
1. Press `Ctrl+C` in Command Prompt to stop attack
2. Click red square in Wireshark to stop capture
3. Analyze the captured traffic

---


# 📊 MITRE ATT&CK Mapping Table

| Attack Phase | Technique ID | Technique Name | Tactic | Real-World Malware |
|--------------|--------------|----------------|--------|-------------------|
| Reconnaissance | T0888 | Remote System Information Discovery | Discovery | INCONTROLLER |
| Coil Manipulation | T0855 | Unauthorized Command Message | Impair Process | FrostyGoop |
| Register Tampering | T0836 | Modify Parameter | Impair Process | Industroyer |
| Process Monitoring | T0801 | Monitor Process State | Collection | VPNFilter |
| Denial of Service | T0814 | Denial of Service | Inhibit Response | INCONTROLLER |

---

# 🛡️ Defense Recommendations

Based on our research, implement these countermeasures:

1. **Network Segmentation** - Isolate OT networks per IEC 62443
2. **Authentication** - Deploy Modbus proxies with authentication
3. **Encryption** - Use TLS/VPN tunnels for Modbus traffic
4. **Monitoring** - Deploy IDS with Modbus-aware rules
5. **Rate Limiting** - Implement command frequency limits

---

# 📚 References

- MITRE ATT&CK for ICS: https://attack.mitre.org/techniques/ics/
- FrostyGoop (C0041): https://attack.mitre.org/campaigns/C0041/
- INCONTROLLER (S1045): https://attack.mitre.org/software/S1045/
- CISA Alert AA22-103A: https://www.cisa.gov/news-events/cybersecurity-advisories/aa22-103a
- IEC 62443 Standard
- NIST SP 800-82

---

# 📄 License

MIT License - See [LICENSE](LICENSE) file.

---

# 📧 Contact

- **Mahmoud A. Khalifa** - mzian@psu.edu.sa (Corresponding Author)

Prince Sultan University, Riyadh, Saudi Arabia

---

# 🙏 Acknowledgments

This research was funded by the Research, Development, and Innovation Authority (RDIA), Kingdom of Saudi Arabia, with grant number 13382-psu-2023-PSNU-R-3-1-EI.
