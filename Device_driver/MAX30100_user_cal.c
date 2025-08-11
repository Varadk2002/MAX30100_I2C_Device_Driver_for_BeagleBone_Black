#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <string.h>

#define DEVICE_FILE "/dev/max30100"
#define NUM_SAMPLES 16

#define NO_FINGER_THRESHOLD 300

// SpO2 calculation function moved from the driver
int calculate_spo2(uint16_t *ir_buffer, uint16_t *red_buffer)
{
    uint32_t dc_ir = 0, dc_red = 0;
    uint16_t ir_min = 0xFFFF, ir_max = 0;
    uint16_t red_min = 0xFFFF, red_max = 0;
    uint32_t ac_ir, ac_red;
    uint64_t R_numerator, R_denominator;
    int spo2;
    int i;

    // Calculate DC averages and find AC peaks/troughs
    for (i = 0; i < NUM_SAMPLES; i++) {
        uint16_t ir = ir_buffer[i];
        uint16_t red = red_buffer[i];

        dc_ir += ir;
        dc_red += red;

        if (ir < ir_min) ir_min = ir;
        if (ir > ir_max) ir_max = ir;

        if (red < red_min) red_min = red;
        if (red > red_max) red_max = red;
    }
    
    if ((dc_ir / NUM_SAMPLES) < NO_FINGER_THRESHOLD || (dc_red / NUM_SAMPLES) < NO_FINGER_THRESHOLD) {
        printf("No finger detected. Returning 0.\n");
        return 0; // Return 0 for no finger
    }
    
    // Calculate AC values
    ac_ir = ir_max - ir_min;
    ac_red = red_max - red_min;

    if (ac_ir == 0 || ac_red == 0 || dc_ir == 0 || dc_red == 0) {
        return -1; // Indicate invalid data if any part is zero
    }
    
    // Fixed-point R calculation.
    R_numerator = (uint64_t)ac_red * (dc_ir / NUM_SAMPLES) * 10000;
    R_denominator = (uint64_t)ac_ir * (dc_red / NUM_SAMPLES);
    
    if (R_denominator == 0) {
        return -1; 
    }
    
    uint64_t R = R_numerator / R_denominator;

    // SpO2 calculation using a simplified formula
    spo2 = 110 - (25 * R / 10000);
    
    if (spo2 > 100) {
        printf("Calculated SpO2 greater than 100, capping at 100.\n");
        spo2 = 100;
    }
    return spo2;
}

int main()
{
    int fd;
    uint16_t *buffer;
    uint16_t ir_data[NUM_SAMPLES];
    uint16_t red_data[NUM_SAMPLES];
    int data_size = NUM_SAMPLES * sizeof(uint16_t) * 2; // size for all samples
    int spo2_value;

    printf("Opening device file: %s\n", DEVICE_FILE);
    fd = open(DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device file");
        return -1;
    }

    buffer = (uint16_t *)malloc(data_size);
    if (!buffer) {
        perror("Failed to allocate memory");
        close(fd);
        return -1;
    }

    while (1) {
        printf("\nReading %d bytes of raw IR and Red data...\n", data_size);

        // Read the data from the driver
        int bytes_read = read(fd, buffer, data_size);
        if (bytes_read < 0) {
            perror("Failed to read from the device");
            break;
        }
        
        printf("Successfully read %d bytes. Parsing data...\n", bytes_read);

        // Parse the interleaved data into separate IR and Red arrays
        for (int i = 0; i < NUM_SAMPLES; i++) {
            ir_data[i] = buffer[i * 2];
            red_data[i] = buffer[i * 2 + 1];
        }

        // Perform the SpO2 calculation in user space
        spo2_value = calculate_spo2(ir_data, red_data);

        if (spo2_value < 0) {
            printf("Error: Invalid data for SpO2 calculation.\n");
        } else {
            printf("Calculated SpO2: %d%%\n", spo2_value);
        }
        
        printf("Waiting for 2 seconds...\n");
        sleep(2);
    }
    
    free(buffer);
    close(fd);

    return 0;
}